#Imports
import pandas as pd

#Function to calculate the quartiles
def calculate_quartiles(data):
    quartiles = {
        "Q1 (25th percentile)": data.quantile(0.25),
        "Q2 (50th percentile - Median)": data.quantile(0.5),
        "Q3 (75th percentile)": data.quantile(0.75)
    }
    return quartiles

if __name__ == "__main__":
    # Example dataset
    file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/luvharishkhati/heart-disease-patients-details/versions/1/heart_disease.csv"

    df = pd.read_csv(file_path)

    if "age" in df.columns:
        data = df["age"].dropna()
        quartile_values = calculate_quartiles(data)
        print("Quartile Values:")
        for label, value in quartile_values.items():
            print(f"{label}: {value}")
    else:
        print("The 'age' column is not present in the dataset.")
