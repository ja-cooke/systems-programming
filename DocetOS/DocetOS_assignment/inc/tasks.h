#ifndef TASKS_H
#define TASKS_H

#include <stdint.h>

#include "mode_utils.h"
#include "OS/os.h"
#include "OS/sleep.h"
#include "OS/mutex.h"
#include "Utils/utils.h"
#include "OS/mempool.h"
#include "OS/queue.h"

/* Example data packet structure, just for demonstration purposes */
typedef struct {
	uint32_t id;
	char data[12];
} packet_t;

void initialiseOSResources(void);
void task1(void const *const args);
void task2(void const *const args);
void task3(void const *const args);

void sleepTest1(void const *const args);
void sleepTest2(void const *const args);

#endif /* TASKS_H */