VT = 0.1
Trod = 150.0
Vout = 0.0
Vin = 25.0
R = 0.0

#for i in range(150):
#    Vout = R
#    J = VT*(Vout/16.0)*Vin
#    K = VT*(1-R)/Trod
#    R = R+(J-K)
#    print Vout

for i in range(50):
    R = (1 - 0.1)*R + 0.1*Vin
    print R
for i in range(50):
    R = (1 - 0.1)*R + 0.1*0.0
    print R