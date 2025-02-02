import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/ankushpanday1/diabetes-prediction-in-india-dataset/versions/1/diabetes_prediction_india (1).csv"
df = pd.read_csv(file_path)

plt.figure(figsize=(10, 6))
sns.scatterplot(
    data=df,
    x="Age",                # Replace with actual column name if different
    y="Fasting_Blood_Sugar",  # Replace with actual column name
    hue="Diabetes_Status",         # Replace with actual diabetes status column (Yes/No)
    palette={"Yes": "red", "No": "blue"}
)

plt.title("Age vs Fasting Blood Sugar (Colored by Diabetes Status)")
plt.xlabel("Age")
plt.ylabel("Fasting Blood Sugar")
plt.legend(title="Diabetes Status")
plt.grid(True)
plt.show()
