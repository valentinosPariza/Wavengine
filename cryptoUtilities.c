/**
*
* cryptoUtilities.c Copyright (C) 2018  Valentinos Pariza 
*
*
* This program is free software: you can redistribute it and/or modify it under 
* the terms of the GNU General Public License as published by the Free Software
* Foundation, either version 3 of the License, or at your option) any later version. 
*
*
* This program is distributed in the hope that it will be useful, but WITHOUT 
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details. 
*
* Υou should have received a copy of the GNU General Public License along with 
* this program.  If not, see <http://www.gnu.org/licenses/>.
*
*
* This file contains the implementation of some methods which are uses by
* decodedText and / or encodedText and all these methods use the methods of this  
* file as helper methods to accomplish their purpose.
*
*/
#include "cryptoUtilities.h"


PUBLIC int getBit(char* m, int n) {

	if (m == NULL || n < 0)
		return 0;

	int bytes = n >> 3;

	while (*m != '\0' && bytes != 0) {
		bytes--;
		m++;
	}

	if (bytes != 0)
		return 0;

	int bits = (7 - (n - ((n >> 3) << 3)));

	return ((int) (*m)) >> bits & 1;

}




PUBLIC int setBit(char* m, int n, int value) {
	if ((value != 0 && value != 1) || m == NULL || n < 0)
		return EXIT_FAILURE;

	int bytes = n >> 3;

	while (m != '\0' && bytes != 0) {
		bytes--;
		m++;
	}

	if (bytes != 0)
		return EXIT_FAILURE;

	int bits = 7 - (n - ((n >> 3) << 3));

	(*m) &= ~(1 << bits);
	(*m) |= (value << bits);

	return EXIT_SUCCESS;

}

PUBLIC int* createPermutationFunction(int N, unsigned int systemkey) {
	if (N <= 0)
		return NULL;

	int* permutation = (int*) malloc(sizeof(int) * N);

	if (permutation == NULL)
		return NULL;

	int i = 0;

	for (i = 0; i < N; i++)
		permutation[i] = i;

	srand(systemkey);

	int a = 0, b = 0, temp = 0;

	for (i = 0; i < N; i++) {
		a = rand() % N;
		b = rand() % N;

		temp = permutation[a];
		permutation[a] = permutation[b];
		permutation[b] = temp;

	}

	return permutation;
}

