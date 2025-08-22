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

#include <kregret/point.h>

#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

point* pointArray(std::vector<std::vector<double>> data, size_t D, size_t N) 
{
	// Convert to point array
	struct point* points = new struct point[N];
	for (int i = 0; i < N; i++) {
		points[i].d = D;
		points[i].a = new double[D];
		for (int j = 0; j < D; j++) {
			points[i].a[j] = data[i][j];
		}
	}

	return points;
}

double dot(struct point p, double* w)
{
// This struct stores a point and its component in a d-dimensional space
// Example:
// 		struct point p;

// Returns the dot product of vector p and w.
// p is the data point
// w is the tuple representing a utilify function
	double product = 0.0;
	int i;

	for(i = 0; i < p.d; ++i)
		product += p.a[i] * w[i];

	return product;
}


struct point maxPoint(int N, struct point *p, double *v)
{
// Finds farthest in direction v.
// This method is intended to find the boundary points
	int i, maxIndex = 0;
	double max = 0.0;

	for(i = 0; i < N; ++i)
		if (dot(p[i], v) > max)
		{
			maxIndex = i;
			max = dot(p[i], v);
		}

	return p[maxIndex];
}


// Prints the components of a point
std::string to_string(point p)
{
	std::string s = "";
	for (int i = 0; i < p.d; ++i)
		s += std::to_string(p.a[i]);
		s += " ";
	s+="\n";
  return s;
}


int pointcmp(const void *a, const void *b)
{
// Compares two points lexicographically.
// Return values:
//		-1: a < b
// 		 1: a > b
// 		 0: a = b
// Example:
// 		p1(1,2,3,4,6) and p2(1,2,3,5,5)
// 		=> p1 < p2 because p1[i] = p2[i] for 1 <= i <= 3
// 		               and p1[4] < p2[4]
	int i;

	for(i = 0; i < ((struct point*)a)->d; ++i)
	{
		if (((struct point*)a)->a[i] < ((struct point*)b)->a[i]) return -1;
		if (((struct point*)a)->a[i] > ((struct point*)b)->a[i]) return 1;
	}

	return 0;
}


int dominates(struct point p1, struct point p2)
{
// Checks if p1 dominates p2
// p1 dominates p2 when p1[i] >= p2[i] for all 1 <= i <= d
	int i;

	for(i = 0; i < p1.d; ++i)
		if (p1.a[i] < p2.a[i])
			return 0;

	return 1;
}


int equals(struct point p1, struct point p2)
{
// Checks if two points are equal.
// In other words, if pointcamp returns 0.
	return pointcmp(&p1, &p2) == 0;
}
