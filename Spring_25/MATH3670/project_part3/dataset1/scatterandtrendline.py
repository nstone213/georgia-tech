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

# Y_L for each X calculation
Y_L = A + B * X

# Scatterplot plotted
plt.scatter(X, Y, label='Actual Data')

# Regression line plotted
plt.plot(X, Y_L, color='red', label='Fitted Line: $Y_L = A + BX$')

# Two chosen values
X1 = 50
X2 = 70

# Y_L1, Y_L2 on the fitted line
YL1 = A + B * X1
YL2 = A + B * X2

# Plot the two highlighted points
plt.plot(X1, YL1, marker='*', markersize=12, color='green', label=f'Highlighted Point at X={X1}')
plt.plot(X2, YL2, marker='*', markersize=12, color='purple', label=f'Highlighted Point at X={X2}')

# Labels
plt.xlabel('Average IQ')
plt.ylabel('Literacy Rate')
plt.title('Scatter Plot of Literacy Rate vs Average IQ with Fitted Line and Highlighted Points')
plt.legend()
plt.show()
