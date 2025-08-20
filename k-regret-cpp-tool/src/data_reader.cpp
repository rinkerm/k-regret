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

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <kregret/data_reader.h>


std::vector<std::vector<double>> processData(const char* filename,const char sep, size_t& D, size_t& N) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        exit(2);
    }

    std::vector<std::vector<double>> data;
    std::string line;
    int lineNumber = 0;
    D = 0;

    // Read CSV file
    while (std::getline(file, line)) {

        lineNumber++;
        std::vector<double> row;
        std::stringstream ss(line);
        std::string value;

        while (std::getline(ss, value, sep)) {
            // Remove any whitespace
            value.erase(0, value.find_first_not_of(" \t\r\n"));
            value.erase(value.find_last_not_of(" \t\r\n") + 1);

            if (!value.empty()) {
                try {
                    row.push_back(std::stod(value));
                }
                catch (const std::exception& e) {
                    std::cerr << "Warning: Error parsing value '" << value
                        << "' at line " << lineNumber
                        << " (" << e.what() << ")" << std::endl;
                    continue;
                }
            }
        }
        if (lineNumber == 1) { D = row.size(); }
        if (row.size() == D) {
            data.push_back(row);
        }
        else if (!row.empty()) {
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

    return data;
}