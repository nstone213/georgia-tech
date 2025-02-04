#Taken from Kaggle, run file to download data locally

import kagglehub

path = kagglehub.dataset_download("mlippo/average-global-iq-per-country-with-other-stats")

print("Path to dataset files:", path)