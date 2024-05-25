import numpy as np
import matplotlib.pyplot as plt

# Load data from the Euler output file
euler_data = np.loadtxt("euler_outputMoon.dat")
euler_t = euler_data[:, 0]  # Time
euler_x = euler_data[:, 1]  # x position
euler_y = euler_data[:, 2]  # y position

# Load data from the RK4 output file
rk4_data = np.loadtxt("rk4_outputMoon.dat")
rk4_t = rk4_data[:, 0]  # Time
rk4_x = rk4_data[:, 1]  # x position
rk4_y = rk4_data[:, 2]  # y position

# Plot the orbits
plt.figure(figsize=(8, 6))
plt.plot(euler_x, euler_y, label="Euler's Method", color='blue')
plt.plot(rk4_x, rk4_y, label="RK4 Method", color='red')
plt.title("Comparison of Orbits: Euler's Method vs RK4")
plt.xlabel("x")
plt.ylabel("y")
plt.legend()
plt.grid(True)
plt.gca().set_aspect('equal', adjustable='box')
plt.show()
