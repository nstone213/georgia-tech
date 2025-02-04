import pandas as pd
import matplotlib.pyplot as plt

# Path to the dataset
file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/mlippo/average-global-iq-per-country-with-other-stats/versions/3/avgIQpercountry.csv"

# Load the dataset
df_avg_iq = pd.read_csv(file_path)

# Display the column names to verify the correct names
print("Available columns:", df_avg_iq.columns)

print(f"Total number of entries (rows) in the dataset: {len(df_avg_iq)}")

# Replace 'Literacy Rate' with the actual column name if different
# Ensure the dataset has 'Average IQ' and 'Literacy Rate' columns
if 'Average IQ' in df_avg_iq.columns and 'Literacy Rate' in df_avg_iq.columns:
    # Create the scatter plot
    plt.figure(figsize=(10, 6))
    plt.scatter(df_avg_iq['Average IQ'], df_avg_iq['Literacy Rate'], alpha=0.7)

    # Add labels and title
    plt.xlabel('Average IQ')
    plt.ylabel('Literacy Rate (%)')
    plt.title('Scatter Plot of Average IQ vs. Literacy Rate per Country')

    # Add grid for better readability
    plt.grid(True)

    # Show the plot
    plt.show()
else:
    print("Ensure the dataset has 'Average IQ' and 'Literacy Rate' columns.")
