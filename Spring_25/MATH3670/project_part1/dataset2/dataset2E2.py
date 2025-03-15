import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from scipy.stats import binom

# File path
file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/luvharishkhati/heart-disease-patients-details/versions/1/heart_disease.csv"
df = pd.read_csv(file_path)

if 'age' in df.columns:
    data = df['age'].dropna().astype(int)  # Age as integer
    
    # Sample mean and variance
    sample_mean = np.mean(data)
    sample_var = np.var(data)
    
    # Binomial params
    n = max(data)  # N = 270
    p = sample_mean / n  # probability of success normalized
    
    # Bin sizes
    bin_sizes = [5, 10, 20]
    errors = {}
    
    for bins in bin_sizes:
        plt.figure(figsize=(10, 6))
        
        # Histogram
        counts, bin_edges, _ = plt.hist(data, bins=bins, density=True, alpha=0.5, color='blue', edgecolor='black', label=f"Histogram (bins={bins})")
        
        # Normalize PMF to fit plot
        x_values = np.arange(min(data), max(data) + 1)
        y_values = binom.pmf(x_values, n, p)
        y_values = y_values * (max(counts) / max(y_values))
        
        # Overlay PMF
        plt.plot(x_values, y_values, 'r-', label="Binomial PMF", linewidth=2)
        
        # Compute E2 error
        E2 = np.sqrt(np.sum((counts - np.interp((bin_edges[:-1] + bin_edges[1:]) / 2, x_values, y_values)) ** 2))
        errors[bins] = E2
        
        # Plot info
        plt.xlabel("Age")
        plt.ylabel("Relative Frequency / Probability Mass")
        plt.title(f"Histogram and Binomial PMF (Bins={bins}) with E2={E2:.4f}")
        plt.legend()
        plt.grid()

        plt.show()
    
    # Return E2
    print("Comparison of E2 for different bin sizes:")
    for bins, error in errors.items():
        print(f"Bins: {bins}, Error E2: {error:.4f}")
else:
    print("Error")