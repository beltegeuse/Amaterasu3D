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
    def __init__(self, screen, nbAngles, i = 8, j = 6, screenRes = Vector2D(800,600)):
        self.screen = screen
        self.dimension = screenRes
        self.nbAngles = nbAngles
        self.cellDimension = Vector2D(self.dimension.x / i, self.dimension.y / j)
        self.NbCells = Vector2D(i,j)
        self.color = pygame.Color("White")
        
        # Intialisation angles data
        self.data = []
        for n in range(i*j):
            t = []
            for k in range(nbAngles):
                t.append(0)
            self.data.append(t)
    
    def Copy(self):
        grid = Grid(self.screen, self.nbAngles, self.NbCells.x, self.NbCells.y, self.dimension)
        for n in range(self.NbCells.x*self.NbCells.y):
            for k in range(self.nbAngles):
                grid.data[n][k] = self.data[n][k]
        return grid
    
    def Draw(self):
        for i in range(self.NbCells.x):
            pygame.draw.line(self.screen,self.color,(self.cellDimension.x*i,0),(self.cellDimension.x*i,self.dimension.y))
        for i in range(self.NbCells.y):
            pygame.draw.line(self.screen,self.color,(0,self.cellDimension.y*i),(self.dimension.x,self.cellDimension.y*i))

    def DrawData(self):
        maxV = max(self.data)[0]
        for i in range(self.NbCells.x):
            for j in range(self.NbCells.y):
                color = 255.0*(self.data[self.GetVoxelID(i,j)][0] / maxV)
                pygame.draw.rect(self.screen,(color,color,color),(i*self.cellDimension.x, j*self.cellDimension.y, self.cellDimension.x, self.cellDimension.y))

    def World2Voxels(self, v):
        return Vector2D(int(v.x / self.cellDimension.x), int(v.y / self.cellDimension.y))
    
    def GetVoxelID(self, x, y):
        return x+y*self.NbCells.x
    
    def World2VoxelsID(self, v):
        v = self.World2Voxels(v)
        return self.GetVoxelID(v.x, v.y)
    
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
    
    def ComputeRayIntersectionsLoop(self, ray):
        """Give an list of list of intersection points"""
        MainDirection = abs(ray.direction.x) < abs(ray.direction.y)
        
        allIntersection = []
        
        needRecast = True
        while needRecast:
            # Compute intersections
            intersections = self.ComputeRayIntersections(ray.Copy())[0]
            #print "Intersections : "+str(intersections)
            
            # If no intersection : break
            if(len(intersections) == 0):
                break
            
            # Add intersection
            temp = []
            if len(allIntersection) == 0:
                temp.append(ray.position)
            temp.extend(intersections)
            allIntersection.append(temp)
            
            # Change the ray position for the looping system
            lastIntersection = intersections[-1]
            ray.position = intersections[-1].Copy()
            if self.__floatTestingEqual(0, lastIntersection.x) and self.__floatTestingEqual(0, lastIntersection.y):
                break
            elif self.__floatTestingEqual(self.dimension.x, lastIntersection.x) and self.__floatTestingEqual(self.dimension.y, lastIntersection.y):
                break
            
            if(MainDirection):
                if(self.__floatTestingEqual(0, lastIntersection.x)):
                    ray.position.x = self.dimension.x
                elif(self.__floatTestingEqual(self.dimension.x, lastIntersection.x)):
                    ray.position.x = 0
                else:
                    needRecast = False
            else:
                if(self.__floatTestingEqual(0, lastIntersection.y)):
                    ray.position.y = self.dimension.y
                elif(self.__floatTestingEqual(self.dimension.y, lastIntersection.y)):
                    ray.position.y = 0
                else:
                    needRecast = False
        #print allIntersection
        return allIntersection
    
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
        allIntersection = self.ComputeRayIntersectionsLoop(ray)
        for intersections in allIntersection:
            # Draw all intersection points
            for inter in intersections:
                pygame.draw.circle(self.screen, (255,0,0), (int(inter.x), int(inter.y)), 3)
            # Draw the ray
            ray.position = intersections[0]
            ray.Draw(self.screen)
            

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

def GetMainDirectionVector(mainDir, value):
    if mainDir == "X":
        return Vector2D(value, 0)
    elif mainDir == "Y":
        return Vector2D(0, value)
    else:
        raise Exception("No found good main direction")

