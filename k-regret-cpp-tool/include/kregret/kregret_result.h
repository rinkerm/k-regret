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

#ifndef KREGRET_INCLUDE_KREGRET_RESULT_H
#define KREGRET_INCLUDE_KREGRET_RESULT_H

#include <vector>
#include <kregret/point.h>
struct kregret_result
{
	std::vector<point> result_points;
	double max_regret;

	kregret_result() : result_points(0), max_regret(0) {}

	void addPoint(point p);
	void calculateMaxRegretRatio(size_t N, struct point* p);

};

#endif
