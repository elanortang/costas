import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

# Load the data
data = pd.read_csv('known_costas.csv')

num_sols = data.iloc[:, 1]
n_values = data.iloc[:, 0]

# Plot both on the same graph
plt.figure(figsize=(8, 6))
plt.plot(n_values[:-2], num_sols[:-2], marker='o', color='blue', label='Total Number of Solutions')
plt.plot(n_values[30], num_sols[30], marker='o', color='red', label='Number of Solutions Known So Far')
plt.plot(n_values[29], num_sols[29], marker='o', color='red')
plt.yscale('log')
plt.minorticks_on()
plt.xlabel("n")
plt.ylabel("Number of Known Costas Solutions (log scale)")
plt.title("Costas Known Solution Space")
plt.legend()
plt.grid(True)
plt.show()