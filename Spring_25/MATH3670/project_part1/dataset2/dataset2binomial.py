import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from scipy.stats import binom

# File path
file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/luvharishkhati/heart-disease-patients-details/versions/1/heart_disease.csv"
df = pd.read_csv(file_path)

if 'age' in df.columns:
    data = df['age'].dropna().astype(int)  # Ensure integer age
    
    # Sample mean and variance
    sample_mean = np.mean(data)
    sample_var = np.var(data)
    
    # Binomial params
    n = max(data)        # N = 170
    p = sample_mean / n  # Normalized success probability
    
    # Bin sizes
    bin_sizes = [5, 10, 20]
    
    # Enumerating bins
    for bins in bin_sizes:
        plt.figure(figsize=(10, 6))
        
        # Histogram plot
        counts, bin_edges, _ = plt.hist(data, bins=bins, density=True, alpha=0.5, color='blue', edgecolor='black', label=f"Histogram (bins={bins})")
        
        # Normalizing binomial distribution to match skew
        x_values = np.arange(min(data), max(data) + 1)
        y_values = binom.pmf(x_values, n, p)
        y_values = y_values * (max(counts) / max(y_values))
        
        # PMF Overlay
        plt.plot(x_values, y_values, 'r-', label="Binomial PMF", linewidth=2)
        
        # Plot info
        plt.xlabel("Age")
        plt.ylabel("Relative Frequency / Probability Mass")
        plt.title(f"Histogram and Binomial PMF (Bins={bins})")
        plt.legend()
        plt.grid()

        plt.show()
else:
    print("Error")