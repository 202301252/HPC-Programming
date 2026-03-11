import matplotlib.pyplot as plt
import numpy as np

# =========================
# EXPERIMENT 1
# =========================
particles = np.array([1e2, 1e4, 1e6, 1e8])

exp1_250 = np.array([8e-6, 4.62e-4, 4.251e-2, 4.3159])
exp1_500 = np.array([7e-6, 4.31e-4, 4.375e-2, 4.5084])
exp1_1000 = np.array([1e-5, 7.52e-4, 7.533e-2, 7.6510])

plt.figure()
plt.loglog(particles, exp1_250, 'o-', label="250x100")
plt.loglog(particles, exp1_500, 's-', label="500x200")
plt.loglog(particles, exp1_1000, '^-', label="1000x400")
plt.xlabel("Number of Particles")
plt.ylabel("Total Interpolation Time (s)")
plt.title("Experiment 1: Scaling with Particles (Lab PC)")
plt.legend()
plt.grid(True)
plt.savefig("exp1_scaling_lab.png", dpi=300)

# =========================
# EXPERIMENT 2
# =========================
configs = [1, 2, 3]
exp2_times = [4.3246, 4.4521, 7.3810]

plt.figure()
plt.plot(configs, exp2_times, 'o-')
plt.xlabel("Problem Index")
plt.ylabel("Total Interpolation Time (s)")
plt.title("Experiment 2: Consistency Across Configurations")
plt.grid(True)
plt.savefig("exp2_consistency_lab.png", dpi=300)

# =========================
# EXPERIMENT 3
# =========================
iters = np.arange(1,11)

interp_serial = np.array([
0.106569,0.105990,0.105588,0.105188,0.106502,
0.106312,0.105777,0.106086,0.109341,0.106059
])

mover_serial = np.array([
0.323011,0.321976,0.321359,0.320484,0.328139,
0.334825,0.345002,0.360283,0.337617,0.343968
])

interp_parallel = np.array([
0.108878,0.108700,0.107953,0.109443,0.110346,
0.109945,0.109996,0.109556,0.109939,0.108497
])

mover_parallel = np.array([
0.025312,0.024777,0.024840,0.024939,0.024692,
0.025637,0.025111,0.024649,0.025429,0.024885
])

total_serial = interp_serial + mover_serial
total_parallel = interp_parallel + mover_parallel

# Iteration vs times
plt.figure()
plt.plot(iters, interp_serial, label="Interpolation")
plt.plot(iters, mover_serial, label="Mover")
plt.plot(iters, total_serial, label="Total")
plt.xlabel("Iteration")
plt.ylabel("Time (s)")
plt.title("Experiment 3: Serial Times")
plt.legend()
plt.grid(True)
plt.savefig("exp3_serial_times.png", dpi=300)

# Serial vs Parallel mover
avg_serial = np.mean(mover_serial)
avg_parallel = np.mean(mover_parallel)

plt.figure()
plt.bar(["Serial","Parallel"], [avg_serial, avg_parallel])
plt.ylabel("Mover Time (s)")
plt.title("Mover Serial vs Parallel")
plt.savefig("exp3_mover_compare.png", dpi=300)

# Speedup
speedup = avg_serial / avg_parallel

plt.figure()
plt.bar(["Speedup"], [speedup])
plt.ylabel("Speedup")
plt.title("OpenMP Mover Speedup")
plt.savefig("exp3_speedup.png", dpi=300)

print("Average Serial Mover:", avg_serial)
print("Average Parallel Mover:", avg_parallel)
print("Speedup:", speedup)
