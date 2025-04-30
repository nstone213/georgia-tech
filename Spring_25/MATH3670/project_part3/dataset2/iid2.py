import pandas as pd
import matplotlib.pyplot as plt
import random
import math
from scipy.stats import norm
random.seed(42)

# File path
file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/luvharishkhati/heart-disease-patients-details/versions/1/heart_disease.csv"
df = pd.read_csv(file_path)

# Determine Variables
X = df["age"].tolist()

# Sample mean and variance computations
N_X = len(X)
X_mean = sum(X) / N_X
S_X2 = sum((xi - X_mean)**2 for xi in X) / (N_X - 1)
S_X = math.sqrt(S_X2)

# Create W, 40 random samples
W = random.sample(X, 40)
W_mean = sum(W) / len(W)

# 95% Confidence interval
z = 1.96
margin_error = z * S_X / math.sqrt(len(W))
CI_lower = W_mean - margin_error
CI_upper = W_mean + margin_error

# Print CI
print("=== Confidence Interval ===")
print(f"Sample mean of W (W̄): {W_mean}")
print(f"Sample mean of X (X̄): {X_mean}")
print(f"Sample standard deviation of X (S_X): {S_X}")
print(f"95% Confidence Interval for μ: ({CI_lower}, {CI_upper})")
print("Is X̄ within the CI?", CI_lower <= X_mean <= CI_upper)

# Plot W histogram
plt.figure(figsize=(10, 6))
plt.hist(W, bins=10, edgecolor='black', density=True)
plt.xlabel("Age (Sampled W)")
plt.ylabel("Relative Frequency")
plt.title("Relative Frequency Histogram of W (n=40)")
plt.grid(True)
plt.show()
Z_stat = (W_mean - X_mean) / (S_X / math.sqrt(len(W)))

# Two-tailed p-value
p_value = 2 * (1 - norm.cdf(abs(Z_stat)))

# Print hypothesis test results
print("\n=== Hypothesis Test ===")
print(f"Z statistic: {Z_stat}")
print(f"p-value: {p_value}")

# Deliberation
if p_value < 0.05:
    print("Reject H₀ at the 5% significance level.")
else:
    print("Fail to reject H₀ at the 5% significance level.")
