#ifndef MUTEX_H
#define MUTEX_H

#include "OS/os.h"

#define OS_MUTEX_STATIC_INITIALISER = {.tcb = 0, .counter = 0}

typedef struct s_OS_mutex_t {
	struct s_OS_TCB_t * tcb;
	uint32_t counter;
} OS_mutex_t;

void OS_mutex_aquire(OS_mutex_t * mutex, OS_TCB_t * current_tcb);
void OS_mutex_acquire_refactor(OS_mutex_t * mutex, OS_TCB_t * current_tcb);
void OS_mutex_release(OS_mutex_t * mutex, OS_TCB_t * current_tcb);

#endif /* MUTEX_H */