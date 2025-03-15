import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

# File path
file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/luvharishkhati/heart-disease-patients-details/versions/1/heart_disease.csv"
df = pd.read_csv(file_path)

if 'age' in df.columns:
    data = df['age'].dropna()

    mean, std_dev = np.mean(data), np.std(data)
    
    # Compute a, b
    a = mean - 2 * std_dev
    b = mean + 2 * std_dev
    print(f"a = {a:.2f}, b = {b:.2f}")
    
    # Compute Percentiles
    X75 = 61
    
    # Define Events
    A_min, A_max = a, b
    B_min = X75
    
    # Compute Histogram
    bins = 10
    counts, bin_edges = np.histogram(data, bins=bins, density=True)
    bin_width = bin_edges[1] - bin_edges[0]
    probabilities = counts * bin_width
    
    # Bin index
    A_bins = (bin_edges[:-1] >= A_min) & (bin_edges[1:] <= A_max)
    B_bins = bin_edges[:-1] >= B_min
    
    # Compute Probabilities
    P_A = sum(probabilities[A_bins])
    P_B = sum(probabilities[B_bins])
    P_A_and_B = sum(probabilities[A_bins & B_bins])  # Overlap of A and B
    
    # Conditional Prob computation
    P_A_given_B = P_A_and_B / P_B if P_B > 0 else 0
    
    # Print Results
    print(f"P(A) = {P_A:.4f}")
    print(f"P(B) = {P_B:.4f}")
    print(f"P(A and B) = {P_A_and_B:.4f}")
    print(f"P(A | B) = {P_A_given_B:.4f}")

else:
    print("Error")
