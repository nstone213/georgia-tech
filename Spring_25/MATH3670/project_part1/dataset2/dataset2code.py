#Imports
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# File Path (relative)
file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/luvharishkhati/heart-disease-patients-details/versions/1/heart_disease.csv"

# Interprets data as Pandas dataframe
df_heart_disease = pd.read_csv(file_path)

# Looks for Age column in data
if 'age' in df_heart_disease.columns:
    data = df_heart_disease['age'].dropna()

    # Defines k bins
    k = 10  # Can be adjusted, but 10 is ideal number in my opinion, essentially separates by decades, 
            # which makes sense in the context of heart disease

    # Creating the Frequency Histogram
    plt.figure(figsize=(10, 6))
    counts, bins, patches = plt.hist(data, bins=k, edgecolor='black')
    
    # Title initialized
    plt.title('Frequency Histogram of Age')
    
    # X-axis titled for Age
    plt.xlabel('Age')

    # Y-axis titled for Frequency
    plt.ylabel('Frequency')

    # Initialized grid for visual pruposes
    plt.grid(True)

    # Initialized the histogram in a separate frame
    plt.show()

    # Defining C variable
    C = counts.sum()  # Total number of data points
    relative_frequencies = counts / C

    # Creating the Relative Frequency Histogram
    plt.figure(figsize=(10, 6))
    plt.bar(bins[:-1], relative_frequencies, width=np.diff(bins), edgecolor='black', align='edge')

    # Title initialized
    plt.title('Relative Frequency Histogram of Age')

    # X-axis titled for age
    plt.xlabel('Age')

    # Y-axis titled for relative frequency
    plt.ylabel('Relative Frequency')

    # Initialized grid for visual purposes
    plt.grid(True)

    # Initialized the relative histogram in a separate frame, keep in mind that the first histogram must be x'd 
    #out of to see this second histogram 
    plt.show()

    # Display C, must have x'd out of both histograms to see in terinal (i.e. the program has terminated)
    print(f"Value of C (normalizing constant): {C}")
else:
    #'Error' statement
    print("Age not present in data.")