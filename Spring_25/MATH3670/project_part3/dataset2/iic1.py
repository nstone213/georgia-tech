import pandas as pd

# File path
file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/luvharishkhati/heart-disease-patients-details/versions/1/heart_disease.csv"
df = pd.read_csv(file_path)

# Determine variables
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

# Fetch quartiles
X_25 = interpolate(X_sorted, pos_25)
X_50 = interpolate(X_sorted, pos_50)
X_75 = interpolate(X_sorted, pos_75)

# Display quartiles
print(f"Number of data points (N_X) = {N_X}")
print(f"X_25 (25th percentile) = {X_25}")
print(f"X_50 (50th percentile, median) = {X_50}")
print(f"X_75 (75th percentile) = {X_75}")
