#ifndef MUTEX_H
#define MUTEX_H

#include "OS/os.h"

typedef struct s_OS_mutex_t {
	struct s_OS_TCB_t *tcb;
	uint32_t counter;
} OS_mutex_t;

#define OS_MUTEX_STATIC_INITIALISER {.tcb = 0, .counter = 0}

void OS_mutex_aquire(OS_mutex_t * mutex);
void OS_mutex_release(OS_mutex_t * mutex);

#endif /* MUTEX_H */