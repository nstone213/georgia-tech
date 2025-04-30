import pandas as pd
import math
from scipy.stats import norm

# File path
file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/luvharishkhati/heart-disease-patients-details/versions/1/heart_disease.csv"
df = pd.read_csv(file_path)

# Determine variables
X = df["age"].tolist()
X_sorted = sorted(X)
N_X = len(X_sorted)
def interpolate(sorted_list, position):
    lower_index = int(position) - 1
    upper_index = lower_index + 1
    fraction = position - int(position)
    if upper_index >= len(sorted_list):
        return sorted_list[lower_index]
    return sorted_list[lower_index] + fraction * (sorted_list[upper_index] - sorted_list[lower_index])

# Q1 and Q3 computation
pos_25 = 0.25 * (N_X + 1)
pos_75 = 0.75 * (N_X + 1)
X_25 = interpolate(X_sorted, pos_25)
X_75 = interpolate(X_sorted, pos_75)
V = [x for x in X if X_25 <= x <= X_75]
N_V = len(V)
V_mean = sum(V) / N_V

# Stats
X_mean = sum(X) / N_X
S_X2 = sum((xi - X_mean)**2 for xi in X) / (N_X - 1)
S_X = math.sqrt(S_X2)

# Z-statistic computation
Z_stat = (V_mean - X_mean) / (S_X / math.sqrt(N_V))

# Two-tailed p-value
p_value = 2 * (1 - norm.cdf(abs(Z_stat)))

# Print results
print("=== Hypothesis Test for V ===")
print(f"X̄ (population mean under H₀): {X_mean}")
print(f"V̄ (sample mean of V): {V_mean}")
print(f"N_V (sample size of V): {N_V}")
print(f"Z statistic: {Z_stat}")
print(f"p-value: {p_value}")

# Deliberation
if p_value < 0.05:
    print("Reject H₀ at the 5% level.")
else:
    print("Fail to reject H₀ at the 5% level.")
