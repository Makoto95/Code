import numpy as np
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

def impedance(f, R, L, C):
    w = 2 * np.pi * f
    s = w * J
    res = R
    reac = s * L - 1 / (s * C)
    return res + reac

#sns.set_style("whitegrid")
# plt.tick_params(axis='both', direction = "in", width = 0.1)

data = pd.read_csv("exp4_7.csv", header = None, names = ("vo", "dt", "cur", "adm", "dphase"))

# f charcteristics plot
plt.hold(True)
plt.subplot(2, 1, 1)
plt.plot(data.index, data["adm"], marker = "o")
plt.xscale("log")
plt.xlabel("Frequency [kHz]")
plt.ylabel("Admittance [S]")
#plt.grid(True,which="both",ls="-")
#plt.xticks([500, 600, 700, 800, 900, 1000, 1500, 2000, 3000, 4000])
plt.xlim(500, 4000)
plt.ylim(0, 0.03)
#fchar.set_xticks([500, 700, 1000, 1500, 2000, 3000, 4000])
#fchar.get_xaxis().set_major_formatter(matplotlib.ticker.ScalarFormatter())

L = 45.6 * 10 ** (-6)
C = 498 * 10 ** (-12)
R = 30 + para(4.7, 100**6) + para(4.7, 50) + 4.6 + 8.56

freq = np.arange(5*10**2, 5*10**3, 10)
adm = [1.0 / impedance(f * 1000, R, L, C) for f in freq]

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
fo_ref = 1 / (2 * np.pi * math.sqrt(L * C))

print("fo = %f [kHz]" % fo)
print("fo_ref = %f [kHz]" % fo_ref)
q_meas = fo / (f2 - f1)
print("Q_from_omega = %f [-]" % q_meas)
q_ref = 2 * np.pi * fo_ref * L / R
print("refQ = %f [-]" % q_ref)
print("error rate = %f" % ((q_meas - q_ref) * 100.0 / q_ref))

plt.subplot(2, 1, 2)
plt.plot(data.index, data["dphase"], marker = "o")
plt.xscale("log")
plt.xlabel("Frequency [kHz]")
plt.ylabel("Phase difference [S]")
plt.grid(True,which="both",ls="-")
plt.xticks([500, 700, 1000, 1500, 2000, 3000, 4000])
plt.xlim(500, 4000)

#plt.plot()
plt.show()
