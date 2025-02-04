import pandas as pd

def calculate_mean(data):

    return sum(data) / len(data) if data else None

if __name__ == "__main__":

    file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/luvharishkhati/heart-disease-patients-details/versions/1/heart_disease.csv"
    df = pd.read_csv(file_path)

    if "age" in df.columns:
        data = df["age"].dropna().tolist()
        mean_value = calculate_mean(data)
        print(f"Sample Mean: {mean_value}")
    else:
        print("The 'age' column is not present in the dataset.")
