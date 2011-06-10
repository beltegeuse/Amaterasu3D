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

#After 4 after lifes
#T = Trod/math.log(16)
#filter = ExponentialFiltering(T,0.0)
#print "Alpha : "+str(1.0/(1.0+T))
#for i in range(50):
#    filter.Update(Vin, 1)
#    print str(i)+" : "+str(filter.Out)
#    #print str(filter.Out)
    
print("Other simulation ...")
Vin = 2000000
Vout = 0.01
R = Vout
vT = 1.0
#for i in range(200):
#    Vout = R
#    Gout = 0.04/(0.04+Vout)
#    J = vT*(Gout/16.0)*Vin
#    K = vT*(1-Gout)/110
#    print "Arod : " + str(Vout) + " | Brod : "+ str(Gout)
#    R = (J-K)+R

R = 2.0*math.pow(10,6) / (2.0*math.pow(10,6) + 0.01)
print "R : "+str(R)
for i in range(200):
    Vout = R
    J = vT*(Vout/(2.2*math.pow(10, 8)))*Vin
    K = vT*(1-Vout)/11000.0
    print "Bcone : "+ str(Vout)
    R = (K-J)+R
    
#    Aout = (200000 / Vout) - 200000
#    k = 1.0/(5.0*Aout + 1.0)
#    sigCone = (12.9223*Aout)/(math.pow(k, 4)*Aout + 0.171*math.pow(1-math.pow(k, 4),2)*math.pow(Aout, 1.0/3.0))
#    print sigCone