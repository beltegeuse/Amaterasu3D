import pygame 
from pygame.locals import *
import os 
from math import *

class Gird:
    def __init__(self, screen, i = 20, j = 20):
        self.screen = screen
        self.dimension = (i, j)
        self.color = pygame.Color("White")
        
    def draw(self):
        for i in range(800 / self.dimension[0]):
            pygame.draw.line(self.screen,self.color,(self.dimension[0]*i,0),(self.dimension[0]*i,600))
        for i in range(600 / self.dimension[1]):
            pygame.draw.line(self.screen,self.color,(0,self.dimension[1]*i),(800,self.dimension[1]*i))

class Camera:
    def __init__(self, pos = (300,300)):
        self.position = pos
        self.color = pygame.Color("Blue")
        self.angle = 0
   
    def move(self, offset):
        self.position = (self.position[0]+cos(self.angle)*2,self.position[1]+sin(self.angle)*2)
    
    def moveAngle(self, offset):
        self.angle += offset
        if(self.angle < 0):
            self.angle += 2.0*pi
        self.angle = fmod(self.angle,2.0*pi)
        print("Angle : " + str(self.angle))
    
    def draw(self, screen):
        direction = (cos(self.angle), sin(self.angle))
        
        pygame.draw.circle(screen, self.color, (int(self.position[0]), int(self.position[1])), 5)
        pygame.draw.aaline(screen, self.color, self.position, (self.position[0] + direction[0]*20, self.position[1] + direction[1]*20))
        

class Volume:
    def __init__(self, camera, i = 20, j = 20, ):
        self.camera = camera
        self.cellsize = 20
        self.nbCell = 16
        self.color = pygame.Color("Red")
    
    def offsetAngle(self):
        angleNormalisation = sqrt(2)
        sift = (self.nbCell/4)*self.cellsize
        if(self.camera.angle >= ((7.0*pi) / 4.0) or self.camera.angle <= (pi / 4.0)):
            self.position = (self.position[0], self.position[1] + sift*(sin(self.camera.angle))*angleNormalisation)
        elif(self.camera.angle > (pi / 4.0) and self.camera.angle <= ((3.0*pi) / 4.0)):
            self.position = (self.position[0] + sift*((cos(self.camera.angle)*angleNormalisation) - 1), self.position[1]+sift)
        elif(self.camera.angle > ((3.0*pi) / 4.0) and self.camera.angle <= ((5.0*pi) / 4.0)):
            self.position = (self.position[0]-sift*2, self.position[1] + sift*(sin(self.camera.angle))*angleNormalisation)
        else:
            self.position = (self.position[0] + (self.nbCell/4)*self.cellsize*((cos(self.camera.angle)*angleNormalisation) - 1), self.position[1]-(self.nbCell/4)*self.cellsize)
    
    def offsetAngle2(self):
        #invert vector
        x = -cos(self.camera.angle)
        y = -sin(self.camera.angle)
        
        a = 1.0 / sqrt(2)
        
        x = max(min(a,x),-a) / a
        y = max(min(a,y),-a) / a
        factor = (self.nbCell/4)*self.cellsize
        self.position = (self.position[0] - (x+1)*factor, self.position[1] - y*factor)
    
    def updatePosition(self):
        # Border offset
        self.position = (self.camera.position[0] - (self.nbCell/4)*self.cellsize, self.camera.position[1] - (self.nbCell/2)*self.cellsize)
        self.offsetAngle2()
        # Snapping
        self.position = (floor(self.position[0] / self.cellsize) *self.cellsize , floor(self.position[1] / self.cellsize) *self.cellsize)
    
    def drawRect(self, pos, pos2, color):
        pygame.draw.line(screen,color,pos,(pos2[0],pos[1]), 2)
        pygame.draw.line(screen,color,pos,(pos[0],pos2[1]), 2)
        pygame.draw.line(screen,color,(pos2[0], pos[1]),pos2, 2)
        pygame.draw.line(screen,color,(pos[0], pos2[1]),pos2, 2)
    
    def draw(self, screen):
        self.updatePosition()
        offset = (self.position[0]+self.cellsize*self.nbCell,self.position[1]+self.cellsize*self.nbCell)
        self.drawRect(self.position, offset, self.color)
        innerPos = (self.position[0] + (self.nbCell/4)*self.cellsize, self.position[1] + (self.nbCell/4)*self.cellsize)
        innerPos2 = (offset[0] - (self.nbCell/4)*self.cellsize, offset[1] - (self.nbCell/4)*self.cellsize)
        self.drawRect(innerPos, innerPos2, (0,255,0))

if __name__ == '__main__':
    os.environ['SDL_VIDEO_CENTERED'] = '1' 
    pygame.init() 
    screen  = pygame.display.set_mode((800, 600))
    grille = Gird(screen)
    cam = Camera()
    vol = Volume(cam)
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
        elif event.type == KEYDOWN:
            if event.key == K_UP:
                cam.move((0,-vitesse))
            elif event.key == K_DOWN:
                cam.move((0,vitesse))
            elif event.key == K_RIGHT:
                cam.move((vitesse,0))
            elif event.key == K_LEFT:
                cam.move((-vitesse,0))
            elif event.key == K_a:
                cam.moveAngle(0.1)
            elif event.key == K_d:
               cam.moveAngle(-0.1)
        
        grille.draw()
        cam.draw(screen)
        vol.draw(screen)
        pygame.display.flip()
        pygame.time.wait(10)
    pygame.key.set_repeat(old_k_delay, old_k_interval)
    pygame.quit()
