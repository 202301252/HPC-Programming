import matplotlib.pyplot as plt
import numpy as np

# ======================
# EXPERIMENT 1 (HPC)
# ======================
particles = np.array([1e2,1e4,1e6,1e8])

exp1_250 = np.array([2.1e-5,2.399e-3,0.233879,23.4918])
exp1_500 = np.array([3.6e-5,2.572e-3,0.251826,20.6272])
exp1_1000 = np.array([6.0e-5,4.732e-3,0.273085,20.7831])

plt.figure()
plt.loglog(particles,exp1_250,'o-',label="250x100")
plt.loglog(particles,exp1_500,'s-',label="500x200")
plt.loglog(particles,exp1_1000,'^-',label="1000x400")
plt.xlabel("Number of Particles")
plt.ylabel("Total Interpolation Time (s)")
plt.title("Experiment 1: Scaling (HPC)")
plt.legend()
plt.grid(True)
plt.savefig("exp1_scaling_hpc.png",dpi=300)

# ======================
# EXPERIMENT 2 (HPC)
# ======================
configs=[1,2,3]
exp2=[21.6401,24.5539,26.0663]

plt.figure()
plt.plot(configs,exp2,'o-')
plt.xlabel("Problem Index")
plt.ylabel("Total Interpolation Time (s)")
plt.title("Experiment 2: Consistency (HPC)")
plt.grid(True)
plt.savefig("exp2_consistency_hpc.png",dpi=300)

# ======================
# EXPERIMENT 3 (HPC)
# ======================
mover_serial=np.array([
0.260365,0.260672,0.260678,0.260018,0.261347,
0.259835,0.260142,0.259830,0.260868,0.261494
])

mover_parallel=np.array([
0.053831,0.051870,0.054237,0.055182,0.054381,
0.051630,0.051939,0.053816,0.052628,0.053093
])

avg_serial=np.mean(mover_serial)
avg_parallel=np.mean(mover_parallel)
speedup=avg_serial/avg_parallel

plt.figure(figsize=(6,5))
bars=plt.bar(["Serial","Parallel"],[avg_serial,avg_parallel])
plt.ylabel("Mover Time (s)")
plt.title(f"HPC Mover Performance (Speedup = {speedup:.2f}×)")
plt.grid(axis="y",linestyle="--",alpha=0.6)

for b in bars:
    h=b.get_height()
    plt.text(b.get_x()+b.get_width()/2,h,f"{h:.3f}",
             ha="center",va="bottom")

plt.tight_layout()
plt.savefig("exp3_speedup_hpc.png",dpi=300)

print("HPC Serial:",avg_serial)
print("HPC Parallel:",avg_parallel)
print("HPC Speedup:",speedup)
