/**
 *  Program: wavengine  Copyright (C) 2018  Marios Pafitis & Valentinos Pariza
 *  This program comes with ABSOLUTELY NO WARRANTY;
 *  This is free software, and you are welcome to redistribute it under certain
 *  conditions;
 */
#ifndef WAVELIB_H
#define WAVELIB_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * @brief Displays the header of a .wav file
 *
 *  Implements the list method in .wav files that it receives as input. It takes as input a
 *  .wav and presents a list of the file's characteristics in the display. Basically the
 *  output is the Header of the .wav file.
 *
 * 	@param *inFilename the input filename of the WAV
 * 	@return int Success or Failure
 *	@author Marios Pafitis
 * 	@bug No known bugs.
 */
int list(char *inFilename);

/**
 * @brief Convert a stereo to mono .wav file
 *
 *  Implements the mono method in .wav files that it receives as input. It takes as input a
 *  .wav file and checks if the file is a stereo audio file. Then it converts the stereo
 *  to mono by keeping only the left channel of the audio. It reduces the size of
 *  the audio file by half. It creates an output file that it has the form of
 *  "new-" + filename + ".wav" and it saves it in the path folder of the input filename.
 *
 * 	@param *inFilename the input filename of the WAV
 * 	@return int Success or Failure
 *	@author Marios Pafitis
 * 	@bug No known bugs.
 */
int mono(char *inFilename);

/**
 * @brief Mixes two soundtracks indicated by two files and write the result in a
 * new file
 *
 * This method takes the names of two .wav files ,that contain information in
 * a header section and data in a data section about the sound track. It mixes the
 * two sound tracks and writes-saves the result as a new .wav file . By saying
 * "mix the two sound tracks " ,it means to take the two sound tracks and put the
 * first one to the right channel and the second one to the left channel. More
 * specific it takes the left channel of the second sound track,if it is stereo,
 * and places it to the left channel of the new sound track(if it is mono it takes
 * its single channel) and if the first one is stereo,it takes the right channel
 * of it and places it to the right channel of the new sound track (if it is mono
 * it takes the single channel of it and places it to the right channel of the
 * new sound track ).
 * This method uses @see createOutputFilenameTwoFiles(char*, char*,char*, char**)
 * for creating the output fileName .It creates the new .wav file with name
 * mix-<filename1>-<filename2>.wav
 * Also uses the method @see writeWAV(char*, WAV*) for writing the the sound tract
 * as a struct to a file and the method @see readWAV(char*, WAV**) for reading
 * the .wav files which their names where passed as arguments
 *
 * @param a pointer to a sequence of characters that is the name of the first
 *        .wav file to mix
 * @param a pointer to a sequence of characters that is the name of the second
 *        .wav file to mix
 *
 * @return EXIT_SUCCESSif the mix was executed successfully or EXIT_FAILURE if
 *         there was a problem during the execution of the method
 *
 * @author Valentinos Pariza
 */
int mix(char*, char*);

/**
 * @brief Chops a .wav audio file
 *
 *  Implements the chop method in .wav files,that it receives as input. It takes as input a
 *  .wav file and chops it for the specific duration which it takes as input.
 *  For example if the left = 2 and the right = 4 it will chop the track from
 *  the second 2 to the second 4. Be careful, the method is not working for decimal
 *  values. For example you can't chop the track from the 1.5 second to the 2.5 second.
 *  It create an output file that it has the form of "chopped-" + filename + ".wav"
 *  and it saves it in the path folder of the input filename.
 *
 * 	@param *inFilename the input filename of the WAV
 * 	@param *l the starting second
 * 	@param *r the ending second
 * 	@return int Success or Failure
 * 	@author Marios Pafitis
 * 	@bug No known bugs.
 */
int chop(char *inFilename, int l, int r);

/**
 * @brief This method reverses a sound track taken from a .wav file and saves it to
 *        a new file
 *
 * This method reads a sound track from a .wav file and reverses its data , so
 * the first sample to be last ,the second to be before last ,and so on. After
 * creating the reversed sound track ,the new sound track (struct) is stored to a
 * file .wav with a new file name.
 *
 *This method uses @see createOutputFilename(char*, char*,char**)
 * for creating the output fileName .It creates the reversed .wav file with name
 * reverse-<filename>.wav
 * Also uses the method @see writeWAV(char*, WAV*) for writing the sound tract
 * as a struct to a file and the method @see readWAV(char*, WAV**) for reading
 * the .wav file which its name is passed as an argument
 *
 * @param a pointer to a sequence of characters that is the name of the
 *        .wav file to reverse
 *
 * @return EXIT_SUCCESSif the reverse-method was executed successfully or
 *         EXIT_FAILURE if there was a problem during the execution of the method
 * @author Valentinos Pariza
 */
