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

#ifndef KREGRET_INCLUDE_CUBE_H_
#define KREGRET_INCLUDE_CUBE_H_

#include <cmath>
#include <kregret/point.h>

void cube(size_t D, size_t N, int K, struct point *p, int *maxIndex);
int cubealgorithm(size_t D, size_t N, int K, struct point *p, size_t L, int t, struct point *c, struct point *answer);

#endif
