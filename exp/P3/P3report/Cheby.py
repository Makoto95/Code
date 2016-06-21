import numpy as np
import cmath
import math

def get_s(a, b):
    return math.sin(a)*math.sinh(b) + 1j*math.cos(a)*math.cosh(b)

def get_a(k, n):
    return (math.pi/(2.0*n))*(2.0*k+1)

def get_b_plus(n):
#    print("%f" % np.arcsinh(1.0))
    return (1.0/n)*np.arcsinh(1.0)

def get_b_minus(n):
    return -(1.0/n)*np.arcsinh(1.0)

def L_1(a, b, c):
    return (c-a/b)

def L_2(a, b, c):
    return (a/b)

def C(a, b, c):
    return (b/L_1(a,b,c))

klist = [0,1,2,3,4,5]
alist = [get_a(k,3.0) for k in klist]
blist = [get_b_plus(3), get_b_minus(3)]

s = [get_s(a, b) for a in alist for b in blist]

snum = [i if s[i].real < 0 else 'fizz' for i in range(12)]

for i in range(12):
    if snum[i] != 'fizz':
        print("snum = %d, s = %f + j %f" % (snum[i], s[i].real, s[i].imag))

#以下は重複結果を排除するため、表示結果確認後の追記。追記後再び実行。
alpha = -1.0/(s[1]*s[3]*s[5])
beta = (s[1]+s[3]+s[5])/(s[1]*s[3]*s[5])
gamma =  - (s[1]*s[3] + s[3]*s[5] + s[5]*s[1])/(s[1]*s[3]*s[5])

print"L1 = %f, L2 = %f, C1 = %f" % (L_1(alpha.real, beta.real, gamma.real), L_2(alpha.real, beta.real, gamma.real), C(alpha.real, beta.real, gamma.real)))
