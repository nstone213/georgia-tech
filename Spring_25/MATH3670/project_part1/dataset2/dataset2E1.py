import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import norm
import pandas as pd

# File path
file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/luvharishkhati/heart-disease-patients-details/versions/1/heart_disease.csv"
df = pd.read_csv(file_path)

if 'age' in df.columns:
    data = df['age'].dropna()

    # Sample mean and variance
    mean, std_dev = np.mean(data), np.std(data)

    # Bin sizes
    bin_sizes = [5, 10, 20]

    # Error per bin size
    errors = []

    # Enumerating bin sizes
    for bins in bin_sizes:
        plt.figure(figsize=(10, 6))
        
        # Histogram computation
        counts, bin_edges = np.histogram(data, bins=bins, density=True)
        bin_centers = (bin_edges[:-1] + bin_edges[1:]) / 2
        bin_width = bin_edges[1] - bin_edges[0]
        fnorm_values = norm.pdf(bin_centers, mean, std_dev)

        # E1 computation
        E1 = np.sqrt(np.sum((counts - fnorm_values * bin_width) ** 2))
        errors.append((bins, E1))

        # Histogram with enumerated bins
        plt.hist(data, bins=bins, density=True, alpha=0.5, color='blue', edgecolor='black', label=f"Histogram (bins={bins})")
        x_values = np.linspace(min(data), max(data), 100)
        y_values = norm.pdf(x_values, mean, std_dev)
        plt.plot(x_values, y_values, 'r-', label="Normal Density Function fnorm(x)", linewidth=2)

        # Plot info
        plt.xlabel("Age")
        plt.ylabel("Relative Frequency / Probability Density")
        plt.title(f"Histogram vs Normal Distribution (Bins={bins})\nError E1: {E1:.4f}")
        plt.legend()
        plt.grid()

        plt.show()

    # Print E1
    for bins, error in errors:
        print(f"Bins: {bins}, Error E1: {error:.4f}")

else:
    print("Error")
