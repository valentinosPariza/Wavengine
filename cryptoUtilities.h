/**
*
* cryptoUtilities.h Copyright (C) 2018  Valentinos Pariza 
*
*
* This file is free software: you can redistribute it and/or modify it under 
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
* This file contains abstract the function prototypes of the functions in the
* file cryptoUtilities.c 
*
*
**/

#ifndef CRYPTO_UTILITIES_H
#define CRYPTO_UTILITIES_H
#include "utilities.h"


/**
 * @brief Returns the n-th bit of a sequence of bytes-characters
 *
 * This method takes sequence of bytes-characters and returns the n-th bit(as
 * indicated by parameter n)startig from the most significant bit of the most 
 * significant byte and end to the least signifivant bit of the least 
 * significant byte. 
 *
 * @param a pointer to a sequence of characters . A message-text.
 * 
 * @param an integer number which indicates the n-th bit of the sequence of bytes
 *        -characters ,startig from the most significant bit of the most significant
 *         byte and end to the least signifivant bit of the least significant byte 
 *
 *
 * @return the value of n-th the bit requested from the sequence of characters
 *         It can be 1 or 0
 *
 * @author Valentinos Pariza
 */
PUBLIC int getBit(char* m,int n);


/**
 * @brief Returns a pointer to a sequence of integers which indicate randomly 
 *       places to put the bits of a message
 *
 * This method takes an integer number for sum bits to hide and a system key
 * for encoding and creates a random array ,which contains for N different 
 *  bits(as indicated by parameter N) the he positions of single sample-channel
 *  blocks where the bits of a message will be hidden in
 * @param an integer number indicating the number of bits to place in a WAV file
 * 
 * @param an integer number which can be recognised as the secret key , on which
 *       the array given will be build based with
 *
 *
 * @return a pointer to a sequence of integers which are the positions of 
 *        single sample-channel blocks where the bits of a message will be 
 *        hidden in , or a NULL if the array hasn't been created
 *
 * @author Valentinos Pariza
 */
PUBLIC int* createPermutationFunction(int N,unsigned int systemkey);


/**
 * @brief Sets the n-th bit of a sequence of bytes-characters
 *
 * This method takes sequence of bytes-characters and sets the n-th bit(as
 * indicated by parameter n)startig from the most significant bit of the most 
 * significant byte and end to the least signifivant bit of the least 
 * significant byte. 
 *
 * @param a pointer to a sequence of characters . A message-text.
 * 
 * @param an integer number which indicates the n-th bit of the sequence of bytes
 *        -characters ,startig from the most significant bit of the most significant
 *         byte and end to the least signifivant bit of the least significant byte 
 *
 * @param The value of bit to set (1 or 0)
 *
 * @return EXIT_SUCCESS if the bit was set succesfully or EXIT_FAILURE 
 *         if there was a problem during the execution of the method
 *
 * @author Valentinos Pariza
 */
PUBLIC int setBit(char* m,int n,int value);

#endif
