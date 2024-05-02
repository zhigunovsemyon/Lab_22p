#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAXLEN 2560
#define SCANF_SPEC_S "%2559s"

enum Switches
{
	QUIT,
	KEYBOARD,
	READ_FILE,
	REMOVE,
	RM_PARENTHESES,
	COUNT_SPACE,
	RM_COMMENT_BLOCK,
	PRINT
};

void GetAndReadFile(char String[]);
uint8_t Switcher(char String[]);
uint32_t FileLen(FILE* f, uint32_t maxLen);
uint8_t CountMaxSpace(char String[]);
void RemoveParentheses(char String[]);
void RemoveCommentBlock(char String[]);