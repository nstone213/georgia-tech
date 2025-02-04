import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Path to the dataset
file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/luvharishkhati/heart-disease-patients-details/versions/1/heart_disease.csv"

# Load the dataset
df_heart_disease = pd.read_csv(file_path)

# Select a numerical column for the histogram (e.g., 'age')
if 'age' in df_heart_disease.columns:
    data = df_heart_disease['age'].dropna()

    # Choose a reasonable number of bins (k)
    k = 10  # Adjust as needed

    # Frequency Histogram
    plt.figure(figsize=(10, 6))
    counts, bins, patches = plt.hist(data, bins=k, edgecolor='black')
    plt.title('Frequency Histogram of Age')
    plt.xlabel('Age')
    plt.ylabel('Frequency')
    plt.grid(True)
    plt.show()

    # Relative Frequency Histogram
    C = counts.sum()  # Total number of data points (normalizing constant)
    relative_frequencies = counts / C

    plt.figure(figsize=(10, 6))
    plt.bar(bins[:-1], relative_frequencies, width=np.diff(bins), edgecolor='black', align='edge')
    plt.title('Relative Frequency Histogram of Age')
    plt.xlabel('Age')
    plt.ylabel('Relative Frequency')
    plt.grid(True)
    plt.show()

    # Display the value of C
    print(f"Value of C (normalizing constant): {C}")
else:
    print("The 'age' column is not present in the dataset.")