import math
import pygame 
from pygame.locals import *
import os 
from math import *

def sign(t):
    if t < 0:
        return -1
    return 1

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

class Ray:
    def __init__(self, pos, dir):
        self.position = pos
        self.direction = dir
        self.direction.Normalize()
        
        # Graphic attribut
        self.color = (0,255,0)
        
    def NewPosition(self, t):
        return Vector2D(self.position.x + self.direction.x * t,self.position.y + self.direction.y * t)

    def Copy(self):
        return Ray(self.position.Copy(), self.direction.Copy())

    def __str__(self):
        return "( "+str(self.position)+", "+str(self.direction)+" )"
    
    def Draw(self, screen):
        EndPoint = self.position + self.direction.Factor(1000)
        pygame.draw.circle(screen, self.color, (int(self.position.x), int(self.position.y)), 3)
        pygame.draw.aaline(screen, self.color, (int(self.position.x), int(self.position.y)), (int(EndPoint.x), int(EndPoint.y)))

class Grid:
    def __init__(self, screen, nbAngles, i = 8, j = 6):
        self.screen = screen
        self.dimension = Vector2D(800,600)
        self.cellDimension = Vector2D(800 / i, 600 / j)
        self.NbCells = Vector2D(i,j)
        self.color = pygame.Color("White")
        
    def Draw(self):
        for i in range(self.NbCells.x):
            pygame.draw.line(self.screen,self.color,(self.cellDimension.x*i,0),(self.cellDimension.x*i,600))
        for i in range(self.NbCells.y):
            pygame.draw.line(self.screen,self.color,(0,self.cellDimension.y*i),(800,self.cellDimension.y*i))

    def World2Voxels(self, v):
        return Vector2D(int(v.x / self.cellDimension.x), int(v.y / self.cellDimension.y))
    
    def PointIsInGrid(self, v):
        #TODO: Precision issue
        return v.x >= -1 and v.x <= (self.cellDimension.x*self.NbCells.x + 1) and v.y >= -1 and v.y <= (self.cellDimension.y*self.NbCells.y + 1)

    def ComputeRayIntersections(self, ray):
        intersections = []
        cellIntersection = []
        voxID = self.World2Voxels(ray.position)
        voxWorldPos = Vector2D.Mult(voxID, self.cellDimension)
        
        tMax = Vector2D()
        
        if(ray.direction.x < 0):
            tMax.x = (voxWorldPos.x - ray.position.x) / ray.direction.x
        elif(ray.direction.x > 0):
            tMax.x = (voxWorldPos.x + self.cellDimension.x - ray.position.x) / ray.direction.x
        else:
            tMax.x = 10000000;
        
        if(ray.direction.y < 0):
            tMax.y = (voxWorldPos.y - ray.position.y) / ray.direction.y
        elif(ray.direction.y > 0):
            tMax.y = (voxWorldPos.y + self.cellDimension.y - ray.position.y) / ray.direction.y
        else:
            tMax.y = 10000000
        
        # Determine the main direction
        tDelta = Vector2D()
        if(ray.direction.x != 0):
            tDelta.x = self.cellDimension.x*1.0/abs(ray.direction.x)
        else:
            tDelta.x = 10000000
        if(ray.direction.y != 0):
            tDelta.y = self.cellDimension.y*1.0/abs(ray.direction.y)
        else:
            tDelta.y = 10000000
        sDelta = ray.direction.Sign()
        cellIntersection.append(voxID.Copy())
        
        pos = ray.position.Copy()
        while True:
            if(tMax.x < tMax.y):
                pos = ray.NewPosition(tMax.x)
                tMax.x += tDelta.x
                voxID.x += sDelta.x
            else:
                pos = ray.NewPosition(tMax.y)
                tMax.y += tDelta.y
                voxID.y += sDelta.y
            
            if(not self.PointIsInGrid(pos)):
                break
            
            cellIntersection.append(voxID.Copy())
            intersections.append(pos)  
            
        return (intersections, cellIntersection)
    
    def DrawCellIntersection(self, voxID):
        p = Vector2D.Mult(voxID, self.cellDimension)
        pygame.draw.rect(self.screen, (0,0,255), (p.x, p.y, self.cellDimension.x, self.cellDimension.y))
    
    def DrawRayIntersection(self, ray):
        intersections, cellIntersection = self.ComputeRayIntersections(ray)
        #print "==========="
        for voxID in cellIntersection:
            #print voxID
            #self.DrawCellIntersection(voxID)
            pass
        
        for inter in intersections:
            pygame.draw.circle(self.screen, (255,0,0), (int(inter.x), int(inter.y)), 3)

        # If there isn't any intersection
        v = None
        if(len(intersections) != 0):
            v = intersections[-1]
            print v

        return v
    
    def __floatTestingEqual(self, f1, f2):
        return abs(f1 - f2) < 0.00001
    
    def DrawLoopRayIntersection(self, ray):
        MainDirection = abs(ray.direction.x) < abs(ray.direction.y)
        
        needRecast = True
        while needRecast:
            lastIntersection = self.DrawRayIntersection(ray)
            if lastIntersection == None:
                break
            
            ray.Draw(self.screen)
            ray.position = lastIntersection
            
            if self.__floatTestingEqual(0, lastIntersection.x) and self.__floatTestingEqual(0, lastIntersection.y):
                break
            elif self.__floatTestingEqual(800, lastIntersection.x) and self.__floatTestingEqual(600, lastIntersection.y):
                break
            
            if(MainDirection):
                if(self.__floatTestingEqual(0, lastIntersection.x)):
                    ray.position.x = 800
                elif(self.__floatTestingEqual(800, lastIntersection.x)):
                    ray.position.x = 0
                else:
                    needRecast = False
            else:
                if(self.__floatTestingEqual(0, lastIntersection.y)):
                    ray.position.y = 600
                elif(self.__floatTestingEqual(600, lastIntersection.y)):
                    ray.position.y = 0
                else:
                    needRecast = False   