def NormalizeCoordinatesVector(v):
    t = (v + Vector2D(1,1)).Factor(0.5)
    return Vector2D(int(t.x), int(t.y))

def FattalAlgorithm(I, U, screen, nbPass = 3, visualisation = True):
    directions = [("X", 1, (0,255,0)),("X", -1, (255,0,255)),("Y", 1, (0,0,255)),("Y", -1, (255,255,0))]
    #directions = [("Y", -1, (255,255,0))]
    lpm = LPM(I)
    
    cellVolume = I.cellDimension.x*I.cellDimension.y
    
    for i in range(nbPass):
        for (mainAxis, propaOri, color) in directions:
            lpm.GenerateRays(mainAxis, propaOri)
            # Compute main directions
            mainDirectionV = GetMainDirectionVector(mainAxis, propaOri)
            normalizeDirectionV = NormalizeCoordinatesVector(mainDirectionV)
            Ars = abs(Vector2D.DotProduct(mainDirectionV, I.cellDimension))
            # Copy U gird 
            UTilde = U.Copy()
            idDir = normalizeDirectionV.x + normalizeDirectionV.y*2
            # Empty U direction because we take these energy
            for iData in range(U.NbCells.x*U.NbCells.y):
                U.data[iData][idDir] = 0
            if i == 0:
                pass #TODO: Border initialisation
            for rId in range(lpm.NumberRays()):
                ray = lpm.rays[rId]
                rayValue = 0.0
                if i == 0:
                    rayValue = 1.0
                if visualisation:
                    ray.color = color
                    ray.Draw(screen)
                    I.DrawLoopRayIntersection(ray)
                # Get all intersections (group of group of intersection)
                Allintersections = I.ComputeRayIntersectionsLoop(ray)
                for intersections in Allintersections:
                    for i in range(len(intersections)-1):
                        beginP = intersections[i]
                        endP = intersections[i+1]
                        v = endP - beginP
                        dist = v.Length()
                        if dist == 0:
                            continue
                        # Know the CellID
                        voxID = I.World2VoxelsID(beginP+v.Factor(0.5))
                        #print beginP
                        #print endP
                        #print voxID
                        # Precompute values for update U and I
                        Wn = Vector2D.DotProduct(mainDirectionV, ray.direction)
                        scatteringTerm = rayValue*(1 - exp(-1*dist*I.S/Wn))
                        # Update ray value
                        absortionCoeff = I.K+I.S
                        absortionFactor = exp(-1*dist*absortionCoeff/Wn)
                        UTildeValue = UTilde.data[voxID][idDir]
                        rayValue = rayValue*absortionFactor+(UTildeValue*(1 - absortionFactor)/absortionCoeff)
                        # I value
                        # Only one direction so don't need to iterate
                        I.data[voxID][0] += Ars*(1.0/(9))*scatteringTerm
                        # Update U value
                        for mID in range(4):
                            U.data[voxID][mID] += Ars*(1.0/(9*4))*scatteringTerm
if __name__=="__main__":
    # Constante
    # Real value for first compute
    RES = Vector2D(512,512)
    NBCELL = Vector2D(16,16)
    VIS = False
    K = 1
    S = 1
    # Test value for drawing
    #RES = Vector2D(800,600)
    #NBCELL = Vector2D(8,6)
    #VIS = True
    
    # Pygame initialisation
    os.environ['SDL_VIDEO_CENTERED'] = '1' 
    pygame.init() 
    screen  = pygame.display.set_mode((RES.x, RES.y))

    I = Grid(screen,1, NBCELL.x, NBCELL.y, RES)
    # Intialise factors
    I.K = K
    I.S = S
    
    # Only 4 angles because we are in 2D problem
    U = Grid(screen,4, NBCELL.x, NBCELL.y, RES)
    
    print "Begin Fattal resolution"
    FattalAlgorithm(I,U, screen, 3, VIS)
    print "End Fattal resolution"
    print max(I.data)
    
    while 1:
        screen.fill((0,0,0))
        #check for quit'n events
        event = pygame.event.poll()
        if event.type == QUIT or (event.type == KEYDOWN and event.key == K_ESCAPE):
            break
        I.DrawData()
        
        pygame.display.flip()
        pygame.time.wait(10)
    