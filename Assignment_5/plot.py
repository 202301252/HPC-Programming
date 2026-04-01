import os
import pandas as pd
import matplotlib.pyplot as plt

PLOT_DIR = "plots"
os.makedirs(PLOT_DIR, exist_ok=True)

def parse(file):
    parts = file.replace(".csv","").split("_")
    d = {}
    for p in parts:
        if p.startswith("NX"): d["NX"] = int(p[2:])
        if p.startswith("NY"): d["NY"] = int(p[2:])
        if p.startswith("T"): d["T"] = int(p[1:])
    return d

def mover_time(path):
    df = pd.read_csv(path)
    return df["mover"].sum()

def plot_compare(base_path, label):
    for (nx, ny) in [(250,100),(500,200),(1000,400)]:
        plt.figure()

        for mode, name in [(2,"Without Deletion (Exp4)"),
                           (3,"With Deletion (Exp5)")]:
            
            folder = f"{base_path}/exp2/mode{mode}"
            threads = []
            times = []

            for file in os.listdir(folder):
                meta = parse(file)

                if meta["NX"] == nx and meta["NY"] == ny:
                    path = os.path.join(folder, file)
                    t = mover_time(path)

                    threads.append(meta["T"])
                    times.append(t)

            if not threads:
                continue

            threads, times = zip(*sorted(zip(threads, times)))

            base = times[0]  # T(2 threads)
            speedup = [base/x for x in times]

            plt.plot(threads, speedup, marker='o', label=name)

        plt.xlabel("Threads")
        plt.ylabel("Speedup")
        plt.title(f"Exp4 vs Exp5 ({label}) - {nx}x{ny}")
        plt.grid()
        plt.legend()

        filename = f"{PLOT_DIR}/exp4_vs_exp5_{label}_{nx}x{ny}.png"
        plt.savefig(filename)
        plt.close()

# RUN
plot_compare("local results", "lab")
plot_compare("cluster results", "cluster")

print("✅ Final comparison plots generated")