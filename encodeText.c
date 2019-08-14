/**
*
* encodedText.c Copyright (C) 2018  Valentinos Pariza 
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
* This file contains the implementation of the encodeText method and all the 
* methods that this method uses as helper methods to accomplish its purpose.
*
*/
#include "utilities.h"
#include "cryptoUtilities.h"
#include "wavelib.h"

static const unsigned int SYSTEM_KEY_INTEGER = 8;

/**
 * @brief This method encodes a message in soundtrack(struct of type WAV)
 *
 * This method takes a pointer to a struct of type WAV and a pointer to a 
 * sequence of characters(pointer to characters) which indicates the content 
 * of the message.After these the message is encrypted inside the soundtrack 
 * based on an algorithm and also on a secret key ,which key is encapsulated 
 * inside the file encodedText.c . 
 *
 * @param a pointer to a struct of type WAV which is used to hide-encode inside
 *        a message
 *
 * @param a pointer to a sequence of characters that is the message to encode in
 *        the data of the struct of type WAV
 *
 *
 * @return EXIT_SUCCESSif the encoding was executed succesfully or EXIT_FAILURE if
 *         there was a problem during the execution of the method
 *
 * @author Valentinos Pariza
 */
PRIVATE int encodeTextWav(WAV* track, char* text);


/**
 * @brief This method reads a text from a text file
 *
 * This method takes the name of a text file and reads from it the text that is
 * contained in that text file. It places the text read to a place in memory
 * which address is passed as an argument.
 *
 * @param a name of a text file from it the message will be read
 * 
 * @param a pointer to a variable which stores a pointer to a sequence of
 *         characters that is the message to be read from a text(placed there
 *         after reading it from the file )
 *
 *
 * @return EXIT_SUCCESS if the reading was executed succesfully or EXIT_FAILURE 
 *         if there was a problem during the execution of the method
 *
 * @author Valentinos Pariza
 */
PRIVATE int readText(char* fileName, char** message);

PRIVATE int readText(char* fileName, char** message) {

	if (fileName == NULL || message == NULL)
		return EXIT_FAILURE;

	FILE* filePointer = fopen(fileName, "r");

	if (filePointer == NULL)
		return EXIT_FAILURE;

	char line[CHARACTERS_PER_LINE];
	(*message) = (char*) malloc(sizeof(char) * CHARACTERS_PER_LINE);

	if ((*message) == NULL)
		return EXIT_FAILURE;

	int size = 1;

	int maxSize = CHARACTERS_PER_LINE;
	char* temp = NULL;

	strcpy(*message, "\0");

	while (fgets(line, CHARACTERS_PER_LINE, filePointer) != NULL) {

		int length = strlen(line);

		if (size + length >= maxSize) {

			maxSize *= 2;
			temp = realloc((*message), maxSize * sizeof(char));

			if (temp == NULL) {
				free((*message));
				(*message) = NULL;
				return EXIT_FAILURE;
			}

			(*message) = temp;

		}

		strcat(*message, line);
		size += length;
	}

	temp = realloc(*message, size);

	if (temp == NULL) {
		free((*message));
		(*message) = NULL;
		return EXIT_FAILURE;
	}

	(*message) = temp;

	fclose(filePointer);

	return EXIT_SUCCESS;

}

PUBLIC int encodeText(char* fileName, char* textFile) {
	if (fileName == NULL || textFile == NULL)
		return EXIT_FAILURE;

	char* message = NULL;

	if (readText(textFile, &message) == EXIT_FAILURE)
		return EXIT_FAILURE;

	WAV* wav = NULL;

	if (readWAV(fileName, &wav) == EXIT_FAILURE) {
		free(message);
		return EXIT_FAILURE;
	}

	if (encodeTextWav(wav, message) == EXIT_FAILURE) {
		deleteWAV(&wav);
		free(message);

		return EXIT_FAILURE;
	}

	char* encryptedFileName = NULL;

	if (createOutputFilename(fileName, "new-",
			&encryptedFileName)==EXIT_FAILURE) {
		deleteWAV(&wav);
		free(message);

		return EXIT_FAILURE;
	}

	if (writeWAV(encryptedFileName, wav) == EXIT_FAILURE) {
		free(encryptedFileName);
		deleteWAV(&wav);
		free(message);

		return EXIT_FAILURE;
	}

	free(encryptedFileName);
	deleteWAV(&wav);
	free(message);

	printf("Success :  %s\t(Created)\n", encryptedFileName);
	return EXIT_SUCCESS;

}

PRIVATE int encodeTextWav(WAV* track, char* text) {
	if (track == NULL || text == NULL || track->header == NULL
			|| track->data == NULL)
		return EXIT_FAILURE;

	if (!isCorrectFormatWAV(track))
		return EXIT_FAILURE;

	int numberOfBitsText = (strlen(text) + 1) << 3;

	int bytesPerSample = ((track->header->BitsPerSample) >> 3);

	if ((unsigned int)numberOfBitsText > ((track->header->Subchunk2Size) / (unsigned int)bytesPerSample))
		return EXIT_FAILURE;

	int* permutation = createPermutationFunction(numberOfBitsText,
			SYSTEM_KEY_INTEGER);

	if (permutation == NULL)
		return EXIT_FAILURE;

	int i = 0;
	for (i = 0; i < numberOfBitsText; i++) 
   {
   
      // it takes the least significant bit of the least significant byte
      // of the signle channel block-sample (left or right respectevely)
 
      // zero the least significant bit
		track->data->channel[(permutation[i] + 1) * (bytesPerSample) - 1] &=(~1);

      // set the value of the least significant bit as indicated by the bit
      // taken from message
		track->data->channel[(permutation[i] + 1) * (bytesPerSample) - 1] |=
				getBit(text, i);
	}

	free(permutation);
	return EXIT_SUCCESS;
}

#ifdef DEBUG_ENCODE_TEXT


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
	char* textFileName=NULL;

	printf("Give the name of the fileName of the soundttack to read : \n");
	do {
		getline(&fileName,&bytes,stdin);
		cutStringAtFirstNewLineCharacter(fileName);

	}while(bytes<=0);

	printf("Give the name of the textfile which inside there is the message to be encrypted : \n");
	do {
		getline(&textFileName,&bytes,stdin);
		cutStringAtFirstNewLineCharacter(textFileName);

	}while(bytes<=0);

	if(encodeText(fileName, textFileName)==EXIT_FAILURE)
	{
		printf("One of the files couldn't be opened or there was a problem during"
				" encoding of the message ! \n");
	}
	else printf("The message was encrypted succesfully ! \n");

	free(fileName);
	free(textFileName);

	return 0;
}
#endif

