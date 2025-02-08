#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846  // Define PI
#endif

// Struct to hold data from CSV
struct SensorData {
    double timestamp;
    double pos_x;
    double pos_y;
    double heading;
    double ultrasound[4]; // Ultrasound sensor readings (4 directions)
};

// Struct to represent 2D point
struct Point { int x, y; };

// Bresenhams algorithm for drawing a line between two points
std::vector<Point> bresenhamLine(int x0, int y0, int x1, int y1) {
    std::vector<Point> line;
    int dx = abs(x1 - x0), dy = -abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1, sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        line.push_back({ x0, y0 }); // Store the cur piont
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; } // Move in x direction
        if (e2 <= dx) { err += dx; y0 += sy; } // Move in y direction
    }
    return line;
}

// read CSV file and parse sensor data
std::vector<SensorData> readCSV(const std::string& filename) {
    std::vector<SensorData> data;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::replace(line.begin(), line.end(), ',', ' '); // Convert CSV commas to spaces
        std::stringstream ss(line);
        SensorData sd;
        ss >> sd.timestamp >> sd.pos_x >> sd.pos_y >> sd.heading
            >> sd.ultrasound[0] >> sd.ultrasound[1] >> sd.ultrasound[2] >> sd.ultrasound[3]; // Read sensor data fields
        data.push_back(sd);
    }
    return data;
}

//update the occupancy grid based on sensor readings
void updateOccupancyGrid(std::vector<std::vector<int>>& grid,
    const SensorData& data,
    double gridResolution,
    double maxRange) {

    const double speedOfSound = 343.0; // Speed of sound in air (m/s)

    //fixed sensor angles relative to the robots headings
    const double sensorAngles[4] = { M_PI / 4, 3 * M_PI / 4, -3 * M_PI / 4, -M_PI / 4 }; // 45°, 135°, -135°, -45°

    for (int i = 0; i < 4; ++i) {
        double distance = (data.ultrasound[i] * speedOfSound) / 2.0;  // Calculate distance from ultrasound time-of-flight
        if (distance <= 0 || distance > maxRange) continue; // Ignore invalid readings

        double angle = data.heading + sensorAngles[i]; // comput absolute sensor angle
        double x_end = data.pos_x + distance * cos(angle); // Compute obstacle x position
        double y_end = data.pos_y + distance * sin(angle); // Compute obstacle y position

        // Convert real-world coordinates to grid indices
        int x0 = static_cast<int>(data.pos_x / gridResolution);
        int y0 = static_cast<int>(data.pos_y / gridResolution);
        int x1 = static_cast<int>(x_end / gridResolution);
        int y1 = static_cast<int>(y_end / gridResolution);

        auto line = bresenhamLine(x0, y0, x1, y1); //line from robot to detected obstacle

        for (size_t j = 0; j < line.size(); ++j) {
            int x = line[j].x, y = line[j].y;
            if (x >= 0 && x < grid.size() && y >= 0 && y < grid[0].size()) {
                if (j == line.size() - 1) {
                    grid[x][y] += 10; // Obstacle: Increase occupancy probability
                }
                else {
                    grid[x][y] -= 3; // Free space: Decrease occupancy probability
                }
                // Ensure occupancy values remain within bounds (-10 to 10)
                grid[x][y] = (grid[x][y] < -10) ? -10 : (grid[x][y] > 10) ? 10 : grid[x][y];
            }
        }
    }
}

//to write occupancy grid to a file
void writeGridToFile(const std::vector<std::vector<int>>& grid, const std::string& filename) {
    std::ofstream file(filename);
    for (const auto& row : grid) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i != row.size() - 1) file << " "; // Space-separated values
        }
        file << "\n";
    }
}

int main() {
    //Read sensor data from CSV file
    auto sensorData = readCSV("C:/Users/navai/Downloads/localization homework/robot.csv");

    // Grid parameters
    const double gridResolution = 0.1; // 10cm per cell
    const double maxRange = 2.0; // Maximum sensor range: 2 meters
    const int gridWidth = 100, gridHeight = 100; // Grid size (100x100 cells)

    // Initialize occupancy grid with neutral values (0)
    std::vector<std::vector<int>> grid(gridWidth, std::vector<int>(gridHeight, 0));

    // Update grid based on sensor data
    for (const auto& data : sensorData) {
        updateOccupancyGrid(grid, data, gridResolution, maxRange);
    }

    // Save the final occupancy grid to a file
    writeGridToFile(grid, "occupancy_grid.txt");

    std::cout << "Occupancy grid saved to occupancy_grid.txt" << std::endl;
    return 0;
}
