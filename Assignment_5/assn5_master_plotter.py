import os
import pandas as pd
import matplotlib.pyplot as plt

BASE = "."  # you are already inside assn 5
PLOT_DIR = "plots"

# create plots folder if not exists
os.makedirs(PLOT_DIR, exist_ok=True)

# ---------------- PARSE FILE ----------------
def parse_filename(file):
    parts = file.replace(".csv","").split("_")
    data = {}
    for p in parts:
        if p.startswith("NX"):
            data["NX"] = int(p[2:])
        elif p.startswith("NY"):
            data["NY"] = int(p[2:])
        elif p.startswith("P"):
            data["P"] = int(p[1:])
        elif p.startswith("T"):
            data["T"] = int(p[1:])
        elif p.startswith("M"):
            data["M"] = int(p[1:])
    return data

# ---------------- READ TIME ----------------
def get_total_time(path):
    df = pd.read_csv(path)
    return df["total"].sum()

# ================= EXP 1 =================
def plot_exp1(base_path, label):
    for (nx, ny) in [(250,100),(500,200),(1000,400)]:
        plt.figure()

        x, y = [], []
        folder = f"{base_path}/exp1/mode1"

        for file in os.listdir(folder):
            meta = parse_filename(file)

            if meta["NX"] == nx and meta["NY"] == ny:
                path = os.path.join(folder, file)
                val = get_total_time(path)

                x.append(meta["P"])
                y.append(val)

        if not x:
            print(f"No data for {label} {nx}x{ny}")
            continue

        x, y = zip(*sorted(zip(x, y)))

        plt.plot(x, y, marker='o', label=label)

        plt.xscale("log")
        plt.yscale("log")
        plt.xlabel("Particles")
        plt.ylabel("Total Time")
        plt.title(f"Exp1 {nx}x{ny} ({label})")
        plt.grid()
        plt.legend()

        filename = f"{PLOT_DIR}/exp1_{label}_{nx}x{ny}.png"
        plt.savefig(filename)
        plt.close()

# ================= EXP 1 LAB VS CLUSTER =================
def plot_exp1_compare():
    for (nx, ny) in [(250,100),(500,200),(1000,400)]:
        plt.figure()

        for base_path,label in [("local results","Lab"),
                                ("cluster results","Cluster")]:

            x, y = [], []
            folder = f"{base_path}/exp1/mode1"

            for file in os.listdir(folder):
                meta = parse_filename(file)

                if meta["NX"] == nx and meta["NY"] == ny:
                    path = os.path.join(folder, file)
                    val = get_total_time(path)

                    x.append(meta["P"])
                    y.append(val)

            if not x:
                continue

            x, y = zip(*sorted(zip(x, y)))
            plt.plot(x, y, marker='o', label=label)

        plt.xscale("log")
        plt.yscale("log")
        plt.xlabel("Particles")
        plt.ylabel("Total Time")
        plt.title(f"Lab vs Cluster {nx}x{ny}")
        plt.grid()
        plt.legend()

        filename = f"{PLOT_DIR}/exp1_compare_{nx}x{ny}.png"
        plt.savefig(filename)
        plt.close()

# ================= PPC PLOT =================
def plot_ppc(base_path, label):
    plt.figure()

    for (nx, ny) in [(250,100),(500,200),(1000,400)]:
        x, y = [], []
        folder = f"{base_path}/exp1/mode1"

        for file in os.listdir(folder):
            meta = parse_filename(file)

            if meta["NX"] == nx and meta["NY"] == ny:
                path = os.path.join(folder, file)
                total = get_total_time(path)

                ppc = meta["P"]/(nx*ny)
                per_particle = total/meta["P"]

                x.append(ppc)
                y.append(per_particle)

        if not x:
            continue

        x, y = zip(*sorted(zip(x, y)))
        plt.plot(x, y, marker='o', label=f"{nx}x{ny}")

    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel("Particles per Cell")
    plt.ylabel("Time per Particle")
    plt.title(f"PPC Plot ({label})")
    plt.grid()
    plt.legend()

    filename = f"{PLOT_DIR}/ppc_{label}.png"
    plt.savefig(filename)
    plt.close()

# ================= EXP 2 =================
def plot_exp2(base_path, label):
    for (nx, ny) in [(250,100),(500,200),(1000,400)]:
        plt.figure()

        for mode,label2 in [(2,"Immediate"),(3,"Deferred")]:
            folder = f"{base_path}/exp2/mode{mode}"

            threads = []
            times = []

            for file in os.listdir(folder):
                meta = parse_filename(file)

                if meta["NX"] == nx and meta["NY"] == ny:
                    path = os.path.join(folder, file)
                    val = get_total_time(path)

                    threads.append(meta["T"])
                    times.append(val)

            if not threads:
                continue

            threads, times = zip(*sorted(zip(threads, times)))

            base = times[0]
            speedup = [base/t for t in times]

            plt.plot(threads, speedup, marker='o', label=label2)

        plt.xlabel("Threads")
        plt.ylabel("Speedup")
        plt.title(f"Exp2 {nx}x{ny} ({label})")
        plt.grid()
        plt.legend()

        filename = f"{PLOT_DIR}/exp2_{label}_{nx}x{ny}.png"
        plt.savefig(filename)
        plt.close()

# ================= RUN =================
plot_exp1("local results", "lab")
plot_exp1("cluster results", "cluster")

plot_exp1_compare()

plot_ppc("local results", "lab")
plot_ppc("cluster results", "cluster")

plot_exp2("local results", "lab")
plot_exp2("cluster results", "cluster")

print("✅ All plots saved in 'plots' folder")