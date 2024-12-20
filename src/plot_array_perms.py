import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import math

xpoints = np.arange(1, 33, 1)
ypoints = [math.factorial(n) for n in range(1, 33)]

plt.figure(figsize=(8, 6))
plt.plot(xpoints, ypoints, marker='o', color='blue')
plt.yscale('log')
plt.minorticks_on()
plt.xlabel("n")
plt.ylabel("Number of n x n Array Permutations (log scale)")
plt.title("Number of Array Permutations")
plt.grid(True)
plt.show()