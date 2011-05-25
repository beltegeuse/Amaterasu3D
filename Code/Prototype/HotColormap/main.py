import pygame 
from pygame.locals import *
import os 
from math import *
import FreeImagePy
import sys
import numpy
import sys
import array
import OpenEXR
import Imath
import math

def HotColormap(i, m = 256.0):
    n = (3.0/8.0)*m;
    r = 0
    g = 0
    b = 0
    
    # Resolution Red
    if(i <= n):
        r = i / n;
    else:
        r = 1.0
        
    # Resolution Green
    if(i <= n):
        g = 0.0
    elif(i <= 2*n):
        g = (i-n)/n
    else:
        g = 1.0
    
    # Resolution Blue
    if(i <= 2*n):
        b = 0.0
    else:
        b = (i-2*n)/(m-2*n)
    
    return (int(r*255),int(g*255),int(b*255))

def minC(v1,v2):
    if v1 > v2:
        return v2
    return v1

def maxC(v1,v2):
    if v1 < v2:
        return v2
    return v1

def toColor(v, max):
    return (int(v[0]*255 / max[0]),int(v[1]*255 / max[1]),int(v[2]*255 / max[2]))

if __name__ == '__main__':
    if(len(sys.argv) < 2):
        print "Error no images"
        sys.exit(-1)
    
    # On recupere l'image
    imgFile = sys.argv[1]
    
    # Open the input file
    file = OpenEXR.InputFile(sys.argv[1])
    
    # Compute the size
    dw = file.header()['dataWindow']
    (width,height) = (dw.max.x - dw.min.x + 1, dw.max.y - dw.min.y + 1)
    
    # Read the three color channels as 32-bit floats
    FLOAT = Imath.PixelType(Imath.PixelType.FLOAT)
    (R,G,B) = [array.array('f', file.channel(Chan, FLOAT)).tolist() for Chan in ("R", "G", "B") ]
    
    print "Image size : "+str((width, height))
    
    imageData = []
    for j in range(height):
        for i in range(width):
            indice = j*width+i
            imageData.append((R[indice],G[indice],B[indice]))
    
    print "First pixel : "+str(imageData[0])
    
    maxV = max(imageData)
    
    print "Max pixel : "+str(maxV)
    
    
    # Initialisation de pygame
    os.environ['SDL_VIDEO_CENTERED'] = '1' 
    pygame.init() 
    screen  = pygame.display.set_mode((width, height))
    m = 256.0
    
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
        
        print "Write image ..."
        for j in range(height):
            for i in range(width):
#                Y = imageData[j*width+i][0]*0.17697+imageData[j*width+i][1]*0.81240+imageData[j*width+i][2]*0.01063
#                v = maxC(minC(math.log10(Y),5.0),0.0)*50
#                pxarray[i][j] = HotColormap(v)
                 pxarray[i][j] = toColor(imageData[j*width+i], maxV)
        print "End Write image"
        
        pygame.display.flip()
        pygame.time.wait(10)
    pygame.key.set_repeat(old_k_delay, old_k_interval)
    pygame.quit()
