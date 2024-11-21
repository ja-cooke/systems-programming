#include "mode_utils.h"

/* Report the state of the CPU using the PSR and CONTROL registers */
void reportState(){
	
	uint32_t psrCheck;
	uint32_t bitmask_PSR;
	psrCheck = 0;
	bitmask_PSR = 0x1FF;
	
	uint32_t psr;
	psr = getPSR();
	
	/* Determine whether CPI is in thread or handler mode */
	psr = psr & bitmask_PSR; // check bits 0:8 (Masks all others)
	psrCheck = psr && 0; 
	
	uint32_t control;
	control = getCONTROL();
	
	
}