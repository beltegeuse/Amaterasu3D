import pycuda.autoinit
import pycuda.driver as drv
import pycuda.gpuarray as gpuarray
from pycuda.compiler import SourceModule
from pycuda.elementwise import ElementwiseKernel

import timeit

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
# Utils
#
#
def print_source_code(code):
     for i,line in enumerate(code.split('\n')):
         print str(i+1) + line

#
#
# Fattal
#
#

# --- this template describe the uniform grid traversing
TemplateFattal = Template("""
///////////////////////////
// Includes
///////////////////////////
#include <cuda.h>
#include <cutil_math.h>


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
// Helper function
/////////////////////////
// Maths
inline __device__ int2 sign(const float2& v)
{
    return make_int2( v.x < 0.0 ? -1 : v.x == 0 ? 0 : 1,
                      v.y < 0.0 ? -1 : v.y == 0 ? 0 : 1);
}

inline __device__ float2 floor(const float2& v)
{
    return make_float2(floor(v.x), floor(v.y));
}

// Grid
inline __device__ bool isInGrid(const float2& voxID, const int2& GridDimension)
{
    return voxID.x >= 0.0 && voxID.y >= 0.0 && 
           voxID.x < GridDimension.x && voxID.y < GridDimension.y;
}

{{code['defines']}}

/////////////////////////
// Kernel prototype
/////////////////////////
extern "C"
{
__global__ void Fattal2DKernel(Ray* rays, {{code['def']}});
}

/////////////////////////
// Kernel sources
/////////////////////////
inline __global__ void Fattal2DKernel(Ray* rays, {{code['def']}})
{
    /////////////////////////
    // Const template settings
    /////////////////////////
    // Uniform trad
    const int LPMDimension = {{nbLPM}};
    const int2 GridDimension = make_int2({{gridDimX}},{{gridDimY}});
    const float2 CellDimension = make_float2(1,1);
    const float AbsortionCoeff = {{AbsortionCoeff}};
    const float ScaterringCoeff = {{ScaterringCoeff}};
    const float2 MainDirection = make_float2({{mainDirX}},{{mainDirY}});
    
    // Precompute things
    const float CellVolume = CellDimension.x*CellDimension.y;
    const float2 GridWorldDimension = make_float2(GridDimension.x*CellDimension.x,GridDimension.y*CellDimension.y);

    ////////////////////////////
    // Get Data
    ////////////////////////////
    // Get the thread ID
    const int dataID = blockIdx.x * (blockDim.x*blockDim.y*blockDim.z) + threadIdx.x;
    
    // * Direction
    float2 Direction = normalize(rays[dataID].Direction);
    float2 DirectionAbs = make_float2(abs(Direction.x), abs(Direction.y));
    int2 sDelta = sign(Direction);
    float maxDirectionCoord = max(DirectionAbs.x,DirectionAbs.y);
    
    // * Position
    float2 OriPositon = rays[dataID].Position;
    float2 Position = OriPositon+Direction*0.0001; // already mult by GridDimension
    float2 voxID = floor(Position/CellDimension);
    float2 voxWorldPos = voxID*CellDimension;
    
    ///////////////////////
    // Configuration of 
    // uniform grid traversing
    ///////////////////////
    
    /////////////////////////////////
    // Initialisation tDelta
    /////////////////////////////////
    float2 tDelta = make_float2(100000);
    if(Direction.x != 0)
        tDelta.x = CellDimension.x/abs(Direction.x);
    if(Direction.y != 0)
        tDelta.y = CellDimension.y/abs(Direction.y);
    
    /////////////////////////////////
    // Initialise tMax
    /////////////////////////////////
    float2 tMax = make_float2(100000);
    // * x Initialisation
    if(Direction.x < 0)
        tMax.x = (voxWorldPos.x - Position.x) / Direction.x;
    else if(Direction.x > 0)
        tMax.x = (voxWorldPos.x + CellDimension.x - Position.x) / Direction.x;
    // * y Initialisation
    if(Direction.y < 0)
        tMax.y = (voxWorldPos.y - Position.y) / Direction.y;
    else if(Direction.y > 0)
        tMax.y = (voxWorldPos.y + CellDimension.y - Position.y) / Direction.y;
    
    ////////////////////////////////
    // Loop (Ray martching )
    ////////////////////////////////
    // Protection for reinjection steps
    bool isNeedRecast = true;
    //int nbIntersection;
    //int SumIntersection = 0;
    
    // Custom_values
    {{code['init']}}
    
    // loop variables
    float OldCurrentLenght = 0.0;
    float2 Offset = make_float2(0.0);
    float Dist;
    
    // know the main direction
    bool xMainDirection = DirectionAbs.x > DirectionAbs.y;
    
    while(isNeedRecast)
    {
        isNeedRecast = false;
        //nbIntersection = 0;
        while(true)
        {
            // March in the volume
            if(tMax.x < tMax.y)
            {
                Dist = tMax.x;
                tMax.x += tDelta.x;
            }
            else
            {
                Dist = tMax.y;
                tMax.y += tDelta.y;
            }
            
            // Get the length & Update OldCurrentLenght for next loop
            float DiffLength = Dist - OldCurrentLenght;
            OldCurrentLenght = Dist;

            // Update variables
            float2 NewPosition = OriPositon + (Dist)*Direction + Offset;
            voxID = floor(NewPosition/CellDimension); // Be careful
            if(!isInGrid(voxID, GridDimension))
            {
                break;
            }
                
            Position = NewPosition;

            {{code['loop']}}
            
            // Protection
            //nbIntersection++;
        }
        
        //SumIntersection += nbIntersection;
        // Protection for looping
        //FIXME
        //if(nbIntersection == 0 || SumIntersection > (GridDimension.x*2)) // < if no intersection => Quit
        //    break;
        
        // Compute relooping :)
        if(xMainDirection)
        {
            if(abs(Position.y) <= 0.01 || abs(Position.y-GridWorldDimension.y) <= 0.01 )
            {
                Position -= Offset;
                isNeedRecast = true;
                if(sDelta.y == -1)
                    Offset.y += GridWorldDimension.y;
                else
                    Offset.y -= GridWorldDimension.y;
            }
        }
        else
        {
            if(abs(Position.x) <= 0.01 || abs(Position.x-GridWorldDimension.x) <= 0.01)
            {
                Position -= Offset;
                isNeedRecast = true;
                if(sDelta.x == -1)
                    Offset.x += GridWorldDimension.x;
                else
                    Offset.x -= GridWorldDimension.x;
            }
        }
        Position += Offset;

        {{code['before_resend']}}
    }
    
    {{code['end']}}
}

""")

