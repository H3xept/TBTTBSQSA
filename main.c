/**************************************************************************
 * Assessment Title:
 * ACE 3 (TBTTBSQSA)
 *
 * Number of Submitted C Files: 
 * 19 (51 total files including .h, .py, .asm and .md)
 * 
 * Date: 
 * 25/11/2018
 * 
 * Authors: 
 *	1. Leonardo Cascianelli, Reg no: 201710666
 *	2. Rory Brown, Reg no: 201717976
 *	3. Ewan Skene, Reg no: 201717413
 * 
 *
 *	Statement: We confirm that this submission is all our own work.
 *
 *  (Signed)_Leonardo_Cascianelli________________(Leonardo Cascianelli)
 *	
 * 	(Signed)_Rory_Brown__________________________(Rory Brown)
 *	
 *	(Signed)_Ewan_Skene__________________________(Ewan Skene)
 *
 **************************************************************************/

/**
* Filename: main.c
* Class: VirtualMachine
* Authors: Leonardo Cascianelli,Rory Brown,Ewan Skene
* Date: 2018-11-25
* 
* Description: The main file, runs the virtual machine/disassembler
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include "./src/oop/umbrella.h"
#include "./src/VirtualMachine/Program.h"
#include "./src/VirtualMachine/VirtualMachine.h"
#include <string.h>
#include <math.h>
#include "./src/Disassembler/Disassembler.h"
#include <assert.h>

#define k_HELP_STRING "Help:\n\
	Select Mode:\n\
	➪ dm (Disassemble Mode): Runs the Disassembler on the loaded program\n\
	➪ rm (Run Mode): Runs the Virtual Machine on the loaded program\n\
	➪ drm (Disassemble + Run Mode): Disassembles and runs the loaded program\n\
	-----------\n\
	Program Loading Flags:\n\
	-d (or --default): Load the default program\n\
	-c (or --console): Load a program from stdin\n\
	-f (or --file) <filename> : Load a program from a file\n"

#define k_NOT_ENOUGH_PARAMS "Not enough arguments. \
Usage: %s <running_mode> <program_loading_flags> .\n\
Type 'help' as first argument for the list of available commands."
#define k_FILE_PATH_REQUIRED "Filepath required. Usage ... %s <filepath>"

#define k_PRE_INPUT "Input accepted in decimal and hex:\n"
#define k_ONE_MORE_WORDS_OUT_BOUNDARIES "One or more words out of boundaries (MAX: %d)"
#define k_INPUT_BEGIN ">>> "

#define k_FLAG_DEFAULT "-d"
#define k_FLAG_DEFAULT_LONG "--default"
#define k_FLAG_CONSOLE "-c"
#define k_FLAG_CONSOLE_LONG "--console"
#define k_FLAG_FILE "-f"
#define k_FLAG_FILE_LONG "--file"

#define k_DEFAULT_PROGRAM_PATH "./src/VirtualMachine/vm_programs/binaryCounter.bin"
#define k_MODE_RUN_MODE "rm"
#define k_MODE_DISASSEMBLE_MODE "dm"
#define k_MODE_DISASSEMBLE_RUN_MODE "drm"

#define k_HELP "help"

typedef enum {
	Program_Mode_Run, Program_Mode_Disassemble,
	Program_Mode_Disassemble_And_Run
} Program_Mode;

void display_help(void);
uint16_t* input_words(void);
Program* handle_program_loading_flag(const char* flag, int* argc, char const *argv[]);
Program_Mode handle_running_mode_flag(const char* flag);
void setup_program_mode(Program_Mode mode, VirtualMachine** vm, Disassembler** disasm, Program* program, char** file_out);
void detect_help(int argc, char const *argv[]);
char* request_file_out();

/**
* @brief: displays the help string
*/
void display_help(void)
{
	_pinfo(k_HELP_STRING,NULL);
}

/**
* @brief: takes in words as input
* @return uint16_t*: an array of words from input
*/
uint16_t* input_words(void)
{
	uint16_t max_accepted = (uword_t)pow(2, WORD_SIZE)-1;

	uint16_t* ret = malloc(MAX_PROG_SIZE+1);
	char* buff = malloc(MAX_PROG_SIZE);
	int c = 0;

	memset(ret, '\0', MAX_PROG_SIZE);
	memset(buff, '\0', MAX_PROG_SIZE);

	printf(k_INPUT_BEGIN);

	fgets(buff, MAX_PROG_SIZE, stdin);
	char* last = NULL;
	long signed_word = strtol(buff, &last, 0);
	errno = 0;

	while(!errno && ++c)
	{
		if (signed_word > max_accepted){
			_warn(k_ONE_MORE_WORDS_OUT_BOUNDARIES, max_accepted);
			return input_words();
		}
		*(ret+c) = (uint16_t)signed_word;
		signed_word = strtol(last, &last, 0);
	} *(ret) = c;

	free(buff);

	return ret;
}

