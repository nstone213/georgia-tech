import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/ankushpanday1/diabetes-prediction-in-india-dataset/versions/1/diabetes_prediction_india (1).csv"
df = pd.read_csv(file_path)

plt.figure(figsize=(10, 6))

#Only using 20% of the data
sample_df = df.sample(frac=0.2, random_state=42)

sns.scatterplot(
    data=sample_df,
    x="Age",
    y="Fasting_Blood_Sugar",
    hue="Diabetes_Status",
    palette={"Yes": "red", "No": "blue"},
    alpha=0.6,
    s=20
)


plt.title("Age vs Fasting Blood Sugar (Colored by Diabetes Status)")
plt.xlabel("Age")
plt.ylabel("Fasting Blood Sugar")
plt.legend(title="Diabetes Status")
plt.grid(True)
plt.show()
