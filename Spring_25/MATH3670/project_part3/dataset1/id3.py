import pandas as pd
import numpy as np
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

# Y Shift
if (Y <= 0).any():
    K = abs(Y.min()) + 1
    print(f"Shifting Y by +{K} to make all values positive for log.")
    Y_shifted = Y + K
    shifted = True
else:
    Y_shifted = Y
    K = 0
    shifted = False

# Natural Log
log_Y = np.log(Y_shifted)

# Linear Regression
N = len(X)
X_mean = X.mean()
logY_mean = log_Y.mean()
sum_logYXi = (log_Y * X).sum()
sum_X2 = (X * X).sum()

D = (sum_logYXi - N * logY_mean * X_mean) / (sum_X2 - N * X_mean**2)
ln_C = logY_mean - D * X_mean
C = np.exp(ln_C)

print(f"\nExponential fit coefficients:")
print(f"D = {D}")
print(f"C = {C}")

YE = C * np.exp(D * X)
if shifted:
    YE -= K

# SS_RE
SS_RE = ((Y - YE) ** 2).sum()

# S_YY
S_YY = ((Y - Y.mean()) ** 2).sum()

# R_E^2
R_squared_E = 1 - (SS_RE / S_YY)

# Results printed to terminal
print(f"\nSum of Squares of Residuals (SS_RE) = {SS_RE}")
print(f"Total Sum of Squares (S_YY) = {S_YY}")
print(f"Coefficient of Determination (R_E^2) = {R_squared_E}")