/**
* @brief: generates a program based on loading flag
* @param flag: string containing loading flag
* @param argc: pointer to argument count
* @param argv: pointer to args array
* return Program*: program generated using loading flag
*/
Program* handle_program_loading_flag(const char* flag, int* argc, char const *argv[])
{
	Program* program = NULL;
	uint16_t* words = NULL;
	uint16_t n_words = 0;
	const char* filename = NULL;

	if (!strcmp(flag,k_FLAG_DEFAULT) || !strcmp(flag,k_FLAG_DEFAULT_LONG)) {

		program = Program_With_File(k_DEFAULT_PROGRAM_PATH);

	} else if (!strcmp(flag, k_FLAG_CONSOLE) || !strcmp(flag, k_FLAG_CONSOLE_LONG)) {

		printf(k_PRE_INPUT);
		words = input_words();
		n_words = *words;
		program = Program_With_Buffer(++words,n_words);

	} else if (!strcmp(flag, k_FLAG_FILE) || !strcmp(flag, k_FLAG_FILE_LONG)) {

		if (*argc < 4) {
			_err(k_FILE_PATH_REQUIRED, flag);
		} filename = argv[1];

		program = Program_With_File(filename);

	} else if (!strcmp(flag, k_HELP)) {
		
		display_help();
		exit(0);

	} else {
		_warn("Invalid program loading flag %s.", flag);
		_pinfo(k_HELP_STRING, NULL);
		exit(0);
	}

	return program;
}

/**
* @brief: returns a Program_Mode based on the running mode flag
* @param flag: string containing running mode flag
* return Program_Mode: program mode corresponding to running mode flag
*/
Program_Mode handle_running_mode_flag(const char* flag)
{
	if (!strcmp(flag, k_MODE_RUN_MODE)) {
		return Program_Mode_Run;
	} else if (!strcmp(flag, k_MODE_DISASSEMBLE_MODE)) {
		return Program_Mode_Disassemble;
	} else if (!strcmp(flag, k_MODE_DISASSEMBLE_RUN_MODE)) {
		return Program_Mode_Disassemble_And_Run;
	} else {
		_err("Running mode %s not recognised.",flag);
	}
}

/**
* @brief: sets up the program with the mode and objects required to run
* @param mode: the program mode to run in
* @param vm: the virtual machine to use
* @param disasm: the disassembler to use
* @param program: the program to use
* @param file_out: the file to output to
*/
void setup_program_mode(Program_Mode mode, VirtualMachine** vm, Disassembler** disasm, Program* program, char** file_out)
{
	switch(mode)
	{
		case Program_Mode_Run:
			*vm = alloc_init(VirtualMachine_Class_Descriptor);
		break;
		case Program_Mode_Disassemble:
			goto DISASM_ONLY;
		break;
		case Program_Mode_Disassemble_And_Run:
			*vm = alloc_init(VirtualMachine_Class_Descriptor);
DISASM_ONLY:
			*disasm = Disassembler_With_Program(program);
			*file_out = request_file_out();
		break;
	}
}

/**
* @brief: detects if the help string needs to be printed, and does so if needed.
* @param argc: argument count
* @param argv: argument array
*/
void detect_help(int argc, char const *argv[])
{
	if (!(argc-1)) return;
	if (!strcmp(argv[1], k_HELP))
	{
		_pinfo(k_HELP_STRING, argv[0]);
		exit(0);
	}
}

/**
* @brief: requests a file to output the disassembled code to
* return char*: returns the sanitised filename
*/
char* request_file_out()
{
	char* rt = calloc(1, 100);
	printf("Output for disassembled file: ");
	rt = fgets(rt, 99, stdin);
	
	int ind = 0;
	while(*(rt+ind++) != '\n');
	*(rt+ind-1) = '\0';

	return rt;
}

/**
* @brief: main function, runs disassembler and/or vm based on flags
* @param argc: argument count
* @param argv: argument array
* return int: exit code
*/
int main(int argc, char const *argv[])
{
	detect_help(argc, argv);

	if (argc < 3)
	{
		_err(k_NOT_ENOUGH_PARAMS, argv[0]);
	}

	VirtualMachine* vm = NULL;
	Disassembler* disasm = NULL;
	Program* program = handle_program_loading_flag(argv[2], &argc, &argv[2]);
	int program_size;		
	char* file_out = NULL;

	setup_program_mode(handle_running_mode_flag(argv[1]), &vm, &disasm, program, &file_out);

	if (disasm) {
		Disassembler_Statically_Disassemble_And_Dump(disasm, &program_size, file_out);
	} if (vm) {
		Virtual_Machine_Run(vm, program);
	}
	
	return 0;
}
