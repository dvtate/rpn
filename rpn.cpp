#include <iostream>
#include <stack>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>


// this is the class used in our stack
#include "calc_value.h"

// user defined variables
#include "user_variables.h"

#define MAX_LEN 200

// some useful functions
#include "utils.h"


long double ans = 0;
unsigned int line = 0;




int main(){

	// display the helper
	if (line == 0)
		displayHelp();

// goto's can be evil, but this program is too smalle for this to be an issue.
main_start_after_help:

	std::stack<CalcValue> numstack;

	std::cout <<line++ <<"> ";

	char rpnln[MAX_LEN + 1];
	std::cin.getline(rpnln, MAX_LEN);

	// used for storing the name for user variables
	char* variableName1 = NULL; // will get used later
	char* variableName2 = NULL;

	// get first token from the input
	char* p = strtok(rpnln, " ");

	// empty string/whitespace input
	if (p == NULL)
		return main();

	while (p != NULL) {

		// char is a binary operator
		if (((*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '%'
			|| *p == '|' || *p == '&' || *p == '^') && *(p + 1) == '\0')
			|| !strcmp(p, "<<") || !strcmp(p, ">>")  || !strcmp(p, "**")
			|| !strcmp(p, "logBase") || !strcmp(p, "logBASE") || !strcmp(p, "logbase")
			|| !strcmp(p, "pow")
		) {

			if (numstack.top().type != CalcValue::NUM) {
				std::cerr <<"ERROR: incompatible data-types!";
				return main();
			}
			long double b = getNextValue(numstack).getNum();

			if (numstack.top().type != CalcValue::NUM) {
				std::cerr <<"ERROR: incompatible data-types!";
				return main();
			}
			long double a = getNextValue(numstack).getNum();

			switch (*p) {
				case '+': numstack.push(a + b); break;
				case '*':
					if (strcmp(p, "**") == 0)
						numstack.push(pow(a, b));
					else
						numstack.push(a * b);
					break;
				case '/': numstack.push(a / b); break;
				case '-': numstack.push(a - b); break;
				case '%': numstack.push((int) a % (int) b); break;
				case '|': numstack.push((int) a | (int) b); break;
				case '^': numstack.push((int) a ^ (int) b); break;
				case '&': numstack.push((int) a & (int) b); break;
				case '<': numstack.push((int) a << (int) b); break;
				case '>': numstack.push((int) a >> (int) b); break;
				case 'l': numstack.push(log10(b) / log10(a)); break;
				case 'p': numstack.push(pow(a, b)); break;
			}

		}
		// char is a unary operator
			//trig functions
		else if (strcmp(p, "sin") == 0)
			numstack.push(sin(getNextValue(numstack).getNum()));
		else if (strcmp(p, "cos") == 0)
			numstack.push(cos(getNextValue(numstack).getNum()));
		else if (strcmp(p, "tan") == 0)
			numstack.push(tan(getNextValue(numstack).getNum()));

		else if (strcmp(p, "asin") == 0)
			numstack.push(asin(getNextValue(numstack).getNum()));
		else if (strcmp(p, "acos") == 0)
			numstack.push(acos(getNextValue(numstack).getNum()));
		else if (strcmp(p, "atan") == 0)
			numstack.push(atan(getNextValue(numstack).getNum()));

		else if (strcmp(p, "sinh") == 0)
			numstack.push(sinh(getNextValue(numstack).getNum()));
		else if (strcmp(p, "cosh") == 0)
			numstack.push(cosh(getNextValue(numstack).getNum()));
		else if (strcmp(p, "tanh") == 0)
			numstack.push(tanh(getNextValue(numstack).getNum()));

		else if (strcmp(p, "asinh") == 0)
			numstack.push(asinh(getNextValue(numstack).getNum()));
		else if (strcmp(p, "acosh") == 0)
			numstack.push(acosh(getNextValue(numstack).getNum()));
		else if (strcmp(p, "atanh") == 0)
			numstack.push(atanh(getNextValue(numstack).getNum()));

		// more unary functions
		else if (strcmp(p, "log") == 0 || strcmp(p, "log10") == 0)
			numstack.push(log10(getNextValue(numstack).getNum()));
		else if (strcmp(p, "ln") == 0)
			numstack.push(log(getNextValue(numstack).getNum()));
		else if (strcmp(p, "sqrt") == 0 || strcmp(p, "sqr") == 0)
			numstack.push(sqrt(getNextValue(numstack).getNum()));

		// comments... because I can XDDDDDDDDD
		else if (*p == '#') {
			if (numstack.size() == 0)
				goto main_start_after_help;
			break;

		// pi
		} else if (strcmp(p, "pi") == 0)
			numstack.push(M_PI);

		// ans
		else if (strcmp(p, "ans") == 0) // p == "ans"
				numstack.push(ans);
		// exit the program
		else if (*p == 'q' || !strcmp(p, "exit")) // p == "q"
			goto exit; // exit the program

		// show help
		else if (strcmp(p, "help") == 0) {
			displayHelp();
			return main();

		// clear screen
		} else if (strcmp(p, "clear") == 0 || strcmp(p, "cls") == 0) {
			#ifdef _WIN32
				system("cls");
			#else
				system("clear");
			#endif
			return main();


		// essentially restarts the program (don't display help)
		} else if (strcmp(p, "reset") == 0 ) {
			ans = line = 0;
			vars::wipeAll(vars::first_node);
			goto main_start_after_help;

		// bitwise not operator
		} else if (*p == '~' && *(p + 1) != '\0')
			numstack.push(~atoi(p + 1));

		// assignment operator
		else if (*p == '=' && *(p + 1) == '\0')
			if (variableName1 != NULL)
				vars::assignVar(vars::first_node, variableName1, numstack.top());
			else if (variableName2 != NULL)
				vars::assignVar(vars::first_node, variableName2, numstack.top());
			else {
				std::cerr <<"\aERROR: inappropriate use of assignment operator.\n" <<std::endl;
				return main();
			}

		// variable
		else if (*p == '$' && *(p + 1) != '\0') { // user must use '$' prefix to access the variables

			if (strlen(p + 1) > USERVAR_NAME_MAXLENGHT) {
				std::cerr <<"Error: Your variable\'s name is too long.\n" <<std::endl;
				return main();
			} else
				if (vars::varExists(vars::first_node, p + 1)) {
					numstack.push(vars::findVar(vars::first_node, p + 1)->getNumber());
					if (variableName1 == NULL)
						variableName1 = p + 1;
					else
						variableName2 = p + 1;
				} else
					if (variableName1 == NULL)
						variableName1 = p + 1;
					else
						variableName2 = p + 1;


		// user is defining a function
		} /*else if (strcmp(p, "@function")) {


		// user is calling a function
		} else  if (*p == '@' && *(p + 1) != '\0') {



		// anything else
		} */
		else {
			// parse input
			double number = atof(p);

			// the user is an asshole :T
			if (number == 0 && *p != '0') {
				std::cerr <<"\aSYNTAX ERROR\n" <<std::endl;
				return main();

			// the user has given us a number :D
			} else
				numstack.push(number);
		}

		// get next token
		p = strtok(NULL, " ");

	}


	if (!numstack.empty())
		std::cout <<"\aans " <<(double)(ans = numstack.top().getNum()) <<" =\n" <<std::endl;

	return main(); //next line...

exit:
//	vars::wipeAll();
//	delete vars::first;

	exit(EXIT_SUCCESS);
}
