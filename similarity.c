/**
 *  Program: wavengine  Copyright (C) 2018  Marios Pafitis & Valentinos Pariza
 *  This program comes with ABSOLUTELY NO WARRANTY;
 *  This is free software, and you are welcome to redistribute it under certain
 *  conditions;
 *  @file similarity.c
 *  @brief Checks the similarity between two audio files.
 *
 *  Implements the mono method in .wav files that it receives as input. It takes as input two
 *  .wav audio files. It checks the similarity between the two files based two methods. THe first
 *  method is the Euclidean Distance a method really fast but inaccurate. The other method is the
 *  LCSS Distance, a more accurate version but very slow. It works properly but it takes a lot of time.
 *  There are other solving methods such as the LCSS which is using recursion but this one is even
 *  more slow. FInally, at the end the method represents the Euclidean Distance and the LCSS Distance.
 *
 *  @version 1.0
 *  @author Marios Pafitis
 *  @bugs No known bugs
 */
#include "utilities.h"
#ifdef DEBUG_SIMILARITY
#include <limits.h>
#endif
/**
 * @brief Calculate Euclidean Distance
 *
 *  Calculates Euclidean Distance Between two audio files. A really fast method to check
 *  if two audio files are the same, but you can't really trust it because it is inaccurate.
 *  As they say in my village "The good thing, always is late."
 *
 * 	@param *wav1 the input1 WAV struct
 * 	@param *wav2 the input2 WAV struct
 * 	@return double the Euclidean Distance
 *	@author Marios Pafitis
 * 	@bug No known bugs.
 */
PRIVATE double euclideanDistance(WAV *, WAV *);
/**
 * @brief Calculate LCSS Distance
 *
 *  Calculates LCSS Distance Between two audio files. It uses a non recursive method.
 *  This method uses a two dimensional array and it is very slow and very memory demanding
 *  so be patient.
 *
 * 	@param *wav1 the input1 WAV struct
 * 	@param *wav2 the input2 WAV struct
 * 	@return double the LCSS Distance
 *	@author Marios Pafitis
 * 	@bug No known bugs.
 */
PRIVATE double LCSSDistance(WAV *, WAV *);

PUBLIC int similarity(char *filename1, char* filename2) {
// Read WAV 1
	WAV *wav1 = NULL;
	if (readWAV(filename1, &wav1) == EXIT_FAILURE) {
		printf("Fail    :  %s\t(Can't read WAV file)\n", filename1);
		return EXIT_FAILURE;
	}
// Read WAV 2
	WAV *wav2 = NULL;
	if (readWAV(filename2, &wav2) == EXIT_FAILURE) {
		printf("Fail    :  %s\t(Can't read WAV file)\n", filename2);
		return EXIT_FAILURE;
	}
// Check if there are aligned
	if (wav1->header->AudioFormat != wav2->header->AudioFormat
			|| wav1->header->NumChannels != wav2->header->NumChannels
			|| wav1->header->SampleRate != wav2->header->SampleRate
			|| wav1->header->ByteRate != wav2->header->ByteRate
			|| wav1->header->BlockAlign != wav2->header->BlockAlign
			|| wav1->header->BitsPerSample != wav2->header->BitsPerSample) {
		printf("Fail    :  %s $ %s\t(The two audio files are not align)\n",
				filename1, filename2);
		deleteWAV(&wav1);
		deleteWAV(&wav2);
		return EXIT_FAILURE;
	}
	printf("\nCompare:\n%s\nwith:\n%s\n\n", filename1, filename2);
// Calculate Euclidean Distance
	printf("Euclidean distance: ");
	printf("%.3f\n", euclideanDistance(wav1, wav2));
	printf("LCSS distance: ");
	double LCSS = LCSSDistance(wav1, wav2);
	if (LCSS == -1) {
		deleteWAV(&wav1);
		deleteWAV(&wav2);
		printf("\nFail    :  %s $ %s\t(Can't Calculate LCSS Distance)\n",
				filename1, filename2);
		return EXIT_FAILURE;
	}
	printf("%.3f\n", LCSS);
	printf("\n");
	deleteWAV(&wav1);
	deleteWAV(&wav2);
	return EXIT_SUCCESS;
}

PRIVATE double euclideanDistance(WAV *wav1, WAV *wav2) {
	WAV *min = NULL;
	long int sum = 0, i;
	if (wav1->header->Subchunk2Size <= wav2->header->Subchunk2Size) {
		min = wav1;
	} else {
		min = wav2;
	}
	for (i = 0; i < min->header->Subchunk2Size; i++) {
		sum += pow(wav1->data->channel[i] - wav2->data->channel[i], 2);
	}
	if (wav1->header->Subchunk2Size > i) {
		for (; i < wav1->header->Subchunk2Size; i++) {
			sum += pow(wav1->data->channel[i], 2);
		}
	} else if (wav2->header->Subchunk2Size >= i) {
		for (; i < wav2->header->Subchunk2Size; i++) {
			sum += pow(wav2->data->channel[i], 2);
		}
	}
	return sqrt(sum);
}

PRIVATE double LCSSDistance(WAV *wav1, WAV *wav2) {
	long int i, j, wav1Size = wav1->header->Subchunk2Size, wav2Size =
			wav2->header->Subchunk2Size;
	unsigned short**C = (unsigned short **) malloc(
			sizeof(unsigned short *) * (wav1Size + 1));
	if (C == NULL) {
		printf("Not enough memory.\n");
		return -1;
	}
	for (i = 0; i <= wav1Size; i++) {
		C[i] = (unsigned short *) malloc(
				sizeof(unsigned short) * (wav2Size + 1));
		if (C[i] == NULL) {
			printf("Not enough memory.\n");
			return -1;
		}
	}
	for (i = 0; i <= wav1Size; i++) {
		C[i][0] = 0;
	}
	for (i = 0; i <= wav2Size; i++) {
		C[0][i] = 0;
	}
#ifdef DEBUG_SIMILARITY
	unsigned long int max = 0;
#endif
	for (i = 1; i <= wav1Size; i++) {
		for (j = 1; j <= wav2Size; j++) {
			if (wav1->data->channel[i - 1] == wav2->data->channel[j - 1]) {
				C[i][j] = C[i - 1][j - 1] + 1;
			} else {
				C[i][j] =
						(C[i][j - 1] > C[i - 1][j]) ? C[i][j - 1] : C[i - 1][j];
			}
#ifdef DEBUG_SIMILARITY
			if (C[i][j] > max) {
				max = C[i][j];
			}
#endif
		}
	}

#ifdef DEBUG_SIMILARITY
	printf("\n\nmax = %lu\n", max);
	printf("\nUSHRT_MAX = %u\n", USHRT_MAX);
	printf("\nWAV 1 Size: = %lu\n", wav1Size);
	printf("\nWAV 2 Size: = %lu\n\n", wav2Size);
#endif
	long int minimum = (wav1Size < wav2Size) ? wav1Size : wav2Size;
	double result = 1 - (double) C[wav1Size][wav2Size] / (double) minimum;
	// Free some space
	for (i = 0; i < wav1Size + 1; i++) {
		free(C[i]);
	}
	free(C);
	return result;
}
#ifdef DEBUG_SIMILARITY
// Test similarity
int main(int argc,char* argv[]) {
	if (strcmp(argv[1], "-similarity") == 0) {
		for (i = 3; i < argc; i++) {
			if (similarity(argv[2], argv[i]) == EXIT_FAILURE) {
				printf("This is not a compatible .wav audio file.\n");
				return EXIT_FAILURE;
			}
		}
	}
	return 0;
}
#endif

