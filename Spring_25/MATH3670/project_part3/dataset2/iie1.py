import pandas as pd
import matplotlib.pyplot as plt

# File path
file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/luvharishkhati/heart-disease-patients-details/versions/1/heart_disease.csv"
df = pd.read_csv(file_path)

# Determine variables
X = df["age"]
X_sorted = sorted(X)
N_X = len(X_sorted)
def interpolate(sorted_list, position):
    lower_index = int(position) - 1
    upper_index = lower_index + 1
    fraction = position - int(position)
    if upper_index >= len(sorted_list):
        return sorted_list[lower_index]
    return sorted_list[lower_index] + fraction * (sorted_list[upper_index] - sorted_list[lower_index])

# X25 and X75, # of variables
pos_25 = 0.25 * (N_X + 1)
pos_75 = 0.75 * (N_X + 1)
X_25 = interpolate(X_sorted, pos_25)
X_75 = interpolate(X_sorted, pos_75)
V = [x for x in X if X_25 <= x <= X_75]
N_V = len(V)

# V, relative frequency histogram
plt.figure(figsize=(10, 6))
plt.hist(V, bins=10, edgecolor='black', density=True)
plt.xlabel("Age (V: Q1 ≤ X ≤ Q3)")
plt.ylabel("Relative Frequency")
plt.title("Relative Frequency Histogram of V (Interquartile Range)")
plt.grid(True)
plt.show()

# Printed values
print(f"X_25 (Q1): {X_25}")
print(f"X_75 (Q3): {X_75}")
print(f"Number of values in V (N_V): {N_V}")
