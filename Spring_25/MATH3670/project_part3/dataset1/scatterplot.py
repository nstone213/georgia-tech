import pandas as pd
import matplotlib.pyplot as plt

# File path
file_path = "/Users/nicholasstone/.cache/kagglehub/datasets/mlippo/average-global-iq-per-country-with-other-stats/versions/3/avgIQpercountry.csv"
data = pd.read_csv(file_path)

# Clean data
data.columns = data.columns.str.strip()
data = data.dropna(subset=['Average IQ', 'Literacy Rate'])

# Determine variables
X = data['Average IQ']
Y = data['Literacy Rate']

# Scatterplot plotted
plt.scatter(X, Y)

# Labels
plt.xlabel('Average IQ')
plt.ylabel('Literacy Rate')
plt.title('Scatter Plot of Literacy Rate vs Average IQ')
plt.show()
