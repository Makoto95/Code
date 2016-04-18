import numpy as np
import cmath
import math
import pandas as pd
import matplotlib
import matplotlib.pyplot as plt
#import seaborn as sns
from matplotlib.ticker import *
ax = plt.gca()


plt.grid(b=True, which='minor', linestyle='-', color = "0.8")
plt.grid(b=True, which='major', linestyle='-', color = "0.3")

def para(r1, r2):
    return (r1 * r2) / (r1 + r2)

def amplitude(F):
    jomega = 2 * np.pi * F * 1j
    return (1.0/40) * abs(-60500-1j*6.32*10**6-jomega) * abs(-60500+1j*6.32*10**6-jomega) * abs(jomega) / ( abs(-154000-1j*4.64*10**6-jomega) * abs(-154000+1j*4.64*10**6-jomega) * abs(-98500000-jomega) )

def argument(F):
    jomega = 2 * np.pi * F * 1j
    return (cmath.phase(jomega+60500-1j*6.32*10**6) + cmath.phase(jomega+60500+1j*6.32*10**6) + cmath.phase(jomega) - ( cmath.phase(jomega+154000-1j*4.64*10**6) + cmath.phase(jomega+154000+1j*4.64*10**6) + cmath.phase(jomega+98500000) )) * 180 / np.pi

data = pd.read_csv("exp4_7.csv", header = None, names = ("vo", "dt", "cur", "adm", "dphase"))

# f charcteristics plot
plt.hold(True)
plt.subplot(2, 1, 1)
plt.plot(data.index, data["adm"], marker = "o")
plt.xscale("log")
plt.xlabel("Frequency [kHz]")
plt.ylabel("Admittance [S]")
plt.xlim(500, 4000)
plt.ylim(0, 0.03)

L = 45.6 * 10 ** (-6)
Rs = 5.5
Cp = 40 * 10 ** (-12)
C = 470 * 10 ** (-12)
R = 30 + para(4.7, 100**6) + para(4.7, 50)

freq = np.arange(1*10**2, 5*10**3, 0.01)
adm = [amplitude(f * 1000) for f in freq]
arg = [argument(f * 1000) for f in freq]

plt.plot(freq, adm)
# draw diagonal line from (70, 90) to (90, 200)
f1 = 975
f2 = 1140
plt.plot([f1, f1], [0, 0.03], 'k-', color = "0")
plt.plot([f2, f2], [0, 0.03], 'k-', color = "0")
plt.plot([500, 4000], [max(data["adm"]), max(data["adm"])], color = "0")
plt.plot([500, 4000], [max(data["adm"]) / math.sqrt(2), max(data["adm"] / math.sqrt(2))], color = "0")

ax.xaxis.set_major_locator(MultipleLocator(500))
ax.xaxis.set_minor_locator(MultipleLocator(50))
#ax.xaxis.set_major_formatter(matplotlib.ticker.ScalarFormatter())

ax.yaxis.set_major_locator(MultipleLocator(0.01))
ax.yaxis.set_minor_locator(MultipleLocator(0.001))

plt.hold(False)

fo = data["adm"].argmax(data["adm"])
fo_ref = (10**2 + 0.01*np.argmax(adm))
fi = data["adm"].argmin(data["adm"])
fi_ref = (10**2 + 0.01*np.argmin(adm))
print("fo = %.0f [kHz]" % fo)
print("fo_ref = %.2f [kHz]" % fo_ref)
print("fi = %.0f [kHz]" % fi)
print("fi_ref = %.2f [kHz]" % fi_ref)
q_meas = fo / (f2 - f1)
print("Q_from_omega = %f [-]" % q_meas)
#q_ref = 2 * np.pi * fo_ref * L / R
#print("refQ = %f [-]" % q_ref)
#print("error rate = %f" % ((q_meas - q_ref) * 100.0 / q_ref))

plt.hold(True)
plt.subplot(2, 1, 2)
plt.plot(data.index, data["dphase"]*(-1), marker = "o")
plt.xscale("log")
plt.xlabel("Frequency [kHz]")
plt.ylabel("Phase difference [degrees]")
plt.grid(True,which="both",ls="-")
plt.xticks([500, 700, 1000, 1500, 2000, 3000, 4000])
plt.xlim(500, 4000)
#plt.ylim(-10, 10)
plt.plot(freq, arg)
plt.hold(False)

plt.show()
