#ifndef CALC_VALUE_H
#define CALC_VALUE_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "string_stack.hpp"

#define USERVAR_NAME_MAXLENGHT 20
// to be defined later
class CalcValue;
class UserVar;
namespace vars {
	extern CalcValue* valueAtVar(UserVar* first, char name[USERVAR_NAME_MAXLENGHT]);
	extern UserVar* findVar(UserVar* first, char* name);
}


/// a temporary value to hold user data in
class CalcValue {

public:

	// which type is represented by the data union
	enum { NUM,	// number/boolean
		   STR,	// string
		   REF,	// reference to a variable
		   //ARR,	// linked-list
		   BLK	// Block of code (StrStack) (subroutine) (executable array)
		 } type;

	// contains the data
	union {
		double		number;
		char*		string;
		StrStack*	block;
	};

  	// Null object
	CalcValue(): type(STR), string(NULL) {}


  	CalcValue (double val):	type(NUM) {
		number = val;
		type = NUM;
	}


	CalcValue(const char* const str): type(STR) {

		type = STR;

		if (str) {
			// allocate memory for the string
		  	string = (char*) malloc(strlen(str) + 1);

			// write the string to the buffer
			strcpy(string, str);

		// declaring a NULL_CALCVAL_OBJECT
		} else
			string = NULL;
	}

	CalcValue(const StrStack& codeBlock): type(BLK)
		{ block = new StrStack(codeBlock); }

	CalcValue(StrStack* codeBlock): type(BLK)
		{ block = new StrStack(*codeBlock); }

	CalcValue(const CalcValue& in){
		// no need to delete anything as nothing is there yet

		// they will be the same type of data
		type = in.type;

		// copy in the value
		if (type == NUM)
		  	number = in.number;
		else if (type == STR || type == REF) {

			if (in.string) {
			  	string = (char*) malloc(strlen(in.string) + 1);
				strcpy(string, in.string);
			} else
				string = NULL;

		} else if (type == BLK)
			block = new StrStack(*in.block);

		//printf("copying CV...\n");
		//printf("copyied CV...\n");
	}

	// lol
	template<class T>
	CalcValue& operator=(const T& val) {
		setValue(val);
		return *this;
	}

	void setValue(const CalcValue& in){

		// delete old value
		if (type == STR || type == REF)
		  	free(string);
		else if (type == BLK)
			delete block;

		// they will be the same type of data
		type = in.type;

		// copy in the value
		if (type == NUM)
		  	number = in.number;
		else if (type == STR || type == REF) {

			if (in.string) {
			  	string = (char*) malloc(strlen(in.string) + 1);
				strcpy(string, in.string);
			} else
				string = NULL;

		} else if (type == BLK)
			block = new StrStack(*in.block);

	}

	// this causes a core dump (QwQ)
	~CalcValue(){
		//printf("deleting CV...\n");
		if (type == STR || type == REF)
			free(string); // free() accepts NULL pointers
		else if (type == BLK)
			delete block;
		//printf("deleted CV...\n");
	}

	void setValue(const char* const str) {

		// memory leaks are pretty bad
		if (type == STR || type == REF)
			free(string); // free() accepts NULL pointers
		else if (type == BLK)
			delete block;

		string = (char*) malloc(strlen(str) + 1);

		// write the string to the buffer
		strcpy(string, str);

		type = STR;

	}

	void setValue(const char ch){

		// memory leaks are pretty bad
		if (type == STR || type == REF)
			free(string); // free() accepts NULL pointers
		else if (type == BLK)
			delete block;

		type = STR;
		string = (char*) malloc(2);
		*string = ch;
		*(string + 1) = '\0';

	}
	void setValue(double val){

		// delete old value
		if (type == STR || type == REF)
			free(string); // free(NULL) gives no errors :)
		else if (type == BLK)
			delete block;

		type = NUM;
		number = val;
	}


	CalcValue& setRef(const char* const str){

		// memory leaks are pretty bad
		if (type == STR || type == REF)
			free(string); // free(NULL) gives no errors :)


		string = (char*) malloc(strlen(str) + 1);

		// write the string to the buffer
		strcpy(string, str);

		type = REF;

		return *this;
	}
	CalcValue& setStr(const char* const str){
		// memory leaks are pretty bad
		if (type == STR || type == REF)
			free(string); // free(NULL) gives no errors :)


		string = (char*) malloc(strlen(str) + 1);

		// write the string to the buffer
		strcpy(string, str);

		type = STR;

		return *this;

	}

	void setNull(){

		if (type == STR || type == REF)
			free(string);

		type = STR;
		string = NULL;

	}


	double getNum(){
		if (type == NUM)
			return number;
		else if (type == STR)
			return strlen(string);
		else
			return 0;
	}

	char* getStr(){
	  	if (type == STR)
			return string;
	  	else
		  	return NULL;
	}

	char* getRef(){
	  	if (type == REF)
			return string;
	  	else
		  	return NULL;
	}

	CalcValue* valAtRef(UserVar* first){
		if (type == REF)
			return vars::valueAtVar(first, string);
		else
			return NULL;
	}

	bool operator==(const CalcValue& val2){

		// same type
		if (type == val2.type) {

			// same value
			if (type == NUM && number == val2.number)
				return true;
			else if ((type == STR || type == REF) && strcmp(string, val2.string) == 0)
				return true;

		}

	  	return false;

	}

	// A Null value
	bool isEmpty()
		{ return (type == STR) && (string == NULL); }
	bool isNull()
		{ return isEmpty(); }

	// type checking (doesn't get used) :/
	bool isRef()
		{ return type == REF; }
	bool isStr()
		{ return !isEmpty() && type == STR; }
	bool isNum()
		{ return type == NUM; }
	bool isBlk()
		{ return type == BLK; }

};


#define NULL_CALCVAL_OBJECT CalcValue((char*) NULL)


#endif
