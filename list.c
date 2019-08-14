/**
 *  Program: wavengine  Copyright (C) 2018  Marios Pafitis & Valentinos Pariza
 *  This program comes with ABSOLUTELY NO WARRANTY;
 *  This is free software, and you are welcome to redistribute it under certain
 *  conditions;
 *  @file list.c
 *  @brief Displays the header of a .wav file
 *
 *  Implements the list method in .wav files that it receives as input. It takes as input a
 *  .wav and presents a list of the file's characteristics in the display. Basically the
 *  output is the Header of the .wav file.
 *
 *  @version 1.0
 *  @author Marios Pafitis
 *  @bugs No known bugs
 */
#include "utilities.h"

/* @brief Add the NUL character at the 5th position of a string
 *
 * A function that adds the NUL character at the 5th position of a string
 *
 * @param in the input string
 * @return char * the output string
 * @author Marios Pafitis
 *
 */
PRIVATE char * printStr(char *);

PUBLIC int list(char *filename) {
// Read Data
	HEADER *header = NULL;
	if (readHeader(filename, &header) == EXIT_FAILURE) {
		printf("Fail    :  %s\t(Can't read Header)\n", filename);
		return EXIT_FAILURE;
	}
// Display Data
	printf("**********************************\n");
	printf("RIFF_CHUNK_HEADER\n");
	printf("==================\n");
	printf("chunkID: %s\n", printStr((char *) header->ChunkID));
	printf("chunkSize: %d\n", header->ChunkSize);
	printf("format: %s\n", printStr((char *) header->Format));
	printf("\nFMT_SUBCHUNK_HEADER\n");
	printf("==================\n");
	printf("subchunk1ID: %s\n", printStr((char *) header->Subchunk1ID));
	printf("subchunk1Size: %d\n", header->Subchunk1Size);
	printf("audioFormat: %hu\n", header->AudioFormat);
	printf("numChannels: %hu\n", header->NumChannels);
	printf("sampleRate: %d\n", header->SampleRate);
	printf("byteRate: %d\n", header->ByteRate);
	printf("blockAlign: %hu\n", header->BlockAlign);
	printf("bitsPerSample: %hu\n", header->BitsPerSample);
	printf("\nDATA_SUBCHUNK_HEADER\n");
	printf("==================\n");
	printf("subchunk2ID: %s\n", printStr((char *) header->Subchunk2ID));
	printf("subchunk2Size: %d\n", header->Subchunk2Size);
	printf("\n");
// Free mallocs
	free(header);
	return EXIT_SUCCESS;
}

PRIVATE char * printStr(char *in) {
	static char str[4];
	strcpy(str, in);
	str[4] = '\0';
	return str;
}

#ifdef DEBUG_LIST
// Test list
int main(int argc,char* argv[]) {
	printf("Test printStr: %s\n\n",printStr("HELLO12345"));
	if (strcmp(argv[1], "-list") == 0) {
		for (i = 2; i < argc; i++) {
			if (list(argv[i]) == EXIT_FAILURE) {
				printf("This is not a compatible .wav audio file.\n");
				return EXIT_FAILURE;
			}
		}
	}
	return 0;
}
#endif
