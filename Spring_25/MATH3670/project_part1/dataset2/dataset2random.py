import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import binom
import pandas as pd

# File path
file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/luvharishkhati/heart-disease-patients-details/versions/1/heart_disease.csv"
df = pd.read_csv(file_path)

if 'age' in df.columns:
    data = df['age'].dropna().astype(int)  # Ensure integer age
    
    # Compute mean and standard deviation
    mean, std_dev = np.mean(data), np.std(data)
    
    # Variable initializations
    a = mean - 2 * std_dev
    b = mean + 2 * std_dev
    X75 = 61
    A_min, A_max = a, b
    B_min = X75
    n = max(data)
    p = mean / n
    
    # Compute PMF values
    x_values = np.arange(min(data), max(data) + 1)
    y_values = binom.pmf(x_values, n, p)
    
    # Histogram Computation
    bins = 10
    counts, bin_edges = np.histogram(data, bins=bins, density=True)
    bin_width = bin_edges[1] - bin_edges[0]
    
    # Normalize binomial PMF to match histogram
    y_values = y_values * (max(counts) / max(y_values))
    
    # Random Sampling w/ Replacement
    filtered_data = data[data > X75]
    sampled_values = np.random.choice(filtered_data, size=50, replace=True)
    count_in_range = np.sum((sampled_values > a) & (sampled_values < b))
    
    print(f"Number of samples in range ({a:.2f}, {b:.2f}): {count_in_range}/50")
    
else:
    print("Error")
