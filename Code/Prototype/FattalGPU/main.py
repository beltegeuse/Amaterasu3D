import pyopencl as cl
import numpy
from OpenGL.arrays import vbo

KERNEL_CODE = """

"""

####################
# Python class 
####################
class IRT:
    def __init__(self, size=(600,600)):
        self.ctx = cl.create_some_context()
        self.queue = cl.CommandQueue(ctx)
    
if __name__=="__main__":
    irt = IRT()
