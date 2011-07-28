import math

class GridCompute:
    def __init__(self, nbElements, screenSize):
        self.nbElements = nbElements
        
        # Compute width and height
        root = math.sqrt(nbElements)
        self.sHeight = int(round(root))
        self.sWidth = int(self.sHeight)
        if(root - self.sHeight > 0.0):
            self.sWidth+=1
        
        print("[DEBUG] dict "+str(self.__dict__))
        
        # Compute size
        self.cellSize = (screenSize[0]/self.sHeight, screenSize[1]/self.sWidth)
        
        self.cells = []
        for i in range(self.sHeight):
            for j in range(self.sWidth):
                self.cells.append((i*self.cellSize[0],j*self.cellSize[1]))
        
if __name__=="__main__":
    for i in range(10):
        print("NbElements : " + str(i+1))
        grid = GridCompute(i+1, (800,600))
        print(str(grid.cells))