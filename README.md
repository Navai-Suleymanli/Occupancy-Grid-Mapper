# Occupancy-Grid-Mapper
I wrote a C++ program that reads the provided data and creates the occupancy grid in .txt format, and then I visualized the created grid using matplotlib in python.

#------------Brief info about the  provided files:

There are total 4 files and 2folders(I created folders to separate the code files, each folder has only 1 code file inside). 

Folder:C++ program to create occupancy grid
1. occupancygridmaker.cpp: This code file is for creating an occupancy grid in form of .txt by reading data from robot.csv(input file provided in homework)

Folder: Visualization using Python:
2. Vs.py: Python code with Matplotlib to visualize the occupancy grid that was created by the c++ code.

3. Readme.txt (this file)

4. occupancy_grid.txt: the result of running occupancygridmaker.cpp. You can try deleting this file and running the c++ program yourself

5. result.png: The result of running vs.py with the occupancy_grid.txt(created by the c++ program)



#-------------Summary of the methods I used:
This program reads robot sensor data from a CSV file and updates a 2D occupancy grid with sonar measurements. Grid models the environment, with each cell storing a value that represents whether the space is free , unknown(not possible to determine based on the info) or occupied.

1. CSV File Parsing (readCSV)

--Reads the sensor data from a CSV file.
--Stores each row into a SensorData struct containing timestamp, position (x, y), heading, and ultrasonic readings.

2. Bresenham’s Line Algorithm (bresenhamLine)

--Used to determine the points along a straight line between two grid coordinates.
--Helps in marking free space and obstacles in the occupancy grid.

3.Occupancy Grid Update (updateOccupancyGrid)

--Converts real-world positions to grid coordinates.
--Uses ultrasonic sensor readings to determine obstacle locations.
--Applies Bresenham’s algorithm to mark free and occupied cells in the grid.
--Ensures occupancy values stay within a predefined range (-10 to 10).

4. Writing Grid Data (writeGridToFile)

--Saves the occupancy grid as a text file with space-separated values.

5. Main Program (main)

--Reads sensor data.
--Initializes an occupancy grid.
--Updates the grid iteratively based on sensor measurements.
--Saves the final occupancy grid to a file.

6. Python Visualization

--Reads the saved occupancy grid.
--Uses matplotlib to visualize the occupancy map.
--Displays free space, obstacles, and unknown areas with a color map.



#------------- Resources and Literature used:

1. Probabilistic Robotics by by Dieter Fox, Sebastian Thrun, and Wolfram Burgard (2005): https://docs.ufpr.br/~danielsantos/ProbabilisticRobotics.pdf
2. Bresenham, J.E. (1965). "Algorithm for Computer Control of a Digital Plotter." IBM Systems Journal: The original paper introducing the algorithm https://ieeexplore.ieee.org/document/5388473
3. GeeksforGeeks - Bresenham’s Line Algorithm: https://www.geeksforgeeks.org/bresenhams-line-generation-algorithm/
4. MIT OpenCourseWare - Robotic Mapping and Navigation

