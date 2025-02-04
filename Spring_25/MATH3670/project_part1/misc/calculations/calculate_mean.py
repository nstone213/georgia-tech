# Imports
import pandas as pd

#Function
def calculate_mean(data):

    #Return statement for sample mean
    return sum(data) / len(data) if data else None

#Main code execution
if __name__ == "__main__":

    #File path (relative)
    file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/luvharishkhati/heart-disease-patients-details/versions/1/heart_disease.csv"
    
    #Reads to Pandas datafram
    df = pd.read_csv(file_path)

    #Looking for age in the dataframe
    if "age" in df.columns:

        #Converts age to a list
        data = df["age"].dropna().tolist()

        #Calculates sample mean by referring to python file
        sample_mean_value = calculate_mean(data)

        #Prints to terminal
        print(f"Sample Mean: {sample_mean_value}")
    else:
        #'Error' message
        print("The 'age' column is not present in the dataset.")
