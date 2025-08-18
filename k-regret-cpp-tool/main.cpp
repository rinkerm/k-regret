//==========================================================================================
//Copyright 2025 ©, 2025 Matthew Rinker
//
//This file is a part of the k-regret-cpp project.
//
//The k-regret-cpp project is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    The k-regret-cpp project is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//along with this program. If not, see <http://www.gnu.org/licenses/>.
//==========================================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <limits>
#include <cstring>
#include <iomanip>

#include <kregret/cube.h>
#include <kregret/point.h>

void printHelp(const char* programName) {
    std::cout << "\nNAME\n";
    std::cout << "    " << programName << " - Run k-regret algorithm on CSV data to find representative subset\n\n";
    
    std::cout << "SYNOPSIS\n";
    std::cout << "    " << programName << " -f FILEPATH -d DIMENSIONS [-k SIZE] [-h]\n\n";
    
    std::cout << "DESCRIPTION\n";
    std::cout << "    This program reads a CSV file containing multi-dimensional data points and uses a\n";
    std::cout << "    k-regret algorithm to select a representative subset that minimizes the maximum regret\n";
    std::cout << "    ratio. The regret ratio measures the worst-case utility loss when using the subset\n";
    std::cout << "    instead of the full dataset.\n\n";
    
    std::cout << "OPTIONS\n";
    std::cout << "    -f FILEPATH\n";
    std::cout << "        Path to the input CSV file (required). The file should contain numeric data\n";
    std::cout << "        with each row representing a data point and each column a dimension.\n\n";
    
    std::cout << "    -d DIMENSIONS\n";
    std::cout << "        Number of dimensions (columns) in the input file (required).\n";
    std::cout << "        Must be a positive integer.\n\n";
    
    std::cout << "    -k SIZE\n";
    std::cout << "        Size of the result set to select (optional, default: 20).\n";
    std::cout << "        Must be a positive integer less than or equal to the total number of points.\n\n";
    
    std::cout << "    -h\n";
    std::cout << "        Display this help message and exit.\n\n";
    
    std::cout << "EXAMPLES\n";
    std::cout << "    " << programName << " -f data.csv -d 5 -k 10\n";
    std::cout << "        Process data.csv with 5 dimensions, selecting 10 representative points.\n\n";
    
    std::cout << "    " << programName << " -f products.csv -d 3\n";
    std::cout << "        Process products.csv with 3 dimensions, using default size of 20.\n\n";
    
    std::cout << "OUTPUT\n";
    std::cout << "    The program outputs:\n";
    std::cout << "    - Maximum regret ratio (0.0 to 1.0)\n";
    std::cout << "    - Indices of selected points in the original dataset\n";
    std::cout << "    - Summary statistics\n\n";
    
    std::cout << "EXIT STATUS\n";
    std::cout << "    0    Success\n";
    std::cout << "    1    Error in command line arguments\n";
    std::cout << "    2    Error reading input file\n";
    std::cout << "    3    Error in data processing\n\n";
}

double calculateMaxRegretRatio(int D, int N, int K, struct point* p, int* resultIndices) {
    double maxRegret = 0.0;
    
    // Check axis-aligned utilities
    for (int d = 0; d < D; d++) {
        double* w = new double[D];
        for (int j = 0; j < D; j++) {
            w[j] = (j == d) ? 1.0 : 0.0;
        }
        
        // Find maximum utility among ALL points
        double maxUtilityOverall = -std::numeric_limits<double>::infinity();
        for (int i = 0; i < N; i++) {
            double utility = dot(p[i], w);
            if (utility > maxUtilityOverall) {
                maxUtilityOverall = utility;
            }
        }
        
        // Find maximum utility in the result set
        double maxUtilityInSet = -std::numeric_limits<double>::infinity();
        for (int j = 0; j < K; j++) {
            double utility = dot(p[resultIndices[j]], w);
            maxUtilityInSet = std::max(maxUtilityInSet, utility);
        }
        
        // Calculate regret for this utility vector
        if (maxUtilityOverall > 0) {
            double regret = (maxUtilityOverall - maxUtilityInSet) / maxUtilityOverall;
            maxRegret = std::max(maxRegret, regret);
        }
        
        delete[] w;
    }
    
    return maxRegret;
}