FattalComputeLPM = { 
            "def" : "uint* offsetBuffer, float* tempBuffer, float* UBuffer",
            "init" : """float rayValue = rays[dataID].Value;
             uint CurrIntersectionID = 0;
             uint memoryOffset = offsetBuffer[dataID];""",
            "loop" : """
            // Compute
            float scatteringTerm = rayValue*(1 - exp(-1*DiffLength*ScaterringCoeff/maxDirectionCoord));
            float extinctionCoeff = (ScaterringCoeff+AbsortionCoeff);
            float extinctionFactor = exp(-1*DiffLength*extinctionCoeff/maxDirectionCoord);
            float UValue = UBuffer[int(voxID.x*GridDimension.y + voxID.y)];
            rayValue = rayValue*extinctionFactor+(UValue*(1 - extinctionFactor)/extinctionCoeff);
            
            // Emit new values
            float DeltaData = scatteringTerm;
            // Compute Ars
            // TODO: nbRay
            if(xMainDirection)
                DeltaData *= CellDimension.y/LPMDimension;
            else
                DeltaData *= CellDimension.x/LPMDimension;
            
            // Write value
            tempBuffer[memoryOffset+CurrIntersectionID] = DeltaData;
            CurrIntersectionID++;""",
            "before_resend" : """// Reinitialise rayValue
            rayValue = 0;"""}

FattalPreComputePass = {
        "def" : "uint* destVoxID, uint* destNbIntersection",
        "init" : """
        int NbIntersection = 0;
        int oriWritePos = dataID*2*{{sizeForRays}};""",
        "loop" : """
            destVoxID[oriWritePos+NbIntersection] = voxID.x * GridDimension.y + voxID.y;
            //destVoxID[oriWritePos+NbIntersection] = dataID;
            NbIntersection++;
        """,
        "end" : "destNbIntersection[dataID] = NbIntersection;"}

DebugPass = {
        "def" : "int* testID",
        "end" : "testID[dataID] = dataID;"
        }

