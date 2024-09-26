#include "Utils/utils.h"
#include <stdio.h>

static int wordcount(char * string) {
	// Set up a while loop to loop over the whole string
	
	int counting = 1;
	int stringIndex = 0;
	int wordBreak = 1;
	int wordTotal = 0;
	
	while(counting){
		char currentChar = string[stringIndex];
		
		// ---- LOGIC FOR IF WORD CHARACTER OR NOT ----
		
		int wordCharacter;
		
		// Capture numerals
		if ((currentChar >= '0') && (currentChar <= '9')) {
			wordCharacter = 1;
		}
		// Capture capital letters
		else if ((currentChar >= 'A') && (currentChar <= 'Z')) {
			wordCharacter = 1;
		}
		// Capture lower case letters
		else if ((currentChar >= 'a') && (currentChar <= 'z')) {
			wordCharacter = 1;
		}
		// Capture termination
		else if (currentChar == 0) {
			counting = 0;
			wordCharacter = 0;
		}
		else {
			wordCharacter = 0;
			wordBreak = 1;
		}
		
		// ---- LOGIC FOR IF NEW WORD OR NOT ----
		
		if (wordCharacter && wordBreak){
			wordTotal++;
			wordBreak = 0;
		}
		
		stringIndex++;
	}
	
	
	int words = wordTotal;

	return words;
}

int main(void) {
	
	configClock();
	configUSART2(38400);
	
	// This text contains 11 words.
	char * text = "-- -. ..A seperate test for word count. 9th November 1885; did thusly the spectre of ha8832ha speak--";
	printf("The text is:\n%s\n", text);
	printf("It contains %d words.\n", wordcount(text));

	while(1);
}
