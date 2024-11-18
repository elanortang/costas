import matplotlib.pyplot as plt
import numpy as np
import csv

# Specify the path to your CSV file
file_path = "displacement-frequencies-24.csv"

# Initialize a set to store the data
data_set = set()

# Read the CSV file
with open(file_path, mode="r") as file:
    csv_reader = csv.reader(file)
    next(csv_reader)  # Skip the header row if present
    
    # Add each row as a tuple to the set
    for row in csv_reader:
        int_row = tuple(map(int, row))
        data_set.add(int_row)

# Unpack the data
x, y, values = zip(*data_set)

# Create a scatter plot with a colormap
plt.figure(figsize=(6, 6))
sc = plt.scatter(x, y, c=values, cmap="viridis", s=200, edgecolor="k")
plt.colorbar(sc, label="Value")
plt.xlabel("X")
plt.ylabel("Y")
plt.title("Scatter Plot with Color-Mapped Values")
plt.grid(True)
plt.show()