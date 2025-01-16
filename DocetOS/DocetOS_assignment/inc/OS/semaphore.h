#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "OS/os.h"

typedef struct s_OS_semaphore_t {
	volatile uint32_t available;
} OS_semaphore_t;

typedef struct s_OS_semBinary_t {
	volatile uint32_t available;
} OS_semBinary_t;

#define OS_SEMAPHORE_STATIC_INITIALISER {.available = 0}
#define OS_BINARY_SEMAPHORE_STATIC_INITIALISER {.available = 1}

void OS_semaphore_acquire(OS_semaphore_t *semaphore);
void OS_semaphore_release(OS_semaphore_t *semaphore);

void OS_semBinary_acquire(OS_semBinary_t *semaphore);
void OS_semBinary_release(OS_semBinary_t *semaphore);

#endif /* SEMAPHORE_H */