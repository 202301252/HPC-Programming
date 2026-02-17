import pandas as pd
import matplotlib.pyplot as plt

# -------- Load CSV --------
csv_file = "results/ikj.csv"   # change if needed
df = pd.read_csv(csv_file)

# -------- Compute MFLOPS --------
N = df["ProblemSize"]
time = df["AvgAlgoTime"]

mflops = (2 * (N ** 3)) / (time * 1e6)

# -------- Plot --------
plt.figure(figsize=(8, 6))
plt.plot(N, mflops, marker='o')

plt.xlabel("Matrix Size (N)")
plt.ylabel("Performance (MFLOPS)")
plt.title("Performance (MFLOPS) vs Matrix Size\n ikj Matrix Multiplication(Local)")
plt.grid(True)

plt.xscale("log", base=2)
plt.yscale("log")

plt.tight_layout()
plt.savefig("results/mflops_vs_matrix_size__local.png")
plt.show()

