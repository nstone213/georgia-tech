# Imports
import pandas as pd
import matplotlib.pyplot as plt

# File Path (relative)
file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/mlippo/average-global-iq-per-country-with-other-stats/versions/3/avgIQpercountry.csv"

# Interpreting data as Pandas dataframe
df_avg_iq = pd.read_csv(file_path)

# Looking to see if Average IQ and Literacy Rates occur in the data
if 'Average IQ' in df_avg_iq.columns and 'Literacy Rate' in df_avg_iq.columns:
    
    # Creates the scatter plot
    plt.figure(figsize=(10, 6))
    plt.scatter(df_avg_iq['Average IQ'], df_avg_iq['Literacy Rate'], alpha=0.7)

    # Labels and Title
    # Label for x-axis
    plt.xlabel('Average IQ')
    # Label for y-axis
    plt.ylabel('Literacy Rate (%)')
    # Title
    plt.title('Scatter Plot of Average IQ vs. Literacy Rate per Country')

    # Background grid initialized for visual purposes
    plt.grid(True)

    # Initialize the scatterplot in a separate frame
    plt.show()
else:
    #'Error' statement
    print("Ensure the dataset has 'Average IQ' and 'Literacy Rate' columns.")
