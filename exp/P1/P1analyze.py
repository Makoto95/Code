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

def impedance(f, R, L, C, Rs, Cp):
    w = 2 * np.pi * f
    s = 0 + 0j
    s = s + w * 1j
    return R + (Rs + s*L) / (1 + s*Cp*Rs + (s**2)*L*Cp) + 1 / (s*C)

data = pd.read_csv("exp4_7.csv", header = None, names = ("vo", "dt", "cur", "adm", "dphase"))

# f charcteristics plot
plt.hold(True)
plt.plot(data.index, data["adm"], marker = "o")
plt.xscale("log")
plt.xlabel("Frequency [kHz]")
plt.ylabel("Admittance [S]")
plt.xlim(500, 4000)
plt.ylim(0, 0.03)

L = 45.6 * 10 ** (-6)
Rs = 5.5
Cp = 24 * 10 ** (-12)
C = 470 * 10 ** (-12)
R = 30 + para(4.7, 100**6) + para(4.7, 50)

freq = np.arange(1*10**2, 6*10**3, 0.01)
adm = [abs(1.0 / impedance(f * 10**3, R, L, C, Rs, Cp)) for f in freq]

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
ax.xaxis.set_major_formatter(matplotlib.ticker.ScalarFormatter())

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

plt.show()
