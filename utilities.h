/**
 *  Program: wavengine  Copyright (C) 2018  Marios Pafitis & Valentinos Pariza
 *  This program comes with ABSOLUTELY NO WARRANTY;
 *  This is free software, and you are welcome to redistribute it under certain
 *  conditions;
 *
 */
#ifndef UTILITIES_H
#define UTILITIES_H

#ifdef _WIN32
#define PATHSEPERATOR '\\'
#else
#define PATHSEPERATOR '/'
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#define _GNU_SOURCE
#define CHARACTERS_PER_LINE 100

#define PUBLIC
#define PRIVATE static
#define CHUNKID_PREDEFINED_VALUE "RIFF"
#define FORMAT_PREDEFINED_VALUE "WAVE"
#define SUBCHUNK1ID_PREDEFINED_VALUE "fmt"
#define SUBCHUCK2ID_PREDEFINED_VALUE "data"
#define BIG_ENDIAN_FIELDS_BYTES 4


typedef unsigned char byte; // 1B
typedef unsigned short int word; // 2B
typedef unsigned int dword; // 4B

typedef struct {
	byte ChunkID[4];
	dword ChunkSize;
	byte Format[4];
	byte Subchunk1ID[4];
	dword Subchunk1Size;
	word AudioFormat;
	word NumChannels;
	dword SampleRate;
	dword ByteRate;
	word BlockAlign;
	word BitsPerSample;
	byte Subchunk2ID[4];
	dword Subchunk2Size;
}__attribute__((packed)) HEADER;

typedef struct {
	byte *channel;
}__attribute__((packed)) DATA;

typedef struct {
	HEADER *header;
	DATA *data;
}__attribute__((packed)) WAV;

/**
 * @brief Read WAV file
 *
 *	This function takes as input a filename type of .wav audio and returns a WAV struct.
 *	The WAV struct contains the header and the data of the input file. It checks if the
 *	filename is not NULL.
 *
 * 	@param *filename the filename of the WAV
 * 	@param **wav the pointer of a WAV struct
 * 	@return int Success or Failure
 *	@author Marios Pafitis
 * 	@bug No known bugs.
 */
PUBLIC int readWAV(char *filename, WAV **wav);
/**
 * @brief Write WAV file
 *
 *	This function takes as input a WAV struct and creates an output file of .wav.
 *	It checks if the filename and the wav are not NULL.
 *
 * 	@param *filename the filename othat I want to write
 * 	@param **wav the pointer of the WAV struct I want to write
 * 	@return int Success or Failure
 *	@author Marios Pafitis
 * 	@bug No known bugs.
 */
PUBLIC int writeWAV(char *filename, WAV *wav);
/**
 * @brief Read WAV file Header
 *
 *	This function takes as input a filename type of .wav audio and returns a HEADER struct.
 *	The HEADER struct contains the header of the input file. It checks if the filename is
 *	not NULL. It doesn't read the data of the WAV file.
 *
 * 	@param *filename the filename of the WAV
 * 	@param **header the pointer of a HEADER struct
 * 	@return int Success or Failure
 *	@author Marios Pafitis
 * 	@bug No known bugs.
 */
PUBLIC int readHeader(char *filename, HEADER **header);
/**
 * @brief Create Output Filename
 *
 *	This function creates output filename based on the index for one file. The output file is the
 *	has the form path + index + "-" + filename. It checks if the inFilename is not NULL.It returns
 *	the output filename as a pointer in the argument **outFilename.
 *
 * 	@param *inFilename the input filename of the WAV
 * 	@param *index the index for the specific output filename
 * 	@param **outFilename the output filename of the WAV
 * 	@return int Success or Failure
 *	@author Marios Pafitis
 * 	@bug No known bugs.
 */
PUBLIC int createOutputFilename(char *inFilename, char *index,
		char **outFilename);
/**
 * @brief Create Output Filename For Two Files
 *
 *	This function creates output filename based on the index for two files. The output file is the has
 *	the form path + index + "-" + filename1 + "-" + filename2. It checks if the inFilename1 and the
 *	inFilename2 are not NULL.It returns the output filename as a pointer in the argument **outFilename.
 *
 * 	@param *inFilename1 the input filename1 of the WAV
 * 	@param *inFilename2 the input filename2 of the WAV
 * 	@param *index the index for the specific output filename
 * 	@param **outFilename the output filename of the WAV
 * 	@return int Success or Failure
 *	@author Marios Pafitis
 * 	@bug No known bugs.
 */
PUBLIC int createOutputFilenameTwoFiles(char *inFilename1, char *inFilename2,
		char *index, char **outFilename);




/**
* @brief This method checks whether a struct of type WAV has a correct format of a .wav file
*
* This method checks whether a struct of type WAV contains actually information
* at header and data segment that is correct (correctness of the big endian 
* fields etc)and corresponds to an actual .wav file.Also it checks for specific
* range of values for its fields.For example it does not accept soundtracks with 
* more than two channels , and less than one channel and also it doesn't accepts
* a form of compressed files(These types of files aren't supported for keeping
* the method to the range of the requested method).Also it checks for the bitsPer
* Sample to be a multiple number of 8 (because we are dealing with bytes in memory
* .We suppose that the smallest unit of memory is a byte (This actually is applies))
* .Moreover we check whether the number of the data of the soundtrack is 
* actually a result from the information of the header(For example if the data 
* as bytes is multiple of the channels of the .wav file ) .For more information
* see the implementation
* 
* @param a pointer to a struct if type WAV (soundtrack)
*
* @return true if the struct of type WAV , pointed by the pointer passed as an 
*         argument has a correct format , otherwise false
*
* @author Valentinos Pariza 
*/
PUBLIC bool isCorrectFormatWAV(WAV *wav);


/**
* @brief This method deletes a WAV struct and frees its memory from heap
*
* This method takes as an argument a pointer to a variable which points to a 
* WAV struct and frees the memory that is used in heap by that struct. It
* also sets the value of the variable which was used , to hold the pointer to a WAV
* struct,after setting free the memory of the struct, to NULL in order to prevent 
* and warn anyone who will want to use that variable(warning that the variable 
* doesn't have an actual pointer inside).
*
* 
* @param a pointer to a variable that holds a pointer to a struct of type WAV
*
*
* @return EXIT_SUCCESSif the deletion was made succesfully or EXIT_FAILURE if
*         there was a problem during the deletion of the WAV struct 
*
* @author Valentinos Pariza 
*/
PUBLIC int deleteWAV(WAV** wav);

#endif
