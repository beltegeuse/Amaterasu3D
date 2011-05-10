import math


class Grid2D:
    '''This class describe the coarsed grid (With angles)'''
    def __init__(self, nbAngles = 1, dim = (256,256)):
        self.dimension = dim
        self.nbAngles = nbAngles
        self.data = []
        for i in range(dim[0]):
            temp = []
            for j in range(dim[1]):
                angles = []
                for k in range(nbAngles):
                    angles.append(0)
                temp.append(angles)
            self.data.append(temp)

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
    
class Ray:
    def __init__(self, pos, dir):
        self.position = pos
        self.direction = dir

class LPM:
    '''This class describe the Light propagation maps'''
    def __init__(self, nbAngles = 9, dim = 256):
        self.nbAngles = nbAngles
        self.dimension = dim
        
    def __GenerateRays(self, p1, p2, facesFactors):
        self.rays = []
        for i in range(self.dimension[0]):
                offset = 1.0 / self.nbAngles
                pos = offset/2
                for k in range(self.nbAngles): #Doesn't works...
                    self.rays.append(Ray(Vector2D(facesFactors.x*i, factors.y*i),p1.Factor(1 - pos) + p2.Factor(pos)))
    
    def GenerateRays(self, propagationOrientation):
        if(propagationOrientation.x == 0):
            pass
        elif(propagationOrientation.y == 0):
            pass
        else:
            raise Exception("Wrong orientation")
    
    def NumberRays(self):
        return self.nbAngles*self.dimension
        
def FattalAlgorithm(I, U, nbPass = 3):
    directions = [Vector2D(1,0),Vector2D(0,1), Vector2D(-1,0),Vector2D(0,-1)]
    lpm = LPM()
    
    for i in range(nbPass):
        for d in directions:
            if i == 0:
                pass # Border initialisation
            for rId in range(lpm.NumberRays()):
                pass # Compute the ray intersection with the grid
    
if __name__=="__main__":
    I = Grid2D()
    U = Grid2D(6)
    
    FattalAlgorithm(I,U)
    
    