int reverse(char*);

/**
 * @brief Checks the similarity between two audio files.
 *
 *  Implements the mono method in .wav files that it receives as input. It takes as input two
 *  .wav audio files. It checks the similarity between the two files based two methods. THe first
 *  method is the Euclidean Distance a method really fast but inaccurate. The other method is the
 *  LCSS Distance, a more accurate version but very slow. It works properly but it takes a lot of time.
 *  There are other solving methods such as the LCSS which is using recursion but this one is even
 *  more slow. FInally, at the end the method represents the Euclidean Distance and the LCSS Distance.
 *
 * 	@param *filename1 the input filename1 of the WAV
 * 	@param *filename2 the input filename2 of the WAV
 * 	@return int Success or Failure
 *	@author Marios Pafitis
 * 	@bug No known bugs.
 */
int similarity(char *filename1, char* filename2);

/**
 * @brief This method encodes a message in soundtrack and saves it as a new soundtrack
 *
 * This method reads a soundtrack from a .wav file which its filename is passed
 * as an argument and also reads a sequence of characters(pointer to characters)
 * which indicates the content of the message.After these the message is
 * encrypted inside the soundtrack based on an algorythm and also on a secret
 * key ,which key is encapsulated inside the code.  After creating the
 * sountrack with the encoded message inside , the new soundtrack (struct) is
 * stored to a file .wav with a new file name.
 *
 * This method uses @see createOutputFilename(char*, char*,char**)
 * for creating the output fileName .It creates the new .wav file with name
 * new-<filename>.wav
 * Also uses the method @see writeWAV(char*, WAV*) for writing the  soundract
 * as a struct to a file and the method @see readWAV(char*, WAV**) for reading
 * the .wav file which its name is passed as an argument
 *
 * @param a pointer to a sequence of characters that is the name of the
 *        .wav file to encode inside a message
 *
 * @param a pointer to a sequence of characters that is the message to be encoded
 *        -stored inside the soundtrack
 *
 * @return EXIT_SUCCESSif the encoding was executed succesfully or EXIT_FAILURE if
 *         there was a problem during the execution of the method
 *
 * @author Valentinos Pariza
 */
int encodeText(char*, char*);

/**
 * @brief This method decodes a message from a soundtrack and saves it in a text file
 *
 * This method reads a sound track from a .wav file which its filename is passed
 * as an argument and also reads an integer which is the length of the string to
 * decode and also a pointer to characters which is the name of the text file to
 * write inside the decoded string.It reads the .wav file ,applies an algorithm
 * for decoding ,based on a secret key (which is encapsulated inside the code)
 * and after gaining the message from the sound track , it writes the message to
 * a text file , which name is passed as an argument.
 *
 * Also uses the method @see writeWAV(char*, WAV*) for writing the  sound tract
 * as a struct to a file and the method @see readWAV(char*, WAV**) for reading
 * the .wav file which its name is passed as an argument
 *
 * @param a pointer to a sequence of characters that is the name of the
 *        .wav file , which contains an encoded message
 *
 * @param an integer which is the length of the string to be decoded from the
 *        sound tract ,without including the NUL (='\0') character
 *
 * @param a pointer to a sequence of characters that is the name of the text file
 *        to be written inside the message which is derived from the sound track
 *
 * @return EXIT_SUCCESS if the decoding was executed successfully or EXIT_FAILURE if
 *         there was a problem during the execution of the method
 *
 * @author Valentinos Pariza
 */
int decodeText(char*, int, char*);

/**
 * @brief Merge two .wav audio files
 *
 *  Implements the merge method in .wav files that it receives as input. It takes as input two
 *  .wav files and merge the second one at the end of the first one. It create an
 *  output file that it has the form of "merge-" + filename1 "-" filename2 ".wav"
 *  and it saves it in the path folder of the first input file.
 *
 * 	@param *filename1 the input filename1 of the WAV
 * 	@param *filename2 the input filename2 of the WAV
 * 	@return int Success or Failure
 *	@author Marios Pafitis
 * 	@bug No known bugs.
 */
int merge(char*filename1, char*filename2);

/**
 * @brief prints the GPL
 *
 *  Prints the GPL for the library wavelib.h
 *
 * 	@return void
 *	@author Marios Pafitis
 * 	@bug No known bugs.
 */
void printGPL();

#endif
