import os
import csv
import subprocess
from collections import defaultdict

# User-configurable parameters
EXECUTABLE = "./output"          # compiled binary
NUM_RUNS = 5                     # number of repetitions
OUTPUT_CSV = "results/avg_algo_times.csv"

# Ensure results directory exists
os.makedirs(os.path.dirname(OUTPUT_CSV), exist_ok=True)

# Storage for results (keyed by problem size)
results = defaultdict(lambda: {
    "e2e_times": [],
    "algo_times": []
})

print(f"Running benchmark {NUM_RUNS} times...\n")

# Run executable multiple times
for run_id in range(NUM_RUNS):
    print(f"=== Run {run_id + 1}/{NUM_RUNS} ===")

    proc = subprocess.run(
        [EXECUTABLE],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        check=True
    )

    lines = proc.stdout.strip().split("\n")

    # Print raw output
    for line in lines:
        print(line)

    # Skip header, parse data
    for line in lines[1:]:
        parts = [p.strip() for p in line.split(",")]
        if len(parts) < 3:
            continue

        problem_size = int(parts[0])
        e2e_time = float(parts[1])
        algo_time = float(parts[2])

        results[problem_size]["e2e_times"].append(e2e_time)
        results[problem_size]["algo_times"].append(algo_time)

    print()

# Write averaged results to CSV
with open(OUTPUT_CSV, "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow([
        "ProblemSize",
        "AvgE2ETime",
        "AvgAlgoTime"
    ])

    for problem_size in sorted(results.keys()):
        entry = results[problem_size]

        avg_e2e = sum(entry["e2e_times"]) / len(entry["e2e_times"])
        avg_algo = sum(entry["algo_times"]) / len(entry["algo_times"])

        writer.writerow([
            problem_size,
            f"{avg_e2e:.9f}",
            f"{avg_algo:.9f}"
        ])

print(f"Averaged results written to: {OUTPUT_CSV}")

