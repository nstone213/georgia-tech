import pandas as pd
import matplotlib.pyplot as plt

# File path
file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/luvharishkhati/heart-disease-patients-details/versions/1/heart_disease.csv"
df = pd.read_csv(file_path)

# Determine variable
X = df["age"]
X_sorted = sorted(X)
N_X = len(X_sorted)

# Calculate quartiles
pos_25 = 0.25 * (N_X + 1)
pos_50 = 0.50 * (N_X + 1)
pos_75 = 0.75 * (N_X + 1)
def interpolate(sorted_list, position):
    lower_index = int(position) - 1
    upper_index = lower_index + 1
    fraction = position - int(position)
    if upper_index >= len(sorted_list):
        return sorted_list[lower_index]
    else:
        return sorted_list[lower_index] + fraction * (sorted_list[upper_index] - sorted_list[lower_index])
X_25 = interpolate(X_sorted, pos_25)
X_50 = interpolate(X_sorted, pos_50)
X_75 = interpolate(X_sorted, pos_75)

# Create Relative Frequency Histogram
plt.figure(figsize=(10, 6))
plt.hist(X, bins=15, edgecolor='black', density=True)

# Plot quartiles
plt.axvline(X_25, color='red', linestyle='dashed', linewidth=2, label=f'X₍₂₅₎ = {X_25:.2f}')
plt.axvline(X_50, color='green', linestyle='dashed', linewidth=2, label=f'X₍₅₀₎ = {X_50:.2f}')
plt.axvline(X_75, color='blue', linestyle='dashed', linewidth=2, label=f'X₍₇₅₎ = {X_75:.2f}')

# Labels
plt.xlabel('Age')
plt.ylabel('Relative Frequency')
plt.title('Relative Frequency Histogram of Age with Quartiles')
plt.legend()
plt.grid(True)
plt.show()

# Printed values
print(f"Number of data points (N_X) = {N_X}")
print(f"X_25 (25th percentile) = {X_25}")
print(f"X_50 (50th percentile, median) = {X_50}")
print(f"X_75 (75th percentile) = {X_75}")
