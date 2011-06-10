import math
T = 0.1

class Register:
    def __init__(self, initialValue = 1.0):
        self.In = initialValue
        self.Out = initialValue
        
    def Update(self, inputValue):
        self.Out = self.In
        self.In = inputValue

class JRegister(Register):
    def __init__(self, initValue = 1.0):
        Register.__init__(self, initValue)

    def Update(self, inputValue):
        Register.Update(self, inputValue)
        self.R = T*(self.Out/16.0)*self.In

class KRegister(Register):
    def __init__(self, initValue = 1.0):
        Register.__init__(self, initValue)

    def Update(self, inputValue):
        Register.Update(self, inputValue)
        self.R = T*(1-self.Out)/400.0

class ExponentialFiltering:
    def __init__(self, T, initV):
        self.T = T
        self.Out = initV
    
    def Update(self,In,deltaT):
        alpha = deltaT / (deltaT + T)
        self.Out = alpha*In + (1.0-alpha)*self.Out
    
#Trod = 150.0
Trod = 40.0
Vin = 24.0

#After 4 after lifes
T = Trod/math.log(16)
filter = ExponentialFiltering(T,0.0)
print "Alpha : "+str(1.0/(1.0+T))
for i in range(50):
    filter.Update(Vin, 1)
    print str(i)+" : "+str(filter.Out)
    #print str(filter.Out)