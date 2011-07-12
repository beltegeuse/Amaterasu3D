import pyopencl as cl
import numpy
import jinja2
from OpenGL.arrays import vbo

KERNEL_CODE = """
struct Sphere
{
    float3 position;
    float raySize;
    float3 color;
};

__kernel void sendrays(__constant struct Sphere* spheres, int numSpheres)
{
    for(int i = 0; i < numSpheres; i++)
    {
        // Nothings
    }
    return;
}
"""

####################
# Python class 
####################
class IRT:
    def __init__(self, size=(600,600)):
        self.ctx = cl.create_some_context()
        self.queue = cl.CommandQueue(self.ctx)
        self.prg = cl.Program(self.ctx, KERNEL_CODE)
    
    def Compute(self):
        pass
if __name__=="__main__":
    irt = IRT()
