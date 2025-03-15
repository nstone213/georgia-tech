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
    
    # a and b computation
    a = mean - 2 * std_dev
    b = mean + 2 * std_dev
    print(f"a = {a:.2f}, b = {b:.2f}")

    # Compute Percentiles
    X75 = 61
    
    # Define intervals
    A_min, A_max = a, b
    B_min = X75
    
    # Define parameters for binomial distribution approximation
    n = max(data) # n = 270
    p = mean / n   # Normalized probability of success
    
    # Compute PMF values
    x_values = np.arange(min(data), max(data) + 1)
    y_values = binom.pmf(x_values, n, p)
    
    # Histogram Computation
    bins = 10
    counts, bin_edges = np.histogram(data, bins=bins, density=True)
    bin_width = bin_edges[1] - bin_edges[0]
    
    # Normalize binomial PMF to match histogram
    y_values = y_values * (max(counts) / max(y_values))
    
    # Compute Probabilities
    A_bins = (bin_edges[:-1] >= A_min) & (bin_edges[1:] <= A_max)
    B_bins = bin_edges[:-1] >= B_min
    
    P_A = sum(counts[A_bins] * bin_width)
    P_B = sum(counts[B_bins] * bin_width)
    P_A_and_B = sum(counts[A_bins & B_bins] * bin_width)
    
    # Conditional Probability computation
    P_A_given_B = P_A_and_B / P_B if P_B > 0 else 0
    
    # Return results
    print(f"P(A) = {P_A:.4f}")
    print(f"P(B) = {P_B:.4f}")
    print(f"P(A and B) = {P_A_and_B:.4f}")
    print(f"P(A | B) = {P_A_given_B:.4f}")

else:
    print("Error")