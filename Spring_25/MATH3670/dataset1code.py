import pandas as pd

file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/ankushpanday1/diabetes-prediction-in-india-dataset/versions/1/diabetes_prediction_india (1).csv"
df = pd.read_csv(file_path)

print(df.head())