import pandas as pd

# File path
file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/luvharishkhati/heart-disease-patients-details/versions/1/heart_disease.csv"
df = pd.read_csv(file_path)

# Determine variables
X = df["age"]

# # of observations
N_X = len(X)

# Sample Mean calculation
sample_mean = sum(X) / N_X

# Sample Variance calculation
sum_squared_diff = sum((xi - sample_mean) ** 2 for xi in X)
sample_variance = sum_squared_diff / (N_X - 1)

# Display
print(f"Sample Mean (X̄) for 'age': {sample_mean}")
print(f"Sample Variance (S²_X) for 'age': {sample_variance}")