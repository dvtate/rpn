#ifndef RPN_CORE_H
#define RPN_CORE_H

#include <iostream>
#include <stack>
#include <queue>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <inttypes.h>

// this is the class used in our stack
#include "calc_value.hpp"

// user defined variables
#include "user_variables.hpp"

// some useful functions
#include "utils.hpp"

#include "string_stack.hpp"

#include "process_line.hpp"

// don't we all love pretty colors :)))
#include "terminal_colors.hpp"


extern char* metaName;


FILE* program = stdin;
char* progName = NULL;


void runFile(char* programFile, bool& errorReporting){

	progName = programFile;

	program = fopen(programFile, "r");

  	// file error
	if (program == NULL) {

		setTermEffect(TERM_EFF_BOLD);

	  	std::cerr <<metaName <<": ";
	  	color_fputs(stderr, "error: ", 255, 0, 0);
	  	std::cerr <<": could not open file \'" <<programFile <<"\'\n";

	  	setTermEffect();

		exit(EXIT_FAILURE);

	}


	// the most important component of the program :)
	std::stack<CalcValue> mainStack;

	// used for storing the name for user variables on a line by line basis
	std::queue<char*> varNames;


	UserVar* first_node = new UserVar(NULL, " ", 0.0);
  	first_node->first = first_node;

	bool elseStatement = false;

	// for each line in the programFile...
	for (;;) {

	  	// used for line numbers in errors
		line++;


		char* rpnln = (char*) malloc(256);
		size_t lineLen = 256;


		if (getline(&rpnln, &lineLen, program) == -1)
			return; // EOF

		// I need a copy of it to call free() on later.
		char	*rpnln_head = rpnln,
				*errorToken = NULL;
		// process the line
		if ((errorToken =
			processLine(mainStack, first_node,errorReporting, rpnln, elseStatement))
			&& errorReporting
		) {

		  	// file name and
		  	setTermEffect(TERM_EFF_BOLD);
			std::cerr <<programFile <<":" <<line <<':' <<errorToken - rpnln_head <<":\n";
			setTermEffect();

			// print the problem statement
			color_fprintf(stderr, 255, 0, 0, "\t%s\t", getLineFromFile(programFile, line));


		  	// point to the problem area
			for (uint16_t i = 0; i < errorToken - rpnln_head; i++)
				std::cerr <<' ';

			color_fputs(stderr, "^\n", 255, 0, 0);

			// windows sucks :P
			#ifdef _WIN32
				std::cin.ignore();
			#endif

		  	// you're dead :P
			exit(EXIT_FAILURE);

		}

		// prevent memory leaks...
		free(rpnln_head);
	}

	// windows sucks :P
	#ifdef _WIN32
		std::cin.ignore();
	#endif

}



// a NULL CalcValue
CalcValue ans;


void runShell(UserVar* first_node, bool& errorReporting,
	      std::stack<CalcValue>& mainStack, bool& elseStatement
){

	std::cout <<line++ <<">>> ";

	char* rpnln = (char*) malloc(256);
	size_t lineLen = 256;

	if (getline(&rpnln, &lineLen, stdin) == -1) {
		std::cerr <<"\aERROR: Input failed... email toast27@gmail.com if this persists\n\n";
		return;
	}

	// I need a copy of it to call free() on later.
	char* rpnln_head = rpnln;


	// process the line
	bool errors = processLine(mainStack, first_node, errorReporting, rpnln, elseStatement);

	if (errors)
		emptyStack(mainStack);


	// prevent memory leaks...
	free(rpnln_head);

	if (!mainStack.empty()) {
		ans = mainStack.top();
		//ans.print(first_node);
		if (!printCalcValue(ans, first_node))
			std::cout <<'\n';
	}

   	// this allows the ans keyword to function
	if (!mainStack.empty())
		ans = mainStack.top();

	std::cout <<std::endl;

}

void runStringStack(StrStack& code, bool& errorReporting){


	// the most important component of the program :)
	std::stack<CalcValue> mainStack;

	// used for storing the name for user variables on a line by line basis
	std::queue<char*> varNames;

	UserVar* first_node = new UserVar(NULL, " ", 0.0);
  	first_node->first = first_node;

	static CalcValue ans(0.0); // here `0` could be a pointer

	bool elseStatement = false;

	char** stackHead = code.stackHead;

	// for each line in the string stack...
	for (size_t i = 0; i < code.stackDepth; i++) {

	  	// used for line numbers in errors
		line++;



		char* rpnln = *(stackHead++);

		// I need a copy of it to call free() on later.
		char	*rpnln_head = rpnln,
				*errorToken = NULL;


		// process the line
		if ((errorToken =
			processLine(mainStack, first_node, errorReporting, rpnln, elseStatement))
			&& errorReporting
		) {

		  	// file name and
		  	setTermEffect(TERM_EFF_BOLD);
			std::cerr <<progName <<":" <<line <<':' <<errorToken - rpnln_head <<":\n";
			setTermEffect();

			// print the problem statement
			color_fprintf(stderr, 255, 0, 0, "\t%s\t", getLineFromFile(progName, line));

		  	// point to the problem area
		  	for (uint16_t i = 0; i < errorToken - rpnln_head; i++)
				std::cerr <<' ';
			color_fputs(stderr, "^\n", 255, 0, 0);

			// windows sucks :P
			#ifdef _WIN32
				std::cin.ignore();
			#endif

		  	// you're dead :P
			exit(EXIT_FAILURE);


		}
	}
}


///TODO: make this work like processLine (with its method of making errors and such)
///TODO: print error location correctly as well
/// should return bool to tell if there were errors that way processLine can point
///		the user towards the place the strstack was called.
void runStringStack(
	StrStack& code, bool& errorReporting, std::stack<CalcValue>& mainStack,
	UserVar* first_node
){

	bool elseStatement = false;
	//static CalcValue ans(0.0); // here `0` could be a pointer
	size_t localLine = 0;

	char** stackHead = code.stackHead;

	// for each line in the string stack...
	for (size_t i = 0; i < code.stackDepth; i++) {

	  	// used for line numbers in errors
		localLine++;


		//char* rpnln = *(stackHead++);
		char* rpnln = new char[strlen(*stackHead)];
		strcpy(rpnln, *(stackHead++));

		// I need a copy of it to call free() on later.
		char	*rpnln_head = rpnln,
				*errorToken = NULL;


		// process the line
		if ((errorToken =
			processLine(mainStack, first_node, errorReporting, rpnln, elseStatement))
			&& errorReporting)
		{
			// why the fuck doesn't this get run ???????
		  	setTermEffect(TERM_EFF_BOLD);
			std::cerr <<progName <<": block : " <<localLine <<" : " <<rpnln - rpnln_head << ":\n";
			setTermEffect();

			// print the problem statement
			//color_fprintf(stderr, 255, 0, 0, "\t%s\t", getLineFromFile(progName, line));
			color_fprintf(stderr, 255, 0, 0, "\t%s\t", trimStr(*(stackHead - 1)));
		  	// point to the problem area
		  	while (rpnln_head++ != rpnln)
				std::cerr <<' ';
			color_fputs(stderr, "^\n", 255, 0, 0);

			// windows sucks :P
			#ifdef _WIN32
				std::cin.ignore();
			#endif

		  	// you're dead :P
			exit(EXIT_FAILURE);

		}

	}

}


#endif
