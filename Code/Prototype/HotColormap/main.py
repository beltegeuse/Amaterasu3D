import pygame 
from pygame.locals import *
import os 
from math import *

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

if __name__ == '__main__':
    os.environ['SDL_VIDEO_CENTERED'] = '1' 
    pygame.init() 
    screen  = pygame.display.set_mode((256*3, 600))
    m = 256.0
    
    # set up key repeating so we can hold down the key to scroll.
    old_k_delay, old_k_interval = pygame.key.get_repeat()
    pygame.key.set_repeat(500, 30)
    
    while 1:
        screen.fill((0,0,0))
        #check for quit'n events
        event = pygame.event.poll()
        if event.type == QUIT or (event.type == KEYDOWN and event.key == K_ESCAPE):
            break
        
        for i in range(int(m)):
            sizex = int(800/m)
            x = sizex*i
            pygame.draw.rect(screen, HotColormap(i), (x,0,sizex, 600))
        
        pygame.display.flip()
        pygame.time.wait(10)
    pygame.key.set_repeat(old_k_delay, old_k_interval)
    pygame.quit()
