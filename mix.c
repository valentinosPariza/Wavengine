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
*
* This file contains the code-implementation of the method of mixing to
* soundtracks
*
*/

#include "utilities.h"

#define min(a,b) ((a<b) ? (a) : (b))



/**
 * @brief Checks if the two structs of type WAV are compatile for mixing
 *
 * This method checks whether two structs of type WAV are compatible for mixing
 * by checking their headers.
 *
 *
 * @param a pointer to a struct of type WAV which indicates the first sound track
 *        
 * @param a pointer to a struct of type WAV which indicates the second sound track
 *         
 *@return true if the two structs of type WAV are compatible for mixing ,otherwise
 *        false indicating that the two WAV structs are not compatible for mixing
 *
 *@author Valentinos Pariza
 */
PRIVATE bool areCompatible(WAV* track1, WAV* track2);


/**
* @brief Mixes two soundtracks indicated by two pointers of WAV structs ,creating a new WAV struct
*
* This method takes two pointers to structs of type WAV,that contain inforamtion in 
* a header section and data in a data section about the soundtrack. . By saying 
* "mix the two soundtracks " ,it means to take the two soundtracks and put the
* first one to the right channel and the second one to the left channel. More
* specific it takes the left channel of the second soundtrack,if it is stereo, 
* and places it to the left channel of the new soundtrack(if it is mono it takes
* its single channel) and if the first one is stereo,it takes the right channel
* of it and places it to the right channel of the new soundtrack (if it is mono
* it takes the single channel of it and places it to the right channel of the 
* new soundtrack ). The new soundtrack created is placed in a place in memory
* which address is passed as an argument.

* 
* @param a pointer to a struct of type WAV which indicates the first sound track
*         to be mixed
* @param a pointer to a struct of type WAV which indicates the second sound track
*         to be mixed
*
* @param a pointer to a variable which points to a struct of type WAV which 
*         indicates the final mixed soundtract (struct of type WAV)
*
* @return EXIT_SUCCESSif the mix was executed succesfully or EXIT_FAILURE if
*         there was a problem during the execution of the method 
*         
* @author Valentinos Pariza 
*/
PRIVATE int mixWav(WAV* track1, WAV* track2, WAV** newTrack) ;




