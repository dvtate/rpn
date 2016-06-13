#ifndef USER_VARIABLES_H
#define USER_VARIABLES_H

#include <cstdlib>
#include <cstring>

#define USERVAR_NAME_MAXLENGHT 20



// the node for my linked list of user defined variables
class UserVar {
public:
	char name[USERVAR_NAME_MAXLENGHT];

  	enum type {NUM, STR, PTR} valType;

	union {
		long double number;
		char* string;
		void* pointer;
	};

	UserVar *next;

	UserVar(const char* identifier, long double contents):
		number(contents)
	{
		next = (UserVar*) NULL;

	  	strncpy(name, identifier, USERVAR_NAME_MAXLENGHT);
	  	valType = NUM;
	}

	UserVar(const char* identifier, long double contents, UserVar* next_node):
		number(contents)
	{
		next = next_node;

	  	strncpy(name, identifier, USERVAR_NAME_MAXLENGHT);
		valType = NUM;
	}

	// geting the values
	double getNumber(){
		if (valType == NUM)
			return number;
	  	else
			return 0;
	}
	char* getString(){
	  	if (valType == STR)
			return string;
	  	else
		  	return (char*) NULL; // beware of segfaults...
	}
	void* getPointer(){
	  	if (valType == PTR)
		  	return pointer;
	  	else
		  	return (void*) NULL;
	}

	// changing the values
	void setValue(long double val){
		number = val;
	  	valType = NUM;
	}
	void setValue(const char* val){
	  	strcpy(string, val);
	  	valType = STR;
	}
	void setValue(void* val){
	  	pointer = val;
		valType = PTR;
	}

};


namespace vars {

	extern UserVar* first_node;

  	// the last element on the linked list
  	extern UserVar* lastVar(UserVar* first);

	// to be called after reset
	extern void wipeAll(UserVar* first);

	// to make a  new variable, or change it's value
	extern void assignVar(UserVar* first, char name[USERVAR_NAME_MAXLENGHT], long double value);

	// to remove an individial variable
	extern void removeVar(UserVar* first, char name[USERVAR_NAME_MAXLENGHT]);

	// returns a pointer to the variable
	extern UserVar* findVar(UserVar* first, char name[USERVAR_NAME_MAXLENGHT]);

	extern bool varExists(UserVar* first, char name[USERVAR_NAME_MAXLENGHT]);

}



#endif
