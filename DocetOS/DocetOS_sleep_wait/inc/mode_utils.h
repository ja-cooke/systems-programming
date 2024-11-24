#ifndef MODE_UTILS_H
#define MODE_UTILS_H

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#define OS_INTERNAL

#include "OS/os.h"

/* Status of the Processor Status Register (PSR) */
uint32_t getPSR();

/* Status of the CONTROL register */
uint32_t getCONTROL();

/* Prototypes */
void reportState(void);

#endif /* MODE_UTILS_H */