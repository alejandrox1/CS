import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv("results.txt", header=None, delim_whitespace=True).as_matrix()

L = df[:,1]
R = [0] * (len(L)-1)
for i in range(1, len(L)):
    R[i-1] = L[i-1]/ L[i]

plt.plot(df[:,0][:-1], R, linestyle='--', marker='o', linewidth=2, markersize=6, color='#FF00FF')
plt.show()
