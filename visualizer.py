import numpy as np
import matplotlib.pyplot as plt

# Load occupancy grid data from a text file
grid = np.loadtxt("occupancy_grid.txt", dtype=int)

# Display the occupancy grid using a color map
plt.imshow(grid, cmap='viridis', interpolation='none', origin='lower')

# Add a color bar to indicate occupancy values
plt.colorbar(label='Occupancy (0=Unknown, 10=Blocked, Negative=Free)')

# Label the axes
plt.xlabel('X Cell Index')
plt.ylabel('Y Cell Index')

# Set the title of the plot
plt.title('Occupancy Grid Map')

# Show the plot
plt.show()