class LPM:
    '''This class describe the Light propagation maps'''
    def __init__(self, grid, nbAngles = 9):
        self.nbAngles = nbAngles
        self.grid = grid
    
    def __GenerateFaceSampling(self):
        samples = []
        offset = 1.0 / (self.nbAngles+1)
        for i in range(self.nbAngles):
            v = Vector2D(0.5, (i+1)*offset - 0.5)
            samples.append(v)
        return samples
    
    def GenerateRays(self, mainAxis, propagationOrientation):
        # Setup startup position
        startupPosition = Vector2D(0,0)
        if(propagationOrientation == -1):
            startupPosition = self.grid.dimension.Copy()
        
        # Setup transformation matrice
        transformationMatrix = Matrix2D(0,0,0,0)
        self.dimension = 0
        if(mainAxis == "X"):
            transformationMatrix.a11 = propagationOrientation
            transformationMatrix.a22 = propagationOrientation
            self.dimension = self.grid.NbCells.y
        elif(mainAxis == "Y"):
            transformationMatrix.a21 = propagationOrientation
            transformationMatrix.a12 = propagationOrientation
            self.dimension = self.grid.NbCells.x
        else:
            raise Exception("Impossible to find main axis")
        
        # Erase old rays
        self.rays = []
        
        # Generate new one
        samples = self.__GenerateFaceSampling()
        transVector = transformationMatrix.Transform(Vector2D(0,1))
        offset = Vector2D.Mult(transVector, self.grid.cellDimension)
        #cellOffset = Vector2D.Mult(transVector,self.grid.cellDimension.Factor(0.5))
        for i in range(self.dimension):
            for j in range(self.nbAngles):
                r = Ray(startupPosition+offset.Factor(i+0.5), transformationMatrix.Transform(samples[j]))
                self.rays.append(r)
        
    def NumberRays(self):
        return self.nbAngles*self.dimension
        
def FattalAlgorithm(I, U, screen, nbPass = 3):
    directions = [("X", 1, (0,255,0)),("X", -1, (255,0,255)),("Y", 1, (0,0,255)),("Y", -1, (255,255,0))]
    #directions = [("Y", -1, (255,255,0))]
    lpm = LPM(I)
    
    for i in range(nbPass):
        for (mainAxis, propaOri, color) in directions:
            lpm.GenerateRays(mainAxis, propaOri)
            if i == 0:
                pass # Border initialisation
            for rId in range(lpm.NumberRays()):
                ray = lpm.rays[rId]
                ray.color = color
                ray.Draw(screen)
                I.DrawLoopRayIntersection(ray)
    
if __name__=="__main__":
    # Pygame initialisation
    os.environ['SDL_VIDEO_CENTERED'] = '1' 
    pygame.init() 
    screen  = pygame.display.set_mode((800, 600))

    I = Grid(screen,1)
    U = Grid(screen,6)
    
    while 1:
        screen.fill((0,0,0))
        #check for quit'n events
        event = pygame.event.poll()
        if event.type == QUIT or (event.type == KEYDOWN and event.key == K_ESCAPE):
            break
        FattalAlgorithm(I,U, screen, 1)
        I.Draw()
        
        pygame.display.flip()
        pygame.time.wait(10)
    