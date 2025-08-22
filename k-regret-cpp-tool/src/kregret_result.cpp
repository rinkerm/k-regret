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

#include <kregret/kregret_result.h>

void kregret_result::calculateMaxRegretRatio(size_t N,struct point* p) {
    double maxRegret = 0.0;
    size_t D = this->result_points[0].d;
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
        for (int j = 0; j < this->result_points.size(); j++) {
            double utility = dot(this->result_points[j], w);
            maxUtilityInSet = std::max(maxUtilityInSet, utility);
        }

        // Calculate regret for this utility vector
        if (maxUtilityOverall > 0) {
            double regret = (maxUtilityOverall - maxUtilityInSet) / maxUtilityOverall;
            maxRegret = std::max(maxRegret, regret);
        }

        delete[] w;
    }

    this->max_regret = maxRegret;
}