TemplateCompaction = Template("""
struct Vox
{
    uint * nbRaysValueAdresse;
    uint ** raysValues;
};

extern "C"
{
    __global__ void Fattal2DCompaction(Vox* voxels, float* tempBuffer, float* UBuffer, float* IBuffer);
}

__global__ void Fattal2DCompaction(Vox* voxels, float* tempBuffer, float* UBuffer, float* IBuffer)
{
    /////////////////////////
    // Const template settings
    /////////////////////////
    uint DirectionIndice = {{DirNum}};
     
    ////////////////////////////
    // Get Data
    ////////////////////////////
    // Get the thread ID
    // TODO: Transformation
    const int dataID = blockIdx.x * (blockDim.x*blockDim.y*blockDim.z) + threadIdx.x;
    
    Vox voxel = voxels[dataID];
    // TODO: Normalisation by Volume
    // TODO: Factor 
    for(int i = 0; i < voxel.nbRaysValueAdresse[DirectionIndice]; i++)
    {
        float value = tempBuffer[voxel.raysValues[DirectionIndice][i]];
        UBuffer[dataID] += value / (4);
        IBuffer[dataID] += value ;
    }
    
    //IBuffer[dataID] = voxel.raysValues[DirectionIndice][0];
    //IBuffer[dataID] = tempBuffer[voxel.raysValues[DirectionIndice][0]];
    //IBuffer[dataID] = voxel.nbRaysValueAdresse[DirectionIndice];
}
""")

TemplateClear = Template("""
extern "C"
{
    __global__ void ClearKernel({{code['def']}});
}

__global__ void ClearKernel({{code['def']}})
{
    const int dataID = blockIdx.x * (blockDim.x*blockDim.y*blockDim.z) + threadIdx.x;
    {{code['main']}}
}
""")

ClearPass = {"def" : "float* buffer",
             "main" : "buffer[dataID] = 0;"}

#
# Cuda structures
#

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

class CudaVox:
    mem_size = numpy.intp(0).nbytes*2
    
    def __init__(self):
        self.voxDataBuffers = []
        self.voxDataBuffersPtr = None
        self.voxDataNb = []
        self.voxDataNbPtr = None
    def AddDataDirection(self, data):
        # Just allocated & send to the gpu
        self.voxDataBuffers.append(drv.to_device(numpy.array(data, dtype=numpy.uint32)))
        # Update numbers
        self.voxDataNb.append(len(data))
    
    def toCuda(self, struct_arr_ptr, indice):
        # Just allocated & send to the gpu
        self.voxDataNbPtr = drv.to_device(numpy.array(self.voxDataNb, dtype=numpy.uint32))
        convertBuffer = []
        for i in range(len(self.voxDataBuffers)):
            convertBuffer.append(numpy.intp(int(self.voxDataBuffers[i])))
        #print convertBuffer
        
        self.voxDataBuffersPtr = drv.to_device(numpy.array(convertBuffer, dtype=numpy.intp))
        # Send pointeurs in object memory
        ptr_current = int(struct_arr_ptr) + indice*self.mem_size
        drv.memcpy_htod(ptr_current, 
                        numpy.intp(int(self.voxDataNbPtr)))
        drv.memcpy_htod(ptr_current +  numpy.intp(0).nbytes,
                        numpy.intp(int(self.voxDataBuffersPtr)))
