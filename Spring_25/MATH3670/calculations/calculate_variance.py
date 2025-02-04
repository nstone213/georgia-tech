import pandas as pd

def calculate_variance(data):
    n = len(data)
    if n < 2:
        return None  # Variance is not defined for n < 2

    mean_value = sum(data) / n
    variance = sum((x - mean_value) ** 2 for x in data) / (n - 1)
    return variance

if __name__ == "__main__":
    # Example dataset
    file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/luvharishkhati/heart-disease-patients-details/versions/1/heart_disease.csv"
    df = pd.read_csv(file_path)

    if "age" in df.columns:
        data = df["age"].dropna().tolist()
        variance_value = calculate_variance(data)
        print(f"Sample Variance: {variance_value}")
    else:
        print("The 'age' column is not present in the dataset.")