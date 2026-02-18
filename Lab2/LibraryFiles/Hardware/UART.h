#ifndef __UART_H
#define __UART_H
#include "sys.h"
#include "system.h"


void uart_init(u32 baudRate);
void usart3_send(u8 data);
void uart_task(void *pvParameters);
void uart3_send_name();
int USART3_IRQHandler(void);
void uart_task_receive(void *pvParameters);

extern TimerHandle_t xAutoTimer ;
extern TimerHandle_t uartTimer;

#endif

