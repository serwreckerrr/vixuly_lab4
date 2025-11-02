/*
 * scheduler.c
 *
 *  Created on: Nov 1, 2025
 *      Author: Admin
 */
#include "scheduler.h"

static sTask *SCH_head = NULL;

void SCH_Init(void) {
    SCH_head = NULL;
}

void SCH_Add_Task(void (*pFunction)(void), uint32_t DELAY, uint32_t PERIOD) {
    sTask *newTask = (sTask *)malloc(sizeof(sTask));
    if (newTask == NULL) return;

    newTask->pTask = pFunction;
    newTask->Delay = DELAY;
    newTask->Period = PERIOD;
    newTask->RunMe = 0;
    newTask->next = NULL;

    if (SCH_head == NULL) {
        SCH_head = newTask;
        return;
    }

    sTask *current = SCH_head;
    sTask *previous = NULL;

    //find insert point
    while (current != NULL && DELAY >= current->Delay) {
        DELAY -= current->Delay;
        previous = current;
        current = current->next;
    }

    newTask->Delay = DELAY;

    if (previous == NULL) { // insert at head
        newTask->next = SCH_head;
        SCH_head->Delay -= newTask->Delay;
        SCH_head = newTask;
    } else {
        newTask->next = current;
        previous->next = newTask;
        if (current != NULL) current->Delay -= newTask->Delay;
    }
}

void SCH_Update(void) {
    if (SCH_head == NULL) return;

    if (SCH_head->Delay > 0) {
        SCH_head->Delay--;
    }

    if (SCH_head->Delay == 0) {
        SCH_head->RunMe++;
    }
}

void SCH_Dispatch_Tasks(void) {
    while (SCH_head != NULL && SCH_head->RunMe > 0) {
        SCH_head->RunMe--;

        void (*taskFunc)(void) = SCH_head->pTask;
        uint32_t period = SCH_head->Period;

        // Save current task pointer
        sTask *temp = SCH_head;

        // Remove from head
        SCH_head = SCH_head->next;
        if (SCH_head != NULL)
            SCH_head->Delay += temp->Delay;

        // Execute
        (*taskFunc)();

        // Re-add periodic task
        if (period > 0) {
            SCH_Add_Task(taskFunc, period, period);
        }

        free(temp);
    }
}


void SCH_Delete(sTask *task) {
    if (SCH_head == NULL || task == NULL) return;

    sTask *current = SCH_head;
    sTask *previous = NULL;

    while (current != NULL) {
        if (current == task) {
            if (previous == NULL) { // delete head
                SCH_head = current->next;
                if (SCH_head != NULL)
                    SCH_head->Delay += current->Delay;
            } else {
                previous->next = current->next;
                if (current->next != NULL)
                    current->next->Delay += current->Delay;
            }
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}





























///*
// * scheduler.c
// *
// *  Created on: Nov 1, 2025
// *      Author: Admin
// */
//#include "scheduler.h"
//
//sTasks SCH_tasks_G[SCH_MAX_TASKS];
//uint8_t current_index_task = 0;
//
//void SCH_Init(void){
//	current_index_task = 0;
//}
//
//void SCH_Add_Task(void(*pFunction)(), uint32_t DELAY, uint32_t PERIOD){
//	if(current_index_task < SCH_MAX_TASKS){
//
//		SCH_tasks_G[current_index_task].ptask = pFunction;
//		SCH_tasks_G[current_index_task].Delay = DELAY;
//		SCH_tasks_G[current_index_task].Period = PERIOD;
//		SCH_tasks_G[current_index_task].RunMe = 0;
//
//		SCH_tasks_G[current_index_task].TaskID = current_index_task;
//
//		current_index_task++;
//	}
//}
//
//void SCH_Update(void){
//	for(int i= 0; i < current_index_task; i++){
//		if(SCH_tasks_G[i].Delay > 0){
//			SCH_tasks_G[i].Delay--;
//		}else{
//			SCH_tasks_G[i].Delay = SCH_tasks_G[i].Period;
//			SCH_tasks_G[i].RunMe += 1;
//		}
//	}
//}
//
//void SCH_Dispatch_Tasks(void){
//	for(int i = 0; i < current_index_task; i++){
//		if(SCH_tasks_G[i].RunMe > 0){
//			SCH_tasks_G[i].RunMe--;
//			(*SCH_tasks_G[i].ptask)();
//			//delete
//			if(SCH_tasks_G[i].Period == 0){
//				SCH_Delete(SCH_tasks_G[i].TaskID);
//			}
//		}
//	}
//}
//
//void SCH_Delete(uint32_t ID){
//	if (ID >= current_index_task) return;
//
//	for (uint32_t i = ID; i < current_index_task - 1; i++) {
//		SCH_tasks_G[i] = SCH_tasks_G[i + 1];
//		SCH_tasks_G[i].TaskID = i;
//	}
//	current_index_task--;
//}
