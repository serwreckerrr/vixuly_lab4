/*
 * scheduler.h
 *
 *  Created on: Nov 1, 2025
 *      Author: Admin
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include <stdint.h>
#include <stdlib.h>

typedef struct sTask{
	void (*pTask)(void);
	uint32_t Delay;
	uint32_t Period;
	uint8_t RunMe;

	uint32_t taskid;

	struct sTask *next;
} sTask;


#define SCH_MAX_TASKS 40

void SCH_Init(void);
void SCH_Add_Task(void (*pFuntion)(),
						uint32_t DELAY,
						uint32_t PERIOD,
						uint32_t taskid);
void SCH_Update(void);

void SCH_Dispatch_Tasks(void);

void SCH_Delete(uint32_t id);
#endif /* INC_SCHEDULER_H_ */
