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

# Y-Shift
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

# Two highlighted values
X1 = 50
X2 = 70

# Calculate Y_E at X1 and X2
YE1 = C * np.exp(D * X1)
YE2 = C * np.exp(D * X2)
if shifted:
    YE1 -= K
    YE2 -= K

# Values plotted on figure
X_sorted = np.sort(X)
YE_curve = C * np.exp(D * X_sorted)
if shifted:
    YE_curve -= K

# Scatterplot data
plt.scatter(X, Y, label='Actual Data', alpha=0.7)

# Exponential curve plotted
plt.plot(X_sorted, YE_curve, color='orange', label=r'Fitted $Y_E = Ce^{DX}$')

# Two highlighted points plotted
plt.plot(X1, YE1, marker='*', markersize=12, color='green', label=f'Highlighted Point at X={X1}')
plt.plot(X2, YE2, marker='*', markersize=12, color='purple', label=f'Highlighted Point at X={X2}')

# Labeling
plt.xlabel('Average IQ')
plt.ylabel('Literacy Rate')
plt.title('Exponential Fit: Literacy Rate vs Average IQ')
plt.legend()
plt.grid(True)
plt.show()
