/**
 *  Program: wavengine  Copyright (C) 2018  Marios Pafitis & Valentinos Pariza
 *  This program comes with ABSOLUTELY NO WARRANTY;
 *  This is free software, and you are welcome to redistribute it under certain
 *  conditions;
 *
 */
#include "utilities.h"

#define CHUNKID_PREDEFINED_VALUE "RIFF"
#define FORMAT_PREDEFINED_VALUE "WAVE"
#define SUBCHUNK1ID_PREDEFINED_VALUE "fmt"
#define SUBCHUCK2ID_PREDEFINED_VALUE "data"
#define BIG_ENDIAN_FIELDS_BYTES 4

PUBLIC void printGPL() {
	char c;
	printf(
			"\nProgram: wavengine Copyright (C) 2018 Marios Pafitis & Valentinos Pariza\n");
	printf(
			"This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.\n");
	printf("This is free software, and you are welcome to redistribute it\n");
	printf("under certain conditions; type `show c' for details.\n> ");
	scanf("show %c", &c);
	if (c == 'c') {
		printf(
				"\nProgram: wavengine Copyright (C) 2018 Marios Pafitis & Valentinos Pariza\n");
		printf("This program comes with ABSOLUTELY NO WARRANTY;\n");
		printf(
				"This is free software, and you are welcome to redistribute it under certain \n");
		printf("conditions; \n\n");
	} else if (c == 'w') {
		printf("\n@brief A WAV file library editor\n");
		printf("Copyright (C) 2018  Marios Pafitis & Valentinos Pariza\n\n\n");
		printf(
				"This program is free software: you can redistribute it and/or modify it under\n");
		printf(
				"the terms of the GNU General Public License as published by the Free Software\n");
		printf(
				"Foundation, either version 3 of the License, or at your option) any later version.\n\n\n");
		printf(
				"This program is distributed in the hope that it will be useful, but WITHOUT\n");
		printf(
				"ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS\n");
		printf(
				"FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.\n\n");
		printf(
				"Υou should have received a copy of the GNU General Public License along with\n");
		printf("this program.  If not, see <http://www.gnu.org/licenses/>.\n\n");
	}
}

PUBLIC int readWAV(char *filename, WAV **wav) { // Used for wav reading
	if (filename == NULL) {
		printf("Wrong input.\n");
		return EXIT_FAILURE;
	}
	FILE *fp = fopen(filename, "rb");
	if (fp == NULL) {
		printf("Can't open the file\n");
		return EXIT_FAILURE;
	}
	*wav = (WAV*) malloc(sizeof(WAV));
	if (*wav == NULL) {
		printf("Not enough space to allocate memory.\n");
		fclose(fp);
		return EXIT_FAILURE;
	}
	(*wav)->header = (HEADER*) malloc(sizeof(HEADER));
	if ((*wav)->header == NULL) {
		printf("Not enough space to allocate memory.\n");
		free(*wav);
		fclose(fp);
		return EXIT_FAILURE;
	}
	(*wav)->data = (DATA*) malloc(sizeof(DATA));
	if ((*wav)->data == NULL) {
		printf("Not enough space to allocate memory.\n");
		free((*wav)->header);
		free(*wav);
		fclose(fp);
		return EXIT_FAILURE;
	}
	// Read Header
	fread((*wav)->header, sizeof(HEADER), 1, fp);
	// Read data
	(*wav)->data->channel = (byte*) malloc((*wav)->header->Subchunk2Size);
	if ((*wav)->data->channel == NULL) {
		printf("Not enough space to allocate memory.\n");
		free((*wav)->data);
		free((*wav)->header);
		free(*wav);
		fclose(fp);
		return EXIT_FAILURE;
	}
	fread((*wav)->data->channel, sizeof(byte), (*wav)->header->Subchunk2Size,
			fp);
	fclose(fp);
	return EXIT_SUCCESS;
}

PUBLIC bool isCorrectFormatWAV(WAV *wav) {
	if (wav == NULL)
		return false;

	// check for the correctness of the big endian fields
	for (int i = 0; i < BIG_ENDIAN_FIELDS_BYTES; i++) {

		if (CHUNKID_PREDEFINED_VALUE[i] != wav->header->ChunkID[i]
				|| FORMAT_PREDEFINED_VALUE[i] != wav->header->Format[i]
				|| SUBCHUNK1ID_PREDEFINED_VALUE[i % 3]
						!= wav->header->Subchunk1ID[i % 3]
				|| SUBCHUCK2ID_PREDEFINED_VALUE[i]
						!= wav->header->Subchunk2ID[i])
			return false;
	}

	// Not supported number of channels
	if (wav->header->NumChannels != 1 && wav->header->NumChannels != 2)
		return false;

	// Not byte alligned memory for bits per sample .Unsupported number of bits
	// per sample
	if ((wav->header->BitsPerSample) % 8 != 0)
		return EXIT_FAILURE;

	// Check for Correctness of  number of bytes for channels and for correct
	// number of bytes for data field
	if (((wav->header->Subchunk2Size % wav->header->NumChannels) != 0)
			|| ((wav->header->Subchunk2Size / wav->header->NumChannels)
					% (wav->header->BitsPerSample / 8) != 0))
		return EXIT_FAILURE;

	// Not Supported option PCM!=1 --> a Form of compression isn't supported
	// in this library
	return wav->header->AudioFormat == 1;
}

