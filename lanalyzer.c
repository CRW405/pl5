/* front.c - a lexical analyzer system for simple
             arithmetic expressions */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

/* Global declarations */
/* Variables */
int charClass;
char lexeme[100];
int nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp;

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

/******************************************************/
/* main driver */
int main(int argc, char **argv) {
	/* Open input and prepare stream based on arguments:
	   - no args: read default file "front.in"
	   - one arg: treat arg as an expression string
	   - -f filename: read from the specified file
	*/
	if (argc == 1) {
		if ((in_fp = fopen("front.in", "r")) == NULL) {
			printf("ERROR - cannot open front.in \n");
			return 1;
		}
	} else if (argc == 2) {
		/* Single argument: expression string (not "-f") */
		if (strcmp(argv[1], "-f") == 0) {
			printf("ERROR - missing filename after -f\n");
			return 1;
		}
		in_fp = tmpfile();
		if (in_fp == NULL) {
			printf("ERROR - cannot create temporary input\n");
			return 1;
		}
		fprintf(in_fp, "%s\n", argv[1]);
		rewind(in_fp);
	} else {
		/* Expect "-f filename" */
		if (strcmp(argv[1], "-f") == 0 && argc >= 3) {
			if ((in_fp = fopen(argv[2], "r")) == NULL) {
				printf("ERROR - cannot open %s\n", argv[2]);
				return 1;
			}
		} else {
			printf("Usage: %s [\"expression\"] | -f filename\n", argv[0]);
			return 1;
		}
	}

	getChar();
	do {
		lex();
	} while (nextToken != EOF);

	return 0;
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses
            and return the token */

int lookup(char ch) {
	switch (ch) {
	case '(':
		addChar();
		nextToken = LEFT_PAREN;
		break;

	case ')':
		addChar();
		nextToken = RIGHT_PAREN;
		break;

	case '+':
		addChar();
		nextToken = ADD_OP;
		break;

	case '-':
		addChar();
		nextToken = SUB_OP;
		break;

	case '*':
		addChar();
		nextToken = MULT_OP;
		break;

	case '/':
		addChar();
		nextToken = DIV_OP;
		break;

	default:
		addChar();
		nextToken = EOF;
		break;
	}
	return nextToken;
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar() {
	if (lexLen <= 98) {
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = 0;
	} else {
		printf("Error - lexeme is too long \n");
	}
}

/*****************************************************/
/* getChar - a function to get the next character of
             input and determine its character class */

void getChar() {
	int c = getc(in_fp);
	if (c != EOF) {
		nextChar = c;
		if (isalpha(nextChar))
			charClass = LETTER;
		else if (isdigit(nextChar))
			charClass = DIGIT;
		else
			charClass = UNKNOWN;
	} else {
		nextChar = EOF;
		charClass = EOF;
	}
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it
                 returns a non-whitespace character */
void getNonBlank() {
	while (isspace(nextChar))
		getChar();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic
         expressions */
int lex() {
	lexLen = 0;
	getNonBlank();
	switch (charClass) {
		/* Parse identifiers */
	case LETTER:
		addChar();
		getChar();
		while (charClass == LETTER || charClass == DIGIT) {
			addChar();
			getChar();
		}
		nextToken = IDENT;
		break;

		/* Parse integer literals */
	case DIGIT:
		addChar();
		getChar();
		while (charClass == DIGIT) {
			addChar();
			getChar();
		}
		nextToken = INT_LIT;
		break;

		/* Parentheses and operators */
	case UNKNOWN:
		lookup(nextChar);
		getChar();
		break;

		/* EOF */
	case EOF:
		nextToken = EOF;
		lexeme[0] = 'E';
		lexeme[1] = 'O';
		lexeme[2] = 'F';
		lexeme[3] = 0;
		break;
	} /* End of switch */
	printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
	return nextToken;
} /* End of function lex */
