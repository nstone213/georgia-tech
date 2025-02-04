import pandas as pd

# Path to the dataset
file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/luvharishkhati/heart-disease-patients-details/versions/1/heart_disease.csv"

# Load the dataset
df_heart_disease = pd.read_csv(file_path)

# Count the number of entries (rows) in the dataset
num_entries = len(df_heart_disease)

# Display the number of entries
print(f"Total number of entries (rows) in the dataset: {num_entries}")