/**
 *  Program: wavengine  Copyright (C) 2018  Marios Pafitis & Valentinos Pariza
 *  This program comes with ABSOLUTELY NO WARRANTY;
 *  This is free software, and you are welcome to redistribute it under certain
 *  conditions;
 *  @file chop.c
 *  @brief Chops a .wav file
 *
 *  Implements the chop method in .wav files that it receives as input. It takes as input a
 *  .wav file and chops it for the specific duration which it takes as input.
 *  For example if the left = 2 and the right = 4 it will chop the track from
 *  the second 2 to the second 4. Be careful, the method is not working for decimal
 *  values. For example you can't chop the track from the 1.5 second to the 2.5 second.
 *  It create an output file that it has the form of "chopped-" + filename + ".wav"
 *  and it saves it in the path folder of the input filename.
 *
 *  @version 1.0
 *  @author Marios Pafitis
 *  @bugs No known bugs
 */
#include "utilities.h"

PUBLIC int chop(char *inFilename, int l, int r) {
	if (r <= l || r < 0 || l < 0) {
		printf("Fail   :  %s   (Invalid time input)\n", inFilename);
		return EXIT_FAILURE;
	}
// Create output filename
	char *outFilename = NULL;
	if (createOutputFilename(inFilename, "chopped-",
			&outFilename) == EXIT_FAILURE) {
		printf("Fail    :  %s\t(Can't create output filename)\n", inFilename);
		return EXIT_FAILURE;
	}
// Read Wav
	WAV *wav_old = NULL;
	if (readWAV(inFilename, &wav_old) == EXIT_FAILURE) {
		free(outFilename);
		printf("Fail    :  %s\t(Can't read WAV file)\n", inFilename);
		return EXIT_FAILURE;
	}
	long left = l * wav_old->header->SampleRate * wav_old->header->BlockAlign
			* wav_old->header->NumChannels
			/ (wav_old->header->BitsPerSample / 8);
	long right = r * wav_old->header->SampleRate * wav_old->header->BlockAlign
			* wav_old->header->NumChannels
			/ (wav_old->header->BitsPerSample / 8);
// Check if left and right are correct
#ifdef DEBUG
	printf("left:%lu\nright:%lu\nsize:%lu\n", left, right,
			wav_old->header->Subchunk2Size);
#endif
	if (right > wav_old->header->Subchunk2Size
			|| left > wav_old->header->Subchunk2Size) {
		printf("Fail    :  %s\t(Invalid time input)\n", inFilename);
		free(outFilename);
		deleteWAV(&wav_old);
		return EXIT_FAILURE;
	}
// Create new Wav and copy data
	WAV *wav_new = (WAV*) malloc(sizeof(WAV));
	wav_new->header = (HEADER*) malloc(sizeof(HEADER));
	wav_new->data = (DATA*) malloc(sizeof(DATA));
	wav_new->data->channel = (byte*) malloc(right - left);
	memcpy(&wav_new->data->channel[0], &wav_old->data->channel[left],
			right - left);
	strcpy((char *) wav_new->header->ChunkID,
			(char *) wav_old->header->ChunkID);
	strcpy((char *) wav_new->header->Format, (char *) wav_old->header->Format);
	strcpy((char *) wav_new->header->Subchunk2ID,
			(char *) wav_old->header->Subchunk2ID);
	wav_new->header->ChunkSize = wav_old->header->ChunkSize
			- (wav_old->header->Subchunk2Size - (right - left));
	wav_new->header->Subchunk1Size = wav_old->header->Subchunk1Size;
	wav_new->header->AudioFormat = wav_old->header->AudioFormat;
	wav_new->header->NumChannels = wav_old->header->NumChannels;
	wav_new->header->SampleRate = wav_old->header->SampleRate;
	wav_new->header->ByteRate = wav_old->header->ByteRate;
	wav_new->header->BlockAlign = wav_old->header->BlockAlign;
	wav_new->header->BitsPerSample = wav_old->header->BitsPerSample;
	wav_new->header->Subchunk2Size = right - left;
// Write data
	writeWAV(outFilename, wav_new);
	printf("Success :  %s\t(Created)\n", outFilename);
// Free mallocs
	free(outFilename);
	deleteWAV(&wav_old);
	deleteWAV(&wav_new);
	return EXIT_SUCCESS;
}
#ifdef DEBUG_CHOP
// Test chop
int main(int argc,char* argv[]) {
	if (strcmp(argv[1], "-chop") == 0) {F
		int l = atoi(argv[3]);
		int r = atoi(argv[4]);
		if (chop(argv[2], l, r) == EXIT_FAILURE) {
			printf("This is not a compatible .wav audio file.\n");
			return EXIT_FAILURE;
		}
	}
	return 0;
}
#endif
