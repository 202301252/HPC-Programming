import matplotlib.pyplot as plt
import numpy as np

cases = ['a','b','c','d','e']

local_opt = [0.0816,0.4455,0.3228,1.7745,1.9433]
hpc_opt   = [0.28,1.17,0.99,5.07,4.53]

local_sp = [1.27,1.25,1.30,1.31,1.00]
hpc_sp   = [1.32,1.32,1.18,1.27,0.97]

x = np.arange(len(cases))
w = 0.35

# -------- Optimized runtime --------
plt.figure()
plt.bar(x - w/2, local_opt, width=w)
plt.bar(x + w/2, hpc_opt, width=w)

plt.xticks(x, cases)
plt.xlabel("Problem Case (a–e)")
plt.ylabel("Execution Time (s)")
plt.title("Optimized Runtime: Local vs HPC")
plt.legend(["Local","HPC"])

plt.savefig("optimized_runtime.png", dpi=300, bbox_inches="tight")
plt.close()

# -------- Speedup --------
plt.figure()
plt.bar(x - w/2, local_sp, width=w)
plt.bar(x + w/2, hpc_sp, width=w)

plt.xticks(x, cases)
plt.xlabel("Problem Case (a–e)")
plt.ylabel("Speedup")
plt.title("Interpolation Optimization Speedup")
plt.legend(["Local","HPC"])

plt.savefig("speedup.png", dpi=300, bbox_inches="tight")
plt.close()