PUBLIC int mix(char* fileName1, char* fileName2) {
	if (fileName1 == NULL || fileName2 == NULL)
		return EXIT_FAILURE;

	WAV* wav1 = NULL;
	WAV* wav2 = NULL;

	if (readWAV(fileName1, &wav1) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	if (readWAV(fileName2, &wav2) == EXIT_FAILURE) {
		deleteWAV(&wav1);
		return EXIT_FAILURE;
	}

	WAV* newSoundTrack = NULL;

	if (mixWav(wav1, wav2, &newSoundTrack) == EXIT_FAILURE) {
		deleteWAV(&wav1);
		deleteWAV(&wav2);
		return EXIT_FAILURE;
	}

	char* newFileName = NULL;

	if (createOutputFilenameTwoFiles(fileName1, fileName2, "mix-",
			&newFileName)==EXIT_FAILURE) {

		deleteWAV(&wav1);
		deleteWAV(&wav2);
		deleteWAV(&newSoundTrack);
		return EXIT_FAILURE;
	}

	if (writeWAV(newFileName, newSoundTrack) == EXIT_FAILURE) {

		free(newFileName);
		deleteWAV(&wav1);
		deleteWAV(&wav2);
		deleteWAV(&newSoundTrack);

		return EXIT_FAILURE;
	}

	printf(
			"\n\nThe mixed soundtrack takeen from filrs %s and %s was saved in file"
					" with name : %s.\n", fileName1, fileName2, newFileName);

	free(newFileName);

	// free(newSoundTrack);
	deleteWAV(&newSoundTrack);

	deleteWAV(&wav1);

	deleteWAV(&wav2);

	return EXIT_SUCCESS;

}


PRIVATE bool areCompatible(WAV* track1, WAV* track2) {
	if (track1 == NULL || track2 == NULL || track1->header == NULL
			|| track2->header == NULL)
		return false;

	if (track1->header->BitsPerSample != track2->header->BitsPerSample
			|| track1->header->SampleRate != track2->header->SampleRate
			|| track1->header->BitsPerSample != track2->header->BitsPerSample)
		return false;

	return true;
}




PRIVATE int mixWav(WAV* track1, WAV* track2, WAV** newTrack) {

	// Check for actual pointers,which were given
	if (track1 == NULL || track2 == NULL || track1->header == NULL
			|| newTrack == NULL || track2->header == NULL
			|| track1->data == NULL || track2->data == NULL)
		return EXIT_FAILURE;

	// Check of the type of the track-wav to be sure that the format of the
	// file is correct
	if (!isCorrectFormatWAV(track1) || !isCorrectFormatWAV(track2))
		return EXIT_FAILURE;

	// check for compatibility of the two tracks given
	if (!areCompatible(track1, track2))
		return EXIT_FAILURE;

	// Find the needed bytes for each block of samples with channels(including the
	// extra samples for the added channels)
	int bytesPerSampleForTrack1 = ((track1->header->BitsPerSample) >> 3)
			* (track1->header->NumChannels);
	int bytesPerSampleForTrack2 = ((track2->header->BitsPerSample) >> 3)
			* (track2->header->NumChannels);

	long int minNumberOfSamples = 0;

	{
		int numSamples1 = (track1->header->Subchunk2Size)
				/ bytesPerSampleForTrack1;

		int numSamples2 = (track2->header->Subchunk2Size)
				/ bytesPerSampleForTrack2;

		minNumberOfSamples = min((numSamples1), (numSamples2));

	}

	// The number of bytes for a single unit of sample
	size_t bytesSingleUnitSample = ((track1->header->BitsPerSample) >> 3);

	*newTrack = (WAV*) malloc(sizeof(WAV));

	if ((*newTrack) == NULL)
		return EXIT_FAILURE;

	(*newTrack)->header = (HEADER*) malloc(sizeof(HEADER));

	if ((*newTrack)->header == NULL) {
		free(*newTrack);
		*newTrack = NULL;

		return EXIT_FAILURE;
	}

	memcpy((*newTrack)->header, track1->header, sizeof(HEADER));

	// update the number of channels on the new track
	(*newTrack)->header->NumChannels = 2;

	// Update the field ChunkSize of the header of teh new track of type WAV
	(*newTrack)->header->ChunkSize = sizeof(HEADER) - 8
			+ minNumberOfSamples * (*newTrack)->header->NumChannels
					* bytesSingleUnitSample;

	// update the ByteRate field of the header of the new track struct of type WAV
	(*newTrack)->header->ByteRate = (track1->header->ByteRate
			/ track1->header->NumChannels) * (*newTrack)->header->NumChannels;

	// update the BlockAlign field of the header of the new track struct of type WAV
	(*newTrack)->header->BlockAlign = (track1->header->BlockAlign
			/ track1->header->NumChannels) * (*newTrack)->header->NumChannels;

	// update the size in bytes of all the data of the new track struct of type WAV
	(*newTrack)->header->Subchunk2Size = minNumberOfSamples
			* (*newTrack)->header->NumChannels * bytesSingleUnitSample;

	(*newTrack)->data = (DATA*) malloc(sizeof(DATA));

	if ((*newTrack)->data == NULL) {
		free((*newTrack)->header);
		free(*newTrack);
		*newTrack = NULL;
		return EXIT_FAILURE;
	}

	// Allocate memory for the data field of the new track struct of type WAV
	(*newTrack)->data->channel = (byte*) malloc(
			sizeof(byte) * (*newTrack)->header->Subchunk2Size);

	if ((*newTrack)->data == NULL) {
		free((*newTrack)->header);
		free((*newTrack)->data);
		free(*newTrack);
		*newTrack = NULL;

		return EXIT_FAILURE;
	}

	// if the file track1 is stereo take the right channel to start ,else take the
	// single unit
	byte* pter1 =
			((track1->header->NumChannels) == 2) ?
					((track1->data->channel) + bytesSingleUnitSample) :
					(track1->data->channel);

	// if the file track2 is stereo take the left channel to start ,else take the
	// single unit, which is aso the left channel
	byte* pter2 = (track2->data->channel);

	byte* pointerToNewData = (*newTrack)->data->channel;
	int i;
	for (i = 0; i < minNumberOfSamples; i++) {
		// Copy the first single sample unit from track2 (it will be placed left)
		memcpy(pointerToNewData, pter2, bytesSingleUnitSample);

		// increase the position of the newTrack pointer to data by a number of
		// bytes indicated by the variable bytesSingleUnitSample.Moving a single
		// unit .In order to move to the right channel of the sample
		pointerToNewData += bytesSingleUnitSample;

		memcpy(pointerToNewData, pter1, bytesSingleUnitSample);

		// increase the position of the newTrack pointer to data by a number of
		// bytes indicated by the variable bytesSingleUnitSample.Moving a single
		// unit .In order to move to the left channel of the next sample
		pointerToNewData += bytesSingleUnitSample;

		// increase pointer of the two tracks in order to point to the next
		// segement of their sample
		pter1 += bytesPerSampleForTrack1;
		pter2 += bytesPerSampleForTrack2;

	}

	return EXIT_SUCCESS;
}


#ifdef DEBUG_MIX

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
	char* fileName1=NULL;
	char* fileName2=NULL;

	printf("Give the name of the fileName 1 of the soundttack to read : \n");
	do {
		getline(&fileName1,&bytes,stdin);
		cutStringAtFirstNewLineCharacter(fileName1);

   
   
   printf("Give the name of the fileName 1 of the soundttack to read : \n");
     do{
         getline(&fileName1,&bytes,stdin);
         cutStringAtFirstNewLineCharacter(fileName1);

	printf("Give the name of the fileName 2 of the soundttack to read : \n");
	do {
		getline(&fileName2,&bytes,stdin);
		cutStringAtFirstNewLineCharacter(fileName2);

	}while(bytes<=0);

	if(mix(fileName1, fileName2)==EXIT_FAILURE)
	{
		printf("\nOne of the files couldn't be opened or there was a problem during"
				" encoding of the message ! \n");
	}
	else printf("\nThe soundtracks from the two files were succesfully mixed! \n");

	free(fileName1);
	free(fileName2);

	return 0;
}
#endif