#    
# Technique class
#

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
                    rayValue = 0.0
                    
                    rayPos.x = OriPosition.x+offset.x*factor*self.cellSize.x;
                    rayPos.y = OriPosition.y+offset.y*factor*self.cellSize.y;
                    rayDir.x = transMatrix.Transform(samples[j]).x;
                    rayDir.y = transMatrix.Transform(samples[j]).y;
                    
                    if(j == (self.nbLPVray/2) and k >= beamLow and k <= beamHigh and id == 0):
                        rayValue = 1000.0;
                    
                    r = CudaRay(rayPos, rayDir, rayValue)
                    r.toCuda(struct_arr_ptr, (self.nbLPVray*k+j))
            
            # --- Store for next usage
            self.raysBuffers.append((struct_arr_ptr,nbGenerateRay))
    
    def DoPrecomputation(self):
        # --- init GPU buffers
        self.offsetBuffers = []
        self.cudaVoxObjects = []
        
        for i in range(self.gridSize.x*self.gridSize.y):
            self.cudaVoxObjects.append(CudaVox())
        
        # --- Allocated data
        for i in range(4):
            mainDir = self.GetMainDirection(i)
            # Get the grid dimension
            sizeVox = 0
            if(mainDir.x != 0):
                sizeVox = self.gridSize.x
            else:
                sizeVox = self.gridSize.y
            #########
            # Allocated buffer
            destVoxID = numpy.zeros(self.raysBuffers[i][1]*2*sizeVox, dtype=numpy.uint32)
            destNumIntersection = numpy.zeros(self.raysBuffers[i][1], dtype=numpy.uint32)
            
            ########
            # Build Cuda kernel
            PreComputeCode = TemplateFattal.render(
                    gridDimX=self.gridSize.x,
                    gridDimY=self.gridSize.y,
                    nbLPM=self.nbLPVray,
                    AbsortionCoeff=self.absortionCoeff,
                    ScaterringCoeff=self.scaterringCoeff,
                    mainDirX=mainDir.x,
                    mainDirY=mainDir.y,
                    code=FattalPreComputePass
            )
            
            # --- Custom flags
            PreComputeCode = Template(PreComputeCode).render(sizeForRays= sizeVox)
            #print_source_code(str(PreComputeCode))
            
            mod = SourceModule(PreComputeCode,
                               options=["-I /opt/cuda_sdk/CUDALibraries/common/inc"],
                               no_extern_c=True)
            func = mod.get_function("Fattal2DKernel")
            
            #############
            # Run Kernel
            kernelBlock = (1,1,1)
            kernelGrid = (self.raysBuffers[i][1],1)
            print "  --Pass " + str(mainDir)
            print "     * Kernel run ..."
            print "       -- Block : " + str(kernelBlock)
            print "       -- Grid : " + str(kernelGrid)
            
            func(self.raysBuffers[i][0], drv.Out(destVoxID), drv.Out(destNumIntersection),
                 block=kernelBlock, grid=kernelGrid)
            
            print "     * Data ..."
            print "       -- voxID : " + str(destVoxID)
            print "       -- NbInter : " + str(destNumIntersection)
            maxIntersection = max(destNumIntersection)
            
#            offset = 0
#            for idRay in range(self.raysBuffers[i][1]):
#                if(destNumIntersection[idRay] == maxIntersection):
#                    print "Ray data : " + str(mainDir)
#                    print " - num intersection " + str(destNumIntersection[idRay])
#                    print " - voxID intersection " + str(destVoxID[offset:offset+destNumIntersection[idRay]])
#                offset += sizeVox*2
            
            #--- Find doublon
            # TODO
            
            print "     * Build offsetMap ..."
            print "        -- init array"
            offset = 0
            offsetMap = (self.raysBuffers[i][1]) * [0]
            
            print "        -- fill array"
            for idRay in range(self.raysBuffers[i][1]):
                offsetMap[idRay] = offset
                offset += destNumIntersection[idRay]
            
            print "        -- send to GPU"
            self.offsetBuffers.append((drv.to_device(numpy.array(offsetMap, dtype=numpy.uint32)),
                                       offset))
            
            
            print "     * Build VoxID ..."
            print "        -- init array"
            voxID = []
            for idVox in range(self.gridSize.x*self.gridSize.y):
                voxID.append([])
            
            print "        -- fill array"
            for idRay in range(self.raysBuffers[i][1]):
                currentOffsetData = offsetMap[idRay]
                dataVoxIdOffset = (sizeVox*2*idRay)
                for currI in range(destNumIntersection[idRay]):
                    voxID[destVoxID[dataVoxIdOffset+currI]].append(currentOffsetData+currI)
            
