import pycuda.autoinit
import pycuda.driver as drv
import pycuda.gpuarray as gpuarray
from pycuda.compiler import SourceModule

from jinja2 import Template

import numpy
from math import *


#
#
# Math Module
#
#

class Vector2D:
    def __init__(self,x = 0,y = 0):
        self.x = x
        self.y = y
    
    def __sub__(a, b):
        return Vector2D(a.x - b.x, a.y - b.y)
    
    def __add__(a,b):
        return Vector2D(a.x + b.x, a.y + b.y)
    
    def Factor(self,a):
        return Vector2D(self.x*a, self.y*a)
    
    @staticmethod
    def Mult(v1, v2):
        return Vector2D(v1.x*v2.x, v1.y*v2.y)
    
    @staticmethod
    def DotProduct(v1, v2):
        return v1.x*v2.x + v1.y*v2.y
    
    def Length(self):
        return sqrt(self.x*self.x + self.y*self.y)
    
    def Normalize(self):
        l = self.Length()
        self.x /= l 
        self.y /= l
    
    def Copy(self):
        return Vector2D(self.x, self.y)
    
    def __str__(self):
        return "< "+str(self.x)+", "+str(self.y)+" >"
    
    def __repr__(self):
        return str(self)
    
    def Sign(self):
        return Vector2D(sign(self.x), sign(self.y))

class Matrix2D:
    def __init__(self, a11, a12, a21, a22):
        """ Matrix format :
        | a11 a12 |
        | a21 a22 |
        """
        self.a11 = a11
        self.a12 = a12
        self.a21 = a21
        self.a22 = a22
    
    def Transform(self,v):
        """Transform 2D vector with this matrix"""
        return Vector2D(v.x*self.a11 + v.y*self.a21, v.x*self.a12 + v.y*self.a22)

#
#
# Fattal
#
#

TemplateFattal = Template("""
///////////////////////////
// Definition of structs
///////////////////////////
struct Ray
{
    float2 Position;
    float2 Direction;
    float Value;
    float __padding;
};

/////////////////////////
// Const template settings
/////////////////////////
// Uniform trad
const float2 GridDimension = make_float2({{gridDimX}},{{gridDimY}});
const float2 CellDimension = make_float2(1,1);
const float AbsortionCoeff = {{AbsortionCoeff}};
const float ScaterringCoeff = {{ScaterringCoeff}};
const float2 MainDirection = make_float2({{mainDirX}},{{mainDirY}});

__global__ void rayMarch(float *dest, Ray* res)
{
  const int i = threadIdx.x;
  dest[i] = res[i].Position.x;
}
""")

class CudaRay:
    mem_size = 6*numpy.float32(0).nbytes
    
    def __init__(self, pos, dir, value):
        self.position = pos
        self.direction = dir
        self.value = value
        self.sendToCuda = False
        
    def toCuda(self, struct_arr_ptr, indice):
        self.sendToCuda = True
        drv.memcpy_htod(int(struct_arr_ptr) + indice*self.mem_size, 
                        numpy.array([self.position.x, self.position.y, 
                                     self.direction.x, self.direction.y, 
                                     self.value],dtype=numpy.float32))

class Fattal2D:
    def __init__(self, size, nbLPVray, absortion=0.01, scattering=0.01):
        self.gridSize = size
        self.raysBuffers = []
        self.nbLPVray = nbLPVray
        self.cellSize = Vector2D(1,1)
        
        self.absortionCoeff=absortion
        self.scaterringCoeff=scattering
        
    def GetMainDirection(self, idDir):
        propagationOrientation = 1
        if((idDir % 2) == 0):
            propagationOrientation = -1
       # Setup vector
        if(idDir < 2):
            return Vector2D(propagationOrientation, 0)
        else:
            return Vector2D(0,propagationOrientation)

    def GenerateRayBuffers(self):
        # Generates samples
        samples = []
        offset = 1.0 / (self.nbLPVray+1);
        for i in range(self.nbLPVray):
            v = Vector2D(0.5, (i+1)*offset - 0.5)
            v.Normalize()
            samples.append(v)
        
        # Generate rays
        for id in range(4):
            mainDir = self.GetMainDirection(id)
            
            # --- init ori position
            OriPosition = Vector2D(0,0)
            if(mainDir.x == -1.0 or mainDir.y == -1.0):
                OriPosition = Vector2D(self.gridSize.x*self.cellSize.x, 
                                       self.gridSize.y*self.cellSize.y)
            
            # --- init trans matrix
            transMatrix = Matrix2D(0,0,0,0)
            NbCells = 0
            if(mainDir.x != 0):
                transMatrix.a11 = mainDir.x
                transMatrix.a22 = mainDir.x
                NbCells = self.gridSize.y
            else:
                transMatrix.a21 = mainDir.y
                transMatrix.a12 = mainDir.y
                NbCells = self.gridSize.x
            
            offset = Vector2D(0, 1)
            offset = transMatrix.Transform(offset)
        
            # --- Compute dim
            NbGroupRays =  NbCells
            nbGenerateRay = self.nbLPVray * NbGroupRays
        
            # --- Value attributes
            beamLow = NbGroupRays/2 - NbGroupRays*0.025;
            beamHigh = NbGroupRays/2 + NbGroupRays*0.025 + 1;
        
            # --- Create memory & Fill
            struct_arr_ptr = drv.mem_alloc(nbGenerateRay * CudaRay.mem_size)
            for k in range(NbGroupRays):
                for j in range(self.nbLPVray):
                    factor = (k+0.5) # / subLevel
                    rayPos = Vector2D(0,0)
                    rayDir = Vector2D(0,0)
                    rayValue = 0
                    
                    rayPos.x = OriPosition.x+offset.x*factor*self.cellSize.x;
                    rayPos.y = OriPosition.y+offset.y*factor*self.cellSize.y;
                    rayDir.x = transMatrix.Transform(samples[j]).x;
                    rayDir.y = transMatrix.Transform(samples[j]).y;
                    
                    if(j == (self.nbLPVray/2) and k >= beamLow and k <= beamHigh and id == 0):
                        rayValue = 100.0;
                    
                    r = CudaRay(rayPos, rayDir, rayValue)
                    r.toCuda(struct_arr_ptr, (self.nbLPVray*k+j))
            
            # --- Store for next usage
            self.raysBuffers.append((struct_arr_ptr,nbGenerateRay))
    
    def Initialize(self):
        # --- Generate rays
        self.GenerateRayBuffers()
        
        # --- Load kernel modules
        self.Fattal2DKernels = []
        for i in range(4):
            mainDir = self.GetMainDirection(i)
            
            # Template arguments
            rendered_TemplateFattal = TemplateFattal.render(
                        gridDimX=self.gridSize.x,
                        gridDimY=self.gridSize.y,
                        AbsortionCoeff=self.absortionCoeff,
                        ScaterringCoeff=self.scaterringCoeff,
                        mainDirX=mainDir.x,
                        mainDirY=mainDir.y)      
            print rendered_TemplateFattal
               
            mod = SourceModule(rendered_TemplateFattal)
            self.Fattal2DKernels.append(mod.get_function("rayMarch"))
        
    def Compute(self):
        for i in range(4):
            dest = numpy.zeros(self.raysBuffers[i][1]).astype(numpy.float32)
            self.Fattal2DKernels[i](
                drv.Out(dest), self.raysBuffers[i][0],
                block=(self.raysBuffers[i][1],1,1), grid=(1,1))
            print dest
if __name__=="__main__":
    technique = Fattal2D(Vector2D(64,64), 9)
    technique.Initialize()
    technique.Compute()


