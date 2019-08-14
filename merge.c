/**
 *  Program: wavengine  Copyright (C) 2018  Marios Pafitis & Valentinos Pariza
 *  This program comes with ABSOLUTELY NO WARRANTY;
 *  This is free software, and you are welcome to redistribute it under certain
 *  conditions;
 *  @file merge.c
 *  @brief Merge two .wav files
 *
 *  Implements the merge method in .wav files that it receives as input. It takes as input two
 *  .wav files and merge the second one at the end of the first one. It create an
 *  output file that it has the form of "merge-" + filename1 "-" filename2 ".wav"
 *  and it saves it in the path folder of the first input file.
 *
 *  @version 1.0
 *  @author Marios Pafitis
 *  @bugs No known bugs
 */
#include "utilities.h"

PUBLIC int merge(char *filename1, char*filename2) {
	// Create output filename
	char *outFilename = NULL;
	if (createOutputFilenameTwoFiles(filename1, filename2, "merged-",
			&outFilename) == EXIT_FAILURE) {
		printf("Fail    :  %s & %s\t(Can't create output filename)\n",
				filename1, filename2);
		return EXIT_FAILURE;
	}
// Read WAV 1
	WAV *wav1 = NULL;
	if (readWAV(filename1, &wav1) == EXIT_FAILURE) {
		free(outFilename);
		printf("Fail    :  %s\t(Can't read WAV file)\n", filename1);
		return EXIT_FAILURE;
	}
// Read WAV 2
	WAV *wav2 = NULL;
	if (readWAV(filename2, &wav2) == EXIT_FAILURE) {
		free(outFilename);
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
		printf("\nThe two audio files are not align.\n");
		free(outFilename);
		deleteWAV(&wav1);
		deleteWAV(&wav2);
		return EXIT_FAILURE;
	}
// Create new WAV and copy data
	WAV *wav_new = (WAV*) malloc(sizeof(WAV));
	wav_new->header = (HEADER*) malloc(sizeof(HEADER));
	wav_new->data = (DATA*) malloc(sizeof(DATA));
	wav_new->data->channel = (byte*) malloc(
			wav1->header->Subchunk2Size + wav2->header->Subchunk2Size);
	memcpy(&wav_new->data->channel[0], &wav1->data->channel[0],
			wav1->header->Subchunk2Size);
	memcpy(&wav_new->data->channel[wav1->header->Subchunk2Size],
			&wav2->data->channel[0], wav2->header->Subchunk2Size);
	strcpy((char *) wav_new->header->ChunkID, (char *) wav1->header->ChunkID);
	strcpy((char *) wav_new->header->Format, (char *) wav1->header->Format);
	strcpy((char *) wav_new->header->Subchunk2ID,
			(char *) wav1->header->Subchunk2ID);
	wav_new->header->ChunkSize = wav1->header->ChunkSize
			+ wav2->header->ChunkSize - 36;
	wav_new->header->Subchunk1Size = wav1->header->Subchunk1Size;
	wav_new->header->AudioFormat = wav1->header->AudioFormat;
	wav_new->header->NumChannels = wav1->header->NumChannels;
	wav_new->header->SampleRate = wav1->header->SampleRate;
	wav_new->header->ByteRate = wav1->header->ByteRate;
	wav_new->header->BlockAlign = wav1->header->BlockAlign;
	wav_new->header->BitsPerSample = wav1->header->BitsPerSample;
	wav_new->header->Subchunk2Size = wav1->header->Subchunk2Size
			+ wav2->header->Subchunk2Size;
// Write data
	writeWAV(outFilename, wav_new);
	printf("Success :  %s\t(Created)\n", outFilename);
// Free mallocs
	free(outFilename);
	deleteWAV(&wav1);
	deleteWAV(&wav2);
	deleteWAV(&wav_new);
	return EXIT_SUCCESS;
}
#ifdef DEBUG_MERGE
// Test merge
int main(int argc, char* argv[]) {
	if (strcmp(argv[1], "-merge") == 0) {
		if (argc != 4) {
			printf(
					"\nWrong command format. Give a two audio files as input\n\n");
		}
		if (merge(argv[2], argv[3]) == EXIT_FAILURE) {
			printf("This is not a compatible .wav audio file.\n");
			return EXIT_FAILURE;
		}
	}
	return 0;
}
#endif
