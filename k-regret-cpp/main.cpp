#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <limits>
#include "include/cube.h"
#include "include/point.h"
// Function to calculate max regret ratio
double calculateMaxRegretRatio(int D, int N, int K, struct point* p, int* resultIndices) {
    double maxRegret = 0.0;
    
    // For each point not in the result set
    for (int i = 0; i < N; i++) {
        bool inResultSet = false;
        for (int j = 0; j < K; j++) {
            if (resultIndices[j] == i) {
                inResultSet = true;
                break;
            }
        }
        
        if (!inResultSet) {
            // Find the maximum regret for this point
            double pointRegret = 0.0;
            
            // Generate random utility vectors or use a systematic approach
            // For simplicity, we'll check along each dimension and some combinations
            for (int d = 0; d < D; d++) {
                double* w = new double[D];
                for (int j = 0; j < D; j++) {
                    w[j] = (j == d) ? 1.0 : 0.0;
                }
                
                // Find max utility in result set
                double maxUtilityInSet = -std::numeric_limits<double>::infinity();
                for (int j = 0; j < K; j++) {
                    double utility = dot(p[resultIndices[j]], w);
                    maxUtilityInSet = std::max(maxUtilityInSet, utility);
                }
                
                // Calculate regret
                double currentUtility = dot(p[i], w);
                double regret = (currentUtility - maxUtilityInSet) / currentUtility;
                pointRegret = std::max(pointRegret, regret);
                
                delete[] w;
            }
            
            maxRegret = std::max(maxRegret, pointRegret);
        }
    }
    
    return maxRegret;
}

// Main function to read CSV and run cube algorithm
void processCSVWithCube(const char* filename, int K, double& maxRegretRatio, int*& resultIndices) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        exit(1);
    }
    
    std::vector<std::vector<double>> data;
    std::string line;
    
    // Read CSV file
    while (std::getline(file, line)) {
        std::vector<double> row;
        std::stringstream ss(line);
        std::string value;
        
        while (std::getline(ss, value, ',')) {
            // Remove any whitespace
            value.erase(0, value.find_first_not_of(" \t\r\n"));
            value.erase(value.find_last_not_of(" \t\r\n") + 1);
            
            try {
                row.push_back(std::stod(value));
            } catch (const std::exception& e) {
                std::cerr << "Error parsing value: " << value << std::endl;
                continue;
            }
        }
        
        if (row.size() == 5) {  // Assuming 5 elements per row
            data.push_back(row);
        }
    }
    file.close();
    
    int N = data.size();
    int D = 5;  // Dimension is 5
    
    if (N == 0) {
        std::cerr << "Error: No valid data found in file" << std::endl;
        exit(1);
    }
    
    // Convert to point array
    struct point* points = new struct point[N];
    for (int i = 0; i < N; i++) {
        points[i].d = D;
        points[i].a = new double[D];
        for (int j = 0; j < D; j++) {
            points[i].a[j] = data[i][j];
        }
    }
    
    // Allocate memory for result indices
    resultIndices = new int[K];
    
    // Run cube algorithm
    cube(D, N, K, points, resultIndices);
    
    // Calculate max regret ratio
    maxRegretRatio = calculateMaxRegretRatio(D, N, K, points, resultIndices);
    
    // Clean up
    for (int i = 0; i < N; i++) {
        delete[] points[i].a;
    }
    delete[] points;
}

// Example usage
int main() {
    const char* filename = "test_data/nba.csv";
    int K = 20;  // Number of points to select
    double maxRegretRatio;
    int* resultIndices;
    
    processCSVWithCube(filename, K, maxRegretRatio, resultIndices);
    
    std::cout << "Max Regret Ratio: " << maxRegretRatio << std::endl;
    std::cout << "Result Set Indices: ";
    for (int i = 0; i < K; i++) {
        std::cout << resultIndices[i] << " ";
    }
    std::cout << std::endl;
    
    delete[] resultIndices;
    
    return 0;
}