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
* This file implements the method decodedText
*/
#include "utilities.h"
#include "cryptoUtilities.h"

// The secret system key of the encoding
PRIVATE const unsigned int SYSTEM_KEY_INTEGER = 8;


PRIVATE int writeMessageToTextFile(char* filename, char* message);

/**
 * @brief This method decodes a message from a soundtrack 
 *
 * This method takes an integer which is the length of the string to
 * decode ,a pointer struct of type WAV from where to derive the encoded message
 * and also a pointer to a variable where the method places the decoded message
 * .It takes the struct WAV and derives the message from it ,with an algorithm
 * for decoding ,based on a secret key (which is encapsulated inside the code).
 *
 *
 * @param a pointer to a struct of type WAV ,which contains the encode message
 *
 * @param an integer which is the length of the string to be decoded from the
 *        sound tract ,without including the NUL (='\0') character
 *
 * @param a pointer a variable which variable can store a pointer to characters
 *        in memory. There the address of the decocded message will be placed.
 *
 * @return EXIT_SUCCESS if the decoding was executed successfully or EXIT_FAILURE if
 *         there was a problem during the execution of the method
 *
 * @author Valentinos Pariza
 */
PRIVATE int decodeTextFromWav(WAV* track, int length, char** decodedText);


PUBLIC int decodeText(char* fileName, int length,char* outputFile) {
	if (fileName == NULL || length < 0 || outputFile==NULL)
		return EXIT_FAILURE;

	char* decodedText = NULL;
	WAV* wav = NULL;

	if (readWAV(fileName, &wav) == EXIT_FAILURE)
		return EXIT_FAILURE;

	// Plus one for the NUL character
	if (decodeTextFromWav(wav, length, &decodedText) == EXIT_FAILURE) {
		deleteWAV(&wav);

		return EXIT_FAILURE;
	}


	if (writeMessageToTextFile(outputFile, decodedText) == EXIT_FAILURE) {

      if(decodedText!=NULL)
         free(decodedText);

		deleteWAV(&wav);
		return EXIT_FAILURE;
	}

	deleteWAV(&wav);

   if(decodedText!=NULL)
     free(decodedText);

	return EXIT_SUCCESS;
}


/**
 * @brief This method writes a message/text to a text file
 *
 * This method takes the file name of a text file and a text/message (pointer
 * to a sequence of characters) and writes this text to the file given.
 *
 *
 * @param a pointer to characters which is the name of a text file
 *
 * @param a pointer to characters which is the message to write in a text file
 *
 *
 * @return EXIT_SUCCESS if the writing was executed successfully or EXIT_FAILURE 
 *         if there was a problem during the execution of the method
 *
 * @author Valentinos Pariza
 */
PRIVATE int writeMessageToTextFile(char* fileName, char* message) {
	if (fileName == NULL || message == NULL)
		return EXIT_FAILURE;

	FILE* filePointer = fopen(fileName, "w");

	if (filePointer == NULL)
		return EXIT_FAILURE;

	fprintf(filePointer, "%s", message);

	fclose(filePointer);

	return EXIT_SUCCESS;
}


PRIVATE int decodeTextFromWav(WAV* track, int length, char** decodedText) 
{
	if (length < 0 || decodedText == NULL)
		return EXIT_FAILURE;

	if (!isCorrectFormatWAV(track))
		return EXIT_FAILURE;

   length+=2;

	if ((unsigned int)length> ((track->header->Subchunk2Size) << 3)/ (track->header->BitsPerSample))
		return EXIT_FAILURE;


	int bits = length << 3;

	int* permutation = createPermutationFunction(bits, SYSTEM_KEY_INTEGER);

	if (permutation == NULL)
		return EXIT_FAILURE;

	*decodedText = (char*) malloc(sizeof(char) * (length));

	if (*decodedText == NULL) {
		free(permutation);
		return EXIT_FAILURE;

	}

	int bytesPerSample = ((track->header->BitsPerSample) >> 3);
	int value = 0;
   int i=0;

	for (i = 0; i < bits; i++) {

		// Find the LSB of the single smaple(left or right)
		value = track->data->channel[(permutation[i] + 1) * bytesPerSample - 1]& 1;
		// value=track->data->channel[permutation[i]]&1;

		setBit(*decodedText, i, value);

	}

   free(permutation);
	return EXIT_SUCCESS;

}


#ifdef DEBUG_DECODE_TEXT

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
      return ;

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
   int length=0;   

   

   printf("Give the name of the fileName of the soundttack to read : \n");
     do{
         getline(&fileName,&bytes,stdin);
         cutStringAtFirstNewLineCharacter(fileName);
         
      }while(bytes<=0);



   printf("Give the length of the message to read : \n");
   do{      
      getline(&textFileName,&bytes,stdin);
      cutStringAtFirstNewLineCharacter(textFileName);
      length=atoi(textFileName);
     }
    while(length<=0);
   
	 printf("Give the name of the textfile ,to write there the decoded message : \n");
     do{
         getline(&textFileName,&bytes,stdin);
         cutStringAtFirstNewLineCharacter(textFileName);
         
      }while(bytes<=0);
	
   
   if(decodeText(fileName,length,textFileName)==EXIT_FAILURE)	
   {
      printf("One of the files couldn't be opened or there was a problem during" 
      " decoding of the message ! \n");
   }
   else printf("The message was decoded succesfully ! \n");


   free(fileName);
   free(textFileName);

	return 0;
}
#endif

