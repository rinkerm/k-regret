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

// Algorithm using the cube "strips" method
#include <vector>
#include <cmath>
#include <kregret/cube.h>

int cubealgorithm(size_t D, size_t N, int K, struct point *p, size_t L, int t, struct point *c, struct point *answer)
{
	int i, j, index, inCube, cubeBestIndex, done, seenBefore;
	
	std::vector<int>boundary(D,0);

	index = 0;
	// first list the maximal points in the directions {1,...,D}\L
	for(i = 0; i < D; ++i)
		if (i != L)
			answer[index++] = c[i];

	/*** Try all 0 <= j_1, j_2, \ldots < t ***/
	done = 0;
	while(!done && index < K)
	{
		// pick the maximal point in current cube
		cubeBestIndex = -1;
		for(i = 0; i < N; ++i)
		{
			// determine if p[i] is in this cube
			inCube = 1;
			for(j = 0; j < D && inCube; ++j)
				if (j != L)  // not the excluded dimension
					inCube = (boundary[j] * c[j].a[j] <= t * p[i].a[j]) && (t * p[i].a[j] < (boundary[j] + 1) * c[j].a[j]);

			if (inCube)  // check if it is maximal in the missing dimension
				if (cubeBestIndex < 0)
					cubeBestIndex = i;  // none seen yet, set to i
				else if(p[i].a[L] > p[cubeBestIndex].a[L])
						cubeBestIndex = i; // replace if larger in dimension L
		}

		// If there is a point in this cube and it is distinct from earlier ones, add to list
		if (cubeBestIndex >= 0)
		{
			seenBefore = 0;
			for(i = 0; i < index && !seenBefore; ++i)
				if(equals(answer[i], p[cubeBestIndex]))
					seenBefore = 1;

			if (!seenBefore)
				answer[index++] = p[cubeBestIndex];
		}


		// increment t-ary counter
		if (L == 0) // if we're skipping 0th dimension, start at 1
			j = 1;
		else
			j = 0;
		boundary[j]++;
		while(boundary[j] == t && !done)
		{
			boundary[j] = 0;  // reset position

			if (j == L - 1) j += 2;  // if we reach the skipped dimension, skip it
			else            j += 1;  // else increment by 1

			if (j < D)
				boundary[j]++; // if not at the end, do the carry
			else
				done = 1;      // else indicate that all combinations tried
		}
	}

	// fill in any remaining positions with the first point found
	for(i = index; i < K; ++i)
		answer[i] = answer[0];

	return index;
}

void cube(size_t D, size_t N, int K, struct point *p, int *maxIndex)
{
	int i, j, t, distinct;
	size_t L = D - 1;
	std::vector<point> c(D); // maximal points in each direction
	std::vector<point> answer(K + 1);

	// compute the maximal points in each of the D directions
	for(i = 0; i < D; ++i)
		c[i] = p[0]; // initialize with first point
	for(i = 0; i < N; ++i)
		for(j = 0; j < D; ++j)
			if (p[i].a[j] > c[j].a[j])
				c[j] = p[i];

	// initialize t as in the cube algorithm
	t = (int)pow(K - D + 1.0, 1.0/(D - 1.0));

	// keep looping until we find at least K distinct points
	do
	{
		distinct = cubealgorithm(D, N, K, p, L, t, c.data(), answer.data());
		t++;
	}
	while(distinct < K && distinct < N);

	if (distinct > K)
		cubealgorithm(D, N, K, p, L, t - 2, c.data(), answer.data());

	// get the indices, to be in the desired format
	for(i = 0; i < N; ++i)
		for(j = 0; j < K; ++j)
			if (equals(p[i], answer[j]))
				maxIndex[j] = i;
}