void processCSVWithCube(const char* filename, int D, int K, double& maxRegretRatio, int*& resultIndices, int& N) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        exit(2);
    }
    
    std::vector<std::vector<double>> data;
    std::string line;
    int lineNumber = 0;
    
    // Read CSV file
    while (std::getline(file, line)) {
        lineNumber++;
        std::vector<double> row;
        std::stringstream ss(line);
        std::string value;
        
        while (std::getline(ss, value, ',')) {
            // Remove any whitespace
            value.erase(0, value.find_first_not_of(" \t\r\n"));
            value.erase(value.find_last_not_of(" \t\r\n") + 1);
            
            if (!value.empty()) {
                try {
                    row.push_back(std::stod(value));
                } catch (const std::exception& e) {
                    std::cerr << "Warning: Error parsing value '" << value 
                             << "' at line " << lineNumber << std::endl;
                    continue;
                }
            }
        }
        
        if (row.size() == D) {
            data.push_back(row);
        } else if (!row.empty()) {
            std::cerr << "Warning: Line " << lineNumber << " has " << row.size() 
                     << " values, expected " << D << ". Skipping." << std::endl;
        }
    }
    file.close();
    
    N = data.size();
    
    if (N == 0) {
        std::cerr << "Error: No valid data found in file" << std::endl;
        exit(3);
    }
    
    if (K > N) {
        std::cerr << "Error: Result set size (k=" << K << ") cannot be larger than number of points (n=" << N << ")" << std::endl;
        exit(3);
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

// Updated main function with command line parsing
int main(int argc, char* argv[]) {
    // Default values
    char* filename = nullptr;
    int D = -1;  // Dimensions (required)
    int K = 20;  // Result set size (default)
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            printHelp(argv[0]);
            return 0;
        }
        else if (strcmp(argv[i], "-f") == 0) {
            if (i + 1 < argc) {
                filename = argv[++i];
            } else {
                std::cerr << "Error: -f requires a filename argument\n";
                return 1;
            }
        }
        else if (strcmp(argv[i], "-d") == 0) {
            if (i + 1 < argc) {
                try {
                    D = std::stoi(argv[++i]);
                    if (D <= 0) {
                        std::cerr << "Error: Dimensions must be positive\n";
                        return 1;
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error: Invalid dimension value\n";
                    return 1;
                }
            } else {
                std::cerr << "Error: -d requires a numeric argument\n";
                return 1;
            }
        }
        else if (strcmp(argv[i], "-k") == 0) {
            if (i + 1 < argc) {
                try {
                    K = std::stoi(argv[++i]);
                    if (K <= 0) {
                        std::cerr << "Error: Result set size must be positive\n";
                        return 1;
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error: Invalid k value\n";
                    return 1;
                }
            } else {
                std::cerr << "Error: -k requires a numeric argument\n";
                return 1;
            }
        }
        else {
            std::cerr << "Error: Unknown option '" << argv[i] << "'\n";
            std::cerr << "Use -h for help\n";
            return 1;
        }
    }
    
    // Validate required arguments
    if (filename == nullptr) {
        std::cerr << "Error: Filename is required. Use -f to specify the input file.\n";
        std::cerr << "Use -h for help\n";
        return 1;
    }
    
    if (D == -1) {
        std::cerr << "Error: Number of dimensions is required. Use -d to specify.\n";
        std::cerr << "Use -h for help\n";
        return 1;
    }
    
    // Process the file
    double maxRegretRatio;
    int* resultIndices;
    int N;  // Total number of points
    
    std::cout << "\n=== Cube Algorithm Analysis ===" << std::endl;
    std::cout << "Input file: " << filename << std::endl;
    std::cout << "Dimensions: " << D << std::endl;
    std::cout << "Target result set size: " << K << std::endl;
    std::cout << "\nProcessing..." << std::endl;
    
    processCSVWithCube(filename, D, K, maxRegretRatio, resultIndices, N);
    
    // Output results
    std::cout << "\n=== Results ===" << std::endl;
    std::cout << "Total points in dataset: " << N << std::endl;
    std::cout << "Points selected: " << K << std::endl;
    std::cout << "Maximum Regret Ratio: " << std::fixed << std::setprecision(6) << maxRegretRatio << std::endl;
    std::cout << "Regret percentage: " << std::fixed << std::setprecision(2) << (maxRegretRatio * 100) << "%" << std::endl;
    
    std::cout << "\nSelected point indices: ";
    for (int i = 0; i < K; i++) {
        std::cout << resultIndices[i];
        if (i < K - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    
    std::cout << "\nInterpretation: In the worst case, a user would get " 
              << std::fixed << std::setprecision(5) << ((1 - maxRegretRatio) * 100) 
              << "% of their maximum possible utility\nwhen choosing from the selected subset.\n" << std::endl;
    
    delete[] resultIndices;
    
    return 0;
}