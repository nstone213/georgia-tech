import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

# File path
file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/luvharishkhati/heart-disease-patients-details/versions/1/heart_disease.csv"
df = pd.read_csv(file_path)

if 'age' in df.columns:
    data = df['age'].dropna().astype(int)  # Age as integers

    # Sample mean and variance
    mean_x = np.mean(data)
    std_x = np.std(data, ddof=1)

    # Initialize a and b
    a = mean_x - 2 * std_x
    b = mean_x + 2 * std_x

    # Initialize quartiles
    X25 = 48
    X50 = 55
    X75 = 61

    # Return values
    print(f"Mean (xbar): {mean_x:.2f}")
    print(f"Standard Deviation (s): {std_x:.2f}")
    print(f"a = xbar - 2s: {a:.2f}")
    print(f"b = xbar + 2s: {b:.2f}")
    print(f"X25 (25th percentile): {X25:.2f}")
    print(f"X50 (Median): {X50:.2f}")
    print(f"X75 (75th percentile): {X75:.2f}")

else:
    print("Error")
