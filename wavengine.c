/**
 *  @file wavengine.c
 *  @brief A WAV file library editor
 *
 *  Program: wavengine  Copyright (C) 2018  Marios Pafitis & Valentinos Pariza
 *  This program comes with ABSOLUTELY NO WARRANTY;
 *  This is free software, and you are welcome to redistribute it under certain
 *  conditions;
 *
 *
 *  This is the customer, the user interface of the library wavelib.a. It controls the actions
 *  based on the input from the user. The general form of execution of the client is as follows:
 *  ./wavengine <-option> sound1.wav [sound2.wav sound3.wav …].
 *
 *  The option could be:
 *  1.	-list :
 *  	Displays the Header segment for each sound.wav file
 *  2.	-mono :
 *  	Converts a Stereo sound.wav into Mono. It keeps only the left channel of the original sound.
 *  	It creates an output file named new-[sound].wav.
 *  3.	-mix
 *  	Combines the left channel of the first sound with the right channel of the second sound.
 *  	It takes as input only two files and creates an output file named mix-[sound].wav.
 *	4.	-chop
 *		Cuts a sound.wav for a specific duration and creates an output file named chopped-[sound].wav
 *	5.	-reverse
 *		Reverses a sound.wav file. More specific it reverses the data of a sound.wav and creates an
 *		output file named reverse-[sound].wav
 *	6.	-similarity
 *		Finds the similarity distance between two sound.wav files. It calculates the distance by using
 *		the Euclidean Distance method and the LCSS Distance method. It displays the two distances.
 *	7.	-encodeText
 *		Encodes a text into a sound.wav in a way that you will not spot any difference and creates an
 *		output file named new-[sound].wav.
 *	8.	-decodeText
 *		Decodes a sound.wav that it has been encoded by using the method encodeText and recovers the
 *		text to the output file.
 *	9.	-merge
 *		Merges two sound.wav audio files. It adds the second audio file at the end of the first one
 *		and creates an output file named merge-[sound1]-[sound2].wav.
 *
 *	This system supports options for multiple input files. If you give the string *.wav as input
 *	filename for the option -list, -mono, -chop, -reverse, -endoceText and -merge it will execute
 *	the code for every single one .wav file in the directory.
 *
 *  @version 1.0
 *  @author Marios Pafitis
 *  @bugs No known bugs
 */
#include "wavelib.h"

#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#ifdef _WIN32
#define PATHSEPERATOR '\\'
#else
#define PATHSEPERATOR '/'
#endif

int main(int argc, char** argv) {
	printf(
			"\nProgram: wavengine Copyright (C) 2018 Marios Pafitis & Valentinos Pariza\n");
	printf("This program comes with ABSOLUTELY NO WARRANTY;\n");
	printf(
			"This is free software, and you are welcome to redistribute it under certain \n");
	printf("conditions; \n\n");
	printf(
			"If you want to see more information about the Copyrights of this program\n");
	printf("Run it again with the option command ./wavengine -gpl\n\n");
	int i;
	if (argc == 2 && strcmp(argv[1], "-gpl") == 0) {
		printGPL();
	} else if (argc < 3) {
		printf("\nWrong command format.\n\n");
	} else {
		if (strcmp(argv[1], "-list") == 0) { // 1: -list
			for (i = 2; i < argc; i++) {
				if (list(argv[i]) == EXIT_FAILURE) {
					//printf("\nThis is not a compatible .wav audio file.\n\n");
				}
			}
		} else if (strcmp(argv[1], "-mono") == 0) { // 2: -mono
			for (i = 2; i < argc; i++) {
				if (mono(argv[i]) == EXIT_FAILURE) {
					// printf("\nThis is not a compatible .wav audio file.\n\n");
				}
			}
		} else if (strcmp(argv[1], "-mix") == 0) { // 3: -mix
			if (argc != 4) {
				printf(
						"\nWrong command format. Give two audio files as input\n\n");
			} else {
				if (mix(argv[2], argv[3]) == EXIT_FAILURE) {
					//printf("\nThis is not a compatible .wav audio file.\n\n");
				}
			}
		} else if (strcmp(argv[1], "-chop") == 0) { // 4: -chop
			int l = atoi(argv[argc - 2]);
			int r = atoi(argv[argc - 1]);
			for (i = 2; i < argc - 2; i++) {
				if (chop(argv[i], l, r) == EXIT_FAILURE) {
					//printf("\nThis is not a compatible .wav audio file.\n\n");
				}
			}
		} else if (strcmp(argv[1], "-reverse") == 0) { // 5: -reverse
			for (i = 2; i < argc; i++) {
				if (reverse(argv[i]) == EXIT_FAILURE) {
					//("\nThis is not a compatible .wav audio file.\n\n");
				}
			}
		} else if (strcmp(argv[1], "-similarity") == 0) { // 6: -similarity
			if (argc != 4) {
				printf(
						"\nWrong command format. Give two audio files as input\n\n");
			} else {
				if (similarity(argv[2], argv[3]) == EXIT_FAILURE) {
					//printf("\nThis is not a compatible .wav audio file.\n\n");
				}
			}
		} else if (strcmp(argv[1], "-encodeText") == 0) { // 7: -encodeText
			for (i = 2; i < argc; i++) {
				if (encodeText(argv[i], argv[argc - 1]) == EXIT_FAILURE) {
					//printf("\nThis is not a compatible .wav audio file.\n\n");
				}
			}
		} else if (strcmp(argv[1], "-decodeText") == 0) { // 8: -decodeText
			if (argc != 5) {
				printf(
						"\nWrong command format. Give an audio file a message length and an output.txt as input\n\n");
			} else {
				int msgLength = atoi(argv[3]);
				if (decodeText(argv[2], msgLength, argv[4]) == EXIT_FAILURE) {
					printf("\nThis is not a compatible .wav audio file.\n\n");
				}
			}
		} else if (strcmp(argv[1], "-merge") == 0) { // Extra: -merge
			if (argc != 4) {
				printf(
						"\nWrong command format. Give two audio files as input\n\n");
			} else {
				if (merge(argv[2], argv[3]) == EXIT_FAILURE) {
					//printf("\nThis is not a compatible .wav audio file.\n\n");
				}
			}
		} else {
			printf("\nWrong command format.\n\n");

		}
	}
	return 0;
}

