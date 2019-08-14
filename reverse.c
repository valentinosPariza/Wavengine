/**
*
* decodedText.c Copyright (C) 2018  Valentinos Pariza 
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
* This file contains the implementation-code of the method that reverses a
* soundtrack
*
*/
#include "utilities.h"



/**
 * @brief This method reverses a struct of type WAV (sound track)
 *
 * This method  reverses the segment data of a struct of type WAV, so
 * the first sample to be last ,the second to be before last ,and so on. After
 * creating the reversed sound track , the struct created (of type WAV) is 
 * placed in a place in memory (variable) which address of that place is given
 * as a parameter.
 *
 * @param a pointer to a struct of type WAV which is the sound track to reverse
 *
 * @return EXIT_SUCCESSif the reverse-method was executed successfully or
 *         EXIT_FAILURE if there was a problem during the execution of the method
 * @author Valentinos Pariza
 */
PRIVATE int reverseWav(WAV* soundtrack, WAV** reversedSoundTrack);


PUBLIC int reverse(char* fileName) {
	if (fileName == NULL)
		return EXIT_FAILURE;

	WAV* wav = NULL;

	if (readWAV(fileName, &wav) == EXIT_FAILURE)
		return EXIT_FAILURE;
	WAV* reversedWAV = NULL;
	if (reverseWav(wav, &reversedWAV) == EXIT_FAILURE) {
		deleteWAV(&wav);
		return EXIT_SUCCESS;
	}
	char* destinationFileName = NULL;

	char index[9] = "reverse-";

	if (createOutputFilename(fileName, index,
			&destinationFileName)==EXIT_FAILURE
			|| writeWAV(destinationFileName, reversedWAV)==EXIT_FAILURE) {
		free(destinationFileName);
		deleteWAV(&reversedWAV);
		deleteWAV(&wav);
		return EXIT_FAILURE;
	}
	printf("The reversed soundtrack taken from file %s has been saved to"
			" file : %s.\n", fileName, destinationFileName);

	free(destinationFileName);
	deleteWAV(&wav);
	deleteWAV(&reversedWAV);

	return EXIT_SUCCESS;
}



PRIVATE int reverseWav(WAV* soundtrack, WAV** reversedSoundTrack) {
	if (soundtrack == NULL || reversedSoundTrack == NULL)
		return EXIT_FAILURE;
	if (!isCorrectFormatWAV(soundtrack))
		return EXIT_FAILURE;

	long sampleBlockBytes = soundtrack->header->BlockAlign;

	(*reversedSoundTrack) = (WAV*) malloc(sizeof(WAV));

	if ((*reversedSoundTrack) == NULL)
		return EXIT_FAILURE;

	(*reversedSoundTrack)->header = (HEADER*) malloc(sizeof(HEADER));

	if ((*reversedSoundTrack)->header == NULL) {
		free((*reversedSoundTrack));
		(*reversedSoundTrack) = NULL;
		return EXIT_FAILURE;
	}

	(*reversedSoundTrack)->data = (DATA*) malloc(sizeof(DATA));

	if ((*reversedSoundTrack)->data == NULL) {
		free((*reversedSoundTrack)->header);
		free((*reversedSoundTrack));
		(*reversedSoundTrack) = NULL;

		return EXIT_FAILURE;
	}

	(*reversedSoundTrack)->data->channel = (byte*) malloc(
			soundtrack->header->Subchunk2Size);

	if ((*reversedSoundTrack)->data->channel == NULL) {
		free((*reversedSoundTrack)->header);
		free((*reversedSoundTrack)->data);
		free((*reversedSoundTrack));
		(*reversedSoundTrack) = NULL;

		return EXIT_FAILURE;
	}

	memcpy((*reversedSoundTrack)->header, soundtrack->header, sizeof(HEADER));

	long numberOfSamples = (soundtrack->header->Subchunk2Size << 3)
			/ (soundtrack->header->BitsPerSample
					* soundtrack->header->NumChannels);

	byte* pointerSrc = (soundtrack->data->channel)
			+ soundtrack->header->Subchunk2Size - sampleBlockBytes;
	byte* pointerDest = (*reversedSoundTrack)->data->channel;

	for (long i = 0; i < (numberOfSamples); i++) {
		memcpy(pointerDest, pointerSrc, sampleBlockBytes);
		pointerSrc -= sampleBlockBytes;
		pointerDest += sampleBlockBytes;
	}

	return EXIT_SUCCESS;
}

#ifdef DEBUG_REVERSE
#define _GNU_SOURCE




/**
*@brief Takes a pointer to a sequence of characters and cuts the sequence of 
*        characters to the first new line character
*
* This method takes a pointer to characters and places at the first occurence of
* a new line character a NUL character (='\0')
*
*
*@param a pointer to a sequence of characters 
*
*@return void
*
*@author Valentinos Pariza
*/
PRIVATE void cutStringAtFirstNewLineCharacter(char* charsequence);


PRIVATE void cutStringAtFirstNewLineCharacter(char* charsequence)
{
	if(charsequence==NULL )
	return;

	char* pointerToCharacter=charsequence;

	while(*pointerToCharacter!='\0')
	{
		if(*pointerToCharacter=='\n')
		{
			*pointerToCharacter='\0';
			break;
		}

		pointerToCharacter++;
	}

}



int main(int argc,char* argv[])
{

	size_t bytes=0;
	char*line=NULL;
	char* fileName=NULL;

	printf("Give the name of the fileName  of the soundttack to read : \n");
	do {
		getline(&fileName,&bytes,stdin);
		cutStringAtFirstNewLineCharacter(fileName);

	}while(bytes<=0);

	if(reverse(fileName)==EXIT_FAILURE)
	{
		printf("\nOne of the files couldn't be opened or there was a problem during"
				" encoding of the message ! \n");
	}
	else printf("\nThe soundtrack was reversed! \n");

	free(fileName);
	return 0;
}
#endif