PUBLIC int readHeader(char *filename, HEADER **header) { // Used for list
	if (filename == NULL) {
		printf("Wrong input.\n");
		return EXIT_FAILURE;
	}
	FILE *fp = fopen(filename, "rb");
	if (fp == NULL) {
		printf("Can't open the file\n");
		return EXIT_FAILURE;
	}
	*header = (HEADER*) malloc(sizeof(HEADER));
	if (*header == NULL) {
		printf("Not enough space to allocate memory.\n");
		return EXIT_FAILURE;
	}
	fread(*header, sizeof(HEADER), 1, fp);
	fclose(fp);
	return EXIT_SUCCESS;
}

PUBLIC int writeWAV(char *filename, WAV *wav) {
	if (filename == NULL || wav == NULL) {
		printf("Wrong input.\n");
		return EXIT_FAILURE;
	}
	FILE *fp = fopen(filename, "wb");
	if (fp == NULL) {
		printf("Can't create the file\n");
		return EXIT_FAILURE;
	}
	fwrite(wav->header, sizeof(HEADER), 1, fp);
	fwrite(wav->data->channel, sizeof(byte), wav->header->Subchunk2Size, fp);
	fclose(fp);
	return EXIT_SUCCESS;
}

PUBLIC int createOutputFilename(char *inFilename, char *index,
		char **outFilename) {
	if (inFilename == NULL || index == NULL) {
		return EXIT_FAILURE;
	}
	char *str = (char *) malloc(sizeof(char) * 512);
	// Copy the path
	strcpy(str, inFilename);
	// Get the name
	char *name = strrchr(str, PATHSEPERATOR);
	*outFilename = (char *) malloc(sizeof(char) * 512);
	if (outFilename == NULL) {
		printf("Not enough space to allocate memory.\n");
		free(str);
		return EXIT_FAILURE;
	}
	if (name != NULL) {
		str[strlen(str) - strlen(name)] = '\0';
		strcpy(*outFilename, str);
		name = name + 1; // Skip the slash
		if (PATHSEPERATOR == '/') {
			strcat(*outFilename, "/");
		} else {
			strcat(*outFilename, "\\");
		}
		strcat(*outFilename, index);
		strcat(*outFilename, name);
	} else {
		strcpy(*outFilename, index);
		strcat(*outFilename, str);
	}
	if (strstr(*outFilename, ".wav") == NULL) { // Add the extension if it doesn't exists
		strcat(*outFilename, ".wav");
	}
	free(str);
	return EXIT_SUCCESS;
}

PUBLIC int createOutputFilenameTwoFiles(char *inFilename1, char *inFilename2,
		char *index, char **outFilename) {
	char * temp1a = (char *) malloc(sizeof(char) * 512);
	if (temp1a == NULL) {
		printf("Not enough space to allocate memory.\n");
		return EXIT_FAILURE;
	}
	char * temp2a = (char *) malloc(sizeof(char) * 512);
	if (temp2a == NULL) {
		printf("Not enough space to allocate memory.\n");
		free(temp1a);
		return EXIT_FAILURE;
	}
	strcpy(temp1a, inFilename1);
	strcpy(temp2a, inFilename2);
	char * temp1 = strrchr(temp1a, PATHSEPERATOR);
	char * temp2 = strrchr(temp2a, PATHSEPERATOR);
	*outFilename = (char *) malloc(sizeof(char) * 512);
	if (outFilename == NULL) {
		printf("Not enough space to allocate memory.\n");
		free(temp1a);
		free(temp2a);
		return EXIT_FAILURE;
	}
	if (temp1 != NULL) {
		memcpy(*outFilename, temp1a, temp1 - temp1a);
		temp1 = temp1 + 1; // Skip the slash
		if (strstr(temp1, ".wav") != NULL) {
			temp1[strlen(temp1) - 4] = '\0'; // Remove the .wav
		}
		(*outFilename)[strlen(*outFilename)] = PATHSEPERATOR;
		strcat(*outFilename, index);
		strcat(*outFilename, temp1);
	} else {
		if (strstr(temp1a, ".wav") != NULL) {
			temp1a[strlen(temp1a) - 4] = '\0'; // Remove the .wav
		}
		strcpy(*outFilename, index);
		strcat(*outFilename, temp1a);
	}
	strcat(*outFilename, "-");
	if (temp2 != NULL) {
		temp2 = temp2 + 1; // Skip the slash
		strcat(*outFilename, temp2);
	} else {
		strcat(*outFilename, temp2a);
	}
	if (strstr(*outFilename, ".wav") == NULL) { // Add the extension if it doesn't exists
		strcat(*outFilename, ".wav");
	}
	free(temp1a);
	free(temp2a);
	return EXIT_SUCCESS;
}

PUBLIC int deleteWAV(WAV** wav) {
	if (*wav == NULL)
		return EXIT_FAILURE;

	if ((*wav)->header != NULL)
		free((*wav)->header);

	if ((*wav)->data != NULL) {
		if ((*wav)->data->channel != NULL)
			free((*wav)->data->channel);

		free((*wav)->data);

	}

	free(*wav);

	*wav = NULL;

	return EXIT_FAILURE;
}

#ifdef DEBUG
int main(int argc, char** argv) {
	if (argc >= 3 && strcmp(argv[1], "-mix") == 0) {
		char *outFilename = NULL;
		createOutputFilenameForMix(argv[2], argv[3], &outFilename);
		printf("\n%s\n\n", outFilename);
	}
	return 0;
}
#endif
