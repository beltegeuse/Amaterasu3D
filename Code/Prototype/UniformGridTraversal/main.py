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
    
class Ray:
    def __init__(self, pos, dir):
        self.position = pos
        self.direction = dir
        self.direction.Normalize()
        
    def NewPosition(self, t):
        return Vector2D(self.position.x + self.direction.x * t,self.position.y + self.direction.y * t)

    def Copy(self):
        return Ray(self.position.Copy(), self.direction.Copy())

    def __str__(self):
        return "( "+str(self.position)+", "+str(self.direction)+" )"
    
    def Draw(self, screen):
        EndPoint = self.position + self.direction.Factor(1000)
        pygame.draw.circle(screen, (0,255,0), (self.position.x, self.position.y), 3)
        pygame.draw.aaline(screen, (0,255,0), (self.position.x, self.position.y), (int(EndPoint.x), int(EndPoint.y)))

class Gird:
    def __init__(self, screen, i = 10, j = 10):
        self.screen = screen
        self.cellDimension = Vector2D(800 / i, 600 / j)
        self.NbCells = Vector2D(i,j)
        self.color = pygame.Color("White")
        
    def draw(self):
        for i in range(self.NbCells.x):
            pygame.draw.line(self.screen,self.color,(self.cellDimension.x*i,0),(self.cellDimension.x*i,600))
        for i in range(self.NbCells.y):
            pygame.draw.line(self.screen,self.color,(0,self.cellDimension.y*i),(800,self.cellDimension.y*i))

    def World2Voxels(self, v):
        return Vector2D(int(v.x / self.cellDimension.x), int(v.y / self.cellDimension.y))
    
    def PointIsInGrid(self, v):
        return v.x >= 0 and v.x < self.cellDimension.x*self.NbCells.x and v.y >= 0 and v.y < self.cellDimension.y*self.NbCells.y

    def ComputeRayIntersections(self, ray):
        intersections = []
        cellIntersection = []
        voxID = self.World2Voxels(ray.position)
        voxWorldPos = Vector2D.Mult(voxID, self.cellDimension)
        
        tMax = Vector2D()
        
        #TODO: Somes times direction component can be equal to 0
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
            self.DrawCellIntersection(voxID)

        for inter in intersections:
            pygame.draw.circle(self.screen, (255,0,0), (int(inter.x), int(inter.y)), 3)

        print intersections[-1]

        # Loop texture coordinates
#        if abs(ray.direction.x) > abs(ray.direction.y):
#            print "Main direction X"
#            print intersections[-1]
#            # Main direction is X
#            if(ray.direction.y > 0 and intersections[-1].y > self.NbCells.y*self.cellDimension.y):
#                newPos = intersections[-1]
#                newPos.y = 0
#                ray.position = newPos
#                self.DrawRayIntersection(ray)
#                ray.Draw(self.screen)
#            elif(ray.direction.y < 0 and intersections[-1].x < 0):
#                newPos = intersections[-1]
#                newPos.y = self.NbCells.y*self.cellDimension.y
#                ray.position = newPos
#                self.DrawRayIntersection(ray)
#                ray.Draw(self.screen)
#        else:
#            print "Main direction Y"

if __name__ == '__main__':
    os.environ['SDL_VIDEO_CENTERED'] = '1' 
    pygame.init() 
    screen  = pygame.display.set_mode((800, 600))
    grille = Gird(screen)
    ray = Ray(Vector2D(10,10), Vector2D(1,0))
    vitesse = 2
    
    # set up key repeating so we can hold down the key to scroll.
    old_k_delay, old_k_interval = pygame.key.get_repeat()
    pygame.key.set_repeat(500, 30)
    
    clicked = False
    oldPosition = Vector2D()
    
    while 1:
        screen.fill((0,0,0))
        #check for quit'n events
        event = pygame.event.poll()
        if event.type == QUIT or (event.type == KEYDOWN and event.key == K_ESCAPE):
            break
        if event.type == MOUSEBUTTONDOWN:
            if not clicked:
                oldPosition = Vector2D(event.pos[0],event.pos[1])
            else:
                ray = Ray(oldPosition, Vector2D(event.pos[0], event.pos[1]) - oldPosition)
            clicked = not clicked
     
        grille.DrawRayIntersection(ray.Copy())
        grille.draw()
        ray.Draw(screen)
        pygame.display.flip()
        pygame.time.wait(10)
    pygame.key.set_repeat(old_k_delay, old_k_interval)
    pygame.quit()
