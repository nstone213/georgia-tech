import pandas as pd
import matplotlib.pyplot as plt

# File path
file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/mlippo/average-global-iq-per-country-with-other-stats/versions/3/avgIQpercountry.csv"
data = pd.read_csv(file_path)

# Clean data
data.columns = data.columns.str.strip()
data = data.dropna(subset=['Average IQ', 'Literacy Rate'])

# Determine variables
X = data['Average IQ']
Y = data['Literacy Rate']

# Calculate terms
N = len(X)
X_mean = X.mean()
Y_mean = Y.mean()
sum_YiXi = (Y * X).sum()
sum_Xi2 = (X * X).sum()

# A and B calculations
B = (sum_YiXi - N * Y_mean * X_mean) / (sum_Xi2 - N * X_mean**2)
A = Y_mean - B * X_mean

print(f"Calculated coefficients:")
print(f"B (slope) = {B}")
print(f"A (intercept) = {A}")

Y_L = A + B * X

# SS_RL
SS_RL = ((Y - Y_L) ** 2).sum()

# S_YY
S_YY = ((Y - Y_mean) ** 2).sum()

# R_L^2
R_squared = 1 - (SS_RL / S_YY)

print(f"\nSS_RL = {SS_RL}")
print(f"S_YY = {S_YY}")
print(f"R_L^2 = {R_squared}")
