import pygame 
from pygame.locals import *
import os 
from math import *

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

    def ComputeRayIntersections(self, ray):
        intersections = []
        return intersections
    
    def DrawRayIntersection(self, ray):
        intersections = self.ComputeRayIntersections(ray)
        for inter in intersections:
            pygame.draw.circle(self.screen, (255,0,0), (int(inter.x), int(inter.y)), 3)

if __name__ == '__main__':
    os.environ['SDL_VIDEO_CENTERED'] = '1' 
    pygame.init() 
    screen  = pygame.display.set_mode((800, 600))
    grille = Gird(screen)
    ray = Ray(Vector2D(10,10), Vector2D(1,1))
    vitesse = 2
    
    # set up key repeating so we can hold down the key to scroll.
    old_k_delay, old_k_interval = pygame.key.get_repeat()
    pygame.key.set_repeat(500, 30)
    
    while 1:
        screen.fill((0,0,0))
        #check for quit'n events
        event = pygame.event.poll()
        if event.type == QUIT or (event.type == KEYDOWN and event.key == K_ESCAPE):
            break
     
        grille.draw()
        ray.Draw(screen)
        pygame.display.flip()
        pygame.time.wait(10)
    pygame.key.set_repeat(old_k_delay, old_k_interval)
    pygame.quit()