#            for idVox in range(self.gridSize.x*self.gridSize.y):
#                print len(voxID[idVox])
#                print voxID[idVox]
#            print voxID
            
            print "        -- send to GPU"
            for idVox in range(self.gridSize.x*self.gridSize.y):
                self.cudaVoxObjects[idVox].AddDataDirection(voxID[idVox])
        
        # Send all vox On GPU
        print "  -- Pass Send VOX GPU"
        self.cudaVoxBuffer = drv.mem_alloc(len(self.cudaVoxObjects) * CudaVox.mem_size)
        for i in range(len(self.cudaVoxObjects)):
            self.cudaVoxObjects[i].toCuda(self.cudaVoxBuffer, i)
            
    def Initialize(self):
        # --- Generate rays
        print "Generate rays ..."
        self.GenerateRayBuffers()
        
        print "Compute precomputation..."
        print "Precomputation : %.2f sec" % timeit.Timer(self.DoPrecomputation).timeit(1)
        
        print "Preallocate ressources..."
        print "  -- temp buffers"
        self.tempBuffers = []
        for i in range(4):
         self.tempBuffers.append(drv.to_device(numpy.zeros(self.offsetBuffers[i][1], dtype=numpy.float32)))
        
        print "  -- U buffers"
        self.UBuffers = []
        for i in range(2):
            self.UBuffers.append(drv.to_device(numpy.zeros(self.gridSize.x*self.gridSize.y, dtype=numpy.float32)))
        
        print "  -- I buffer"
        self.IBuffer = drv.to_device(numpy.zeros(self.gridSize.x*self.gridSize.y, dtype=numpy.float32))
        
        print "Load Fattals Kernels ..."
        # --- Fattal Computation
        self.Fattal2DKernels = []
        for i in range(4):
            mainDir = self.GetMainDirection(i)
            
            # Template arguments
            rendered_TemplateFattal = TemplateFattal.render(
                        gridDimX=self.gridSize.x,
                        gridDimY=self.gridSize.y,
                        nbLPM=self.nbLPVray,
                        AbsortionCoeff=self.absortionCoeff,
                        ScaterringCoeff=self.scaterringCoeff,
                        mainDirX=mainDir.x,
                        mainDirY=mainDir.y,
                        code=FattalComputeLPM)
            
            mod = SourceModule(rendered_TemplateFattal,
                               options=["-I /opt/cuda_sdk/CUDALibraries/common/inc"],
                               no_extern_c=True)
            
            self.Fattal2DKernels.append(mod.get_function("Fattal2DKernel"))
        
        # --- Compaction Kernel
        self.CompactionKernels = []
        for i in range(4):
            rendered_TemplateCompaction = TemplateCompaction.render(
                                DirNum=i)
            #print_source_code(rendered_TemplateCompaction)
            
            mod = SourceModule(rendered_TemplateCompaction,
                               options=["-I /opt/cuda_sdk/CUDALibraries/common/inc"],
                               no_extern_c=True)
            self.CompactionKernels.append(mod.get_function("Fattal2DCompaction"))
        
        # --- Clear kernel
        rendered_TemplateClear = TemplateClear.render(
                                code=ClearPass)
        #print_source_code(rendered_TemplateCompaction)
        
        mod = SourceModule(rendered_TemplateClear,
                           options=["-I /opt/cuda_sdk/CUDALibraries/common/inc"],
                           no_extern_c=True)
        
        self.ClearKernel = mod.get_function("ClearKernel")
        
    def Compute(self, nbPass = 3):
        #Need to reset I buffer
        self.ClearKernel(self.IBuffer,
                         block=(32,1,1), grid=((self.gridSize.y*self.gridSize.x)/32,1))
        self.ClearKernel(self.UBuffers[0],
                         block=(32,1,1), grid=((self.gridSize.y*self.gridSize.x)/32,1))
        currentUBufferID = 0
        for idPass in range(nbPass):
            nextUBufferID = (currentUBufferID+1)%2
            self.ClearKernel(self.UBuffers[nextUBufferID],
                         block=(32,1,1), grid=((self.gridSize.y*self.gridSize.x)/32,1))
            for i in range(4):
                self.Fattal2DKernels[i](
                    self.raysBuffers[i][0], self.offsetBuffers[i][0], self.tempBuffers[i],self.UBuffers[currentUBufferID],
                    block=(1,1,1), grid=(self.raysBuffers[i][1],1))
                
                self.CompactionKernels[i](
                    self.cudaVoxBuffer, self.tempBuffers[i], 
                    self.UBuffers[nextUBufferID], self.IBuffer,
                    block=(32,1,1), grid=((self.gridSize.y*self.gridSize.x)/32,1))
            currentUBufferID = nextUBufferID
        drv.Context.synchronize()
                
if __name__=="__main__":
    technique = Fattal2D(Vector2D(128,128), 3, scattering=0.05, absortion=0.01)
    technique.Initialize()
    
    print "Launch ..."
    t = timeit.Timer(technique.Compute)
    print "Fattal GPU : %.2f msec/pass" % (1000 * t.timeit(number=1)/1)
    
    values = numpy.zeros(technique.gridSize.x*technique.gridSize.y, dtype=numpy.float32)
    drv.memcpy_dtoh(values, technique.IBuffer)
    print values
    values = numpy.reshape(values, (technique.gridSize.x,technique.gridSize.y))
    try:
        import matplotlib.pyplot as plt
        print numpy.max(values)
        plt.imshow(numpy.clip(values,0,1.5))
        plt.show()
    except e:
        pass
#    for i in range(len(values)):
#        if values[i] > technique.offsetBuffers[-1][1]:
#            print "Overflow : " + str(i)

