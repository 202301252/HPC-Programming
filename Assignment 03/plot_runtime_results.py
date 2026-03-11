import matplotlib.pyplot as plt
import numpy as np

cases = ['a','b','c','d','e']

local = [0.1039,0.5586,0.4180,2.3163,1.9516]
hpc   = [0.37,1.55,1.17,6.46,4.39]

x = np.arange(len(cases))
w = 0.35

plt.figure()
plt.bar(x - w/2, local, width=w)
plt.bar(x + w/2, hpc, width=w)

plt.xticks(x, cases)
plt.xlabel("Problem Case (a–e)")
plt.ylabel("Execution Time (s)")
plt.title("Execution Time: Local vs HPC")
plt.legend(["Local","HPC"])

plt.savefig("localvshpc.png", dpi=300, bbox_inches="tight")
plt.close()
