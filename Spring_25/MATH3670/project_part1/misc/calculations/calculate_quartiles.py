#Imports
import pandas as pd

#Function to calculate the quartiles
def calculate_quartiles(data):

    #Listing the quartiles and calculating them
    quartiles = {
        #1st quartile
        "25 percentile": data.quantile(0.25),

        #2nd quartile
        "50 percentile": data.quantile(0.5),

        #3rd quartile
        "75 percentile": data.quantile(0.75)
    }

    #Return statement
    return quartiles

#Main code execution
if __name__ == "__main__":

    #File Path (relative)
    file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/luvharishkhati/heart-disease-patients-details/versions/1/heart_disease.csv"

    #Uses Pandas to read dataframe
    df = pd.read_csv(file_path)

    #Checking for age data
    if "age" in df.columns:
        data = df["age"].dropna()
        quartile_values = calculate_quartiles(data)
        print("Quartile Values:")

        #Individually prints quartiles to terminal
        for quartiled, num in quartile_values.items():
            print(f"{quartiled}: {num}")
    else:
        #'Error' statement
        print("Age not present in data.")
