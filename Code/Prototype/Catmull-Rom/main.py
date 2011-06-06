import pygame 
from pygame.locals import *
import os 
from math import *
import sys
import numpy
import sys
import array
import math

class Points:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __add__(self, o):
        self.x += o.x
        self.y += o.y
    
    def __sub__(self, o):
        self.x += o.x
        self.y += o.y
    
    def __mult__(self, o):
        self.x *= o 
        self.y *= o 
    
    def __str__(self):
        return "< " + str(self.x) + " | " + str(self.y) + " >"
    
    def __repr__(self):
        return str(self)

class CatmullRomTrajectory:
    def __init__(self):
        self.controlPoints = []
    
    def addControlPoint(self, p):
        self.controlPoints.append(p)
    
    def ComputePosition(self, p0, p1, p2, p3, t):
        return 0.5*((2*p1) + (p2 - p0)*t + (2*p0 - 5*p1 + 4*p2 - p3)*t*t + (-p0 + 3*p1 - 3*p2 + p3)*t*t*t)
    
    def ComputePositionPoint(self, p1, p2, p3, p4, t):
        return Points(self.ComputePosition(p1.x, p2.x, p3.x, p4.x, t),self.ComputePosition(p1.y, p2.y, p3.y, p4.y, t))
    
    def Draw(self, screen, numberPoints = 10):
        if(len(self.controlPoints) < 2):
            return
        
        deltaT = 1.0/numberPoints
        
        l = [self.controlPoints[0]]
        l.extend(self.controlPoints)
        l.append(l[-1])
        
        p0 = l[0]

        for i in range(len(l) - 3):
            for t in range(numberPoints):
                p1 = self.ComputePositionPoint(l[i], l[i+1], l[i+2], l[i+3], t*deltaT)
                pygame.draw.aaline(screen, (255,255,255), (p0.x, p0.y), (p1.x, p1.y))
                p0 = p1
                
        for p in self.controlPoints:
            pygame.draw.circle(screen, (255,0,0), (p.x, p.y), 5)
            
if __name__ == '__main__':
    
    # T 
    T = CatmullRomTrajectory()
    T.addControlPoint(Points(10,10))
    T.addControlPoint(Points(100,100))
    T.addControlPoint(Points(200,100))
    T.addControlPoint(Points(200,200))
    
    # Initialisation de pygame
    os.environ['SDL_VIDEO_CENTERED'] = '1' 
    pygame.init() 
    screen  = pygame.display.set_mode((500, 500))
    
    # set up key repeating so we can hold down the key to scroll.
    old_k_delay, old_k_interval = pygame.key.get_repeat()
    pygame.key.set_repeat(500, 30)
    pxarray = pygame.PixelArray (screen)
    while 1:
        screen.fill((0,0,0))
        #check for quit'n events
        event = pygame.event.poll()
        if event.type == QUIT or (event.type == KEYDOWN and event.key == K_ESCAPE):
            break
        
        T.Draw(screen)
        
        pygame.display.flip()
        pygame.time.wait(10)
    pygame.key.set_repeat(old_k_delay, old_k_interval)
    pygame.quit()
