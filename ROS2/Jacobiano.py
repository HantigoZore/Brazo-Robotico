import numpy as np
from numpy import sin as s, cos as c

theta_0=0
theta_1=0
theta_2=0
theta_3=0
theta_4=0

def Jacobiano(t0, t1, t2, t3, t4):
    
    a1=0.0088*c(t1+t2-t3-t4)
    a2=0.0088*c(t1+t2-t3+t4)
    a3=0.25*c(t1+t2)
    a4=s(t1+t2-t3)
    a6=0.0088*s(t1+t2-t3-t4)
    a7=0.0088*s(t1+t2-t3+t4)
    a8=0.25*s(t1+t2)
    a5=a7+a6+a8-0.25*c(t1)
    
    R11=0.0222*c(t0)-0.0176*c(t0)*s(t4)-0.24*s(t0)*s(t1)+0.24*s(t0)*s(t1)*s(t2)-0.24*c(t1)*c(t2)*s(t0)-0.0176*c(t1)*c(t2)*c(t3)*c(t4)*s(t0)-0.0176*c(t1)*c(t4)*s(t0)*s(t2)*s(t3)-0.0176*c(t2)*c(t4)*s(t0)*s(t1)*s(t3)+0.0176*c(t3)*c(t4)*s(t0)*s(t1)*s(t2)
    R12=-c(t0)*a5
    R13=-c(t0)*(a7+a6+a8)
    R14=0.0176*c(t0)*c(t4)*a4
    R15=0.0176*c(t0)*c(t3)*s(t1)*s(t2)*s(t4)-0.0176*c(t0)*c(t1)*c(t2)*c(t3)*s(t4)-0.0176*c(t0)*c(t1)*s(t2)*s(t3)*s(t4)-0.0176*c(t0)*c(t2)*s(t1)*s(t3)*s(t4)-0.0176*c(t4)*s(t0)
   
    R21=0.0222*s(t0)+0.25*c(t0)*s(t1)-0.0176*s(t0)*s(t4)-0.25*c(t0)*s(t1)*s(t2)+0.25*c(t0)*c(t1)*c(t2)+0.0176*c(t1)*c(t2)*c(t3)*c(t4)+0.0176*c(t0)*c(t1)*c(t4)*s(t2)*s(t3)+0.0176*c(t0)*c(t2)*c(t4)*s(t1)*s(t3)-0.0176*c(t0)*c(t3)*c(t4)*s(t1)*s(t2)
    R22=-s(t0)*a5
    R23=-s(t0)*(a7+a6+a8)
    R24=0.0176*c(t4)*s(t0)*a4
    R25=0.0176*c(t0)*c(t4)-0.0176*c(t1)*c(t2)*c(t3)*s(t0)*s(t4)-0.0176*c(t1)*s(t0)*s(t2)*s(t3)*s(t4)-0.0176*c(t2)*s(t0)*s(t1)*s(t3)*s(t4)+0.0176*c(t3)*s(t0)*s(t1)*s(t2)*s(t4)
    
    R31=0
    R32=-a1-a3-0.25*s(t1)-a2
    R33=-a1-a3-a2
    R34=a1+a2
    R35=a1-a2
    
    R41=0
    R42=-s(t0)
    R43=-s(t0)
    R44=s(t0)
    R45=0.5*s(t0+t1+t2-t3)-0.5*s(t0-t1-t2+t3)
    
    R51=0
    R52=c(t0)
    R53=c(t0)
    R54=-c(t0)
    R55=0.5*c(t0-t1-t2+t3)-0.5*c(t0+t1+t2-t3)
    
    R61=1
    R62=0
    R63=0
    R64=0
    R65=c(t1+t2-t3)
    
    return np.array([
    [R11, R12, R13, R14, R15],
    [R21, R22, R23, R24, R25],
    [R31, R32, R33, R34, R35]])

print(Jacobiano(theta_0, theta_1, theta_2, theta_3, theta_4))

