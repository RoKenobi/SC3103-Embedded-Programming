#ifndef __USERBUTTON_H
#define __USERBUTTON_H
#include "sys.h"
#include "system.h"

//#define USERBUTTON_TASK_PRIO    3     //Task priority 
#define USERBUTTON_STK_SIZE     128   //Task stack size 


/*--------UserButton control pin--------*/
#define UserButton_PORT GPIOD
#define UserButton_PIN GPIO_Pin_8
#define UserButton PDin(8) 
/*----------------------------------*/

void UserButton_Init(void);  
void UserButton_task(void *pvParameters);
#endif
