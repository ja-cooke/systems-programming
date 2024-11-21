#include "mode_utils.h"

/* Report the state of the CPU using the PSR and CONTROL registers */
void reportState(){
	
	uint32_t psr;
	psr = getPSR();
	
	/* Determine whether CPI is in thread or handler mode */
	uint32_t bitmask_PSR;
	bitmask_PSR = 0x1FF;
	
	psr = psr & bitmask_PSR; // check bits 0:8 (Masks all others)
	
	/* Determine whether MSP or PSP is the active stack pointer */

	uint32_t control;
	control = getCONTROL();
	
	uint32_t spsel;
	uint32_t npriv;
	
	uint32_t bitmask_spsel, bitmask_npriv;
	bitmask_spsel = 0b10;
	bitmask_npriv = 0b01;
	
	spsel = control & bitmask_spsel;
	npriv = control & bitmask_npriv;
	
	/* Determine the CPU state and print the status */
	char mode[8];
	char privilage[13];
	char stack_pointer[4];
	
	if (psr == 0){
		strcpy(mode, "Thread");
	}
	else{
		strcpy(mode, "Handler");
	}
	
	if (npriv == 0){
		strcpy(privilage, "privilaged");
	}
	else{
		strcpy(privilage, "unprivilaged");
	}
	
	if (spsel == 0){
		strcpy(stack_pointer, "MSP");
	}
	else{
		strcpy(stack_pointer, "PSP");
	}
	
	printf("%s mode, %s, %s in use.\r\n", mode, privilage, stack_pointer);
	
}