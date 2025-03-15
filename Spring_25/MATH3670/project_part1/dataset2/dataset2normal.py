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

    #Enumerating bins
    for bins in bin_sizes:
        plt.figure(figsize=(10, 6))
        
        # Histogram per bin size
        counts, bin_edges, _ = plt.hist(data, bins=bins, density=True, alpha=0.5, color='blue', edgecolor='black', label=f"Histogram (bins={bins})")

        # Normal Distribution computation
        x_values = np.linspace(min(data), max(data), 100)
        y_values = norm.pdf(x_values, mean, std_dev)

        plt.plot(x_values, y_values, 'r-', label="Normal Density Function fnorm(x)", linewidth=2)

        # Plot info
        plt.xlabel("Age")
        plt.ylabel("Relative Frequency / Probability Density")
        plt.title(f"Relative Frequency Histogram and Normal Density Function (Bins={bins})")
        plt.legend()
        plt.grid()

        plt.show()

else:
    print("Error")
