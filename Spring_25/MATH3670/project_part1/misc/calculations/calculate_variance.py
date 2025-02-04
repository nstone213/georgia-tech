#Imports
import pandas as pd

#Function to calculate variance
def calculate_variance(data):
    n = len(data)
    if n < 2:
        return None

    #Sample variance calculation (See pdf for rationale)
    mean_value = sum(data) / n
    variance = sum((x - mean_value) ** 2 for x in data) / (n - 1)
    return variance

#Main code execution
if __name__ == "__main__":
    
    #File Path (relative)
    file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/luvharishkhati/heart-disease-patients-details/versions/1/heart_disease.csv"
    
    #Reads as a Pandas dataframe
    df = pd.read_csv(file_path)

    #Checks to see if age data exists
    if "age" in df.columns:
        data = df["age"].dropna().tolist()
        variance_value = calculate_variance(data)

        #Prints out sample variance final number
        print(f"Sample Variance: {variance_value}")
    else:
        #'Error' statement
        print("The 'age' column is not present in the dataset.")