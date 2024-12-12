import matplotlib.pyplot as plt
import numpy as np
import csv
import pandas as pd

df = pd.read_csv('./master-avg-runningtime.csv',header=None)
df.rename(columns={df.columns[0]: "n"}, inplace=True)
print(df.head())

# Compute the average of all columns except the first
averages = df.iloc[:, 1:].mean(axis=1)
std_devs = df.iloc[:, 1:].std(axis=1)

# Create the new DataFrame
new_df = pd.DataFrame({
    "n": df["n"],         # First column from original
    "average": averages,   # New column with averages
    "std_devs": std_devs
})
#print(new_df)
# Plot the average with error bars (standard deviation)
plt.errorbar(df["n"], averages, yerr=std_devs, fmt='o', capsize=5)

# Add labels and title
plt.xlabel('n')
plt.ylabel('Seconds (log scale)')
plt.title('Average running time across 80 scranfilized trials')
plt.legend()
# Set the y-axis to a logarithmic scale
plt.yscale('log')

# Show the plot
plt.show()