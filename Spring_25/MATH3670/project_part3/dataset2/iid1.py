import pandas as pd
import matplotlib.pyplot as plt
import random
random.seed(42)

# File path
file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/luvharishkhati/heart-disease-patients-details/versions/1/heart_disease.csv"
df = pd.read_csv(file_path)

# Determine variable
DII = df["age"].tolist()

# Random sample of 40
W = random.sample(DII, 40)

# Plot W, the relative frequency histogram
plt.figure(figsize=(10, 6))
plt.hist(W, bins=10, edgecolor='black', density=True)

# Labels
plt.xlabel("Sampled Age (W)")
plt.ylabel("Relative Frequency")
plt.title("Relative Frequency Histogram of W (Random Sample of 40 from DII)")
plt.grid(True)
plt.show()

# Printed values
print("Random Sample (W):")
print(W)
