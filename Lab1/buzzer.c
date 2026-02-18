#include "buzzer.h"


#define BUZZER_ON_TIME   pdMS_TO_TICKS(8000)  // 2 LED cycles (2s ON + 2s OFF each)
#define BUZZER_OFF_TIME  pdMS_TO_TICKS(4000)  // 1 LED cycle (2s ON + 2s OFF)

void BUZZER_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = BUZZER_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

    GPIO_Init(BUZZER_PORT, &GPIO_InitStructure);

    GPIO_ResetBits(BUZZER_PORT, BUZZER_PIN);   // buzzer OFF
}

void buzzer_task(void *pvParameters){
    while(1)
    {
        BUZZER = 1;                    // Turn buzzer ON
        vTaskDelay(BUZZER_ON_TIME);    // Keep ON for 2 LED cycles (8s)

        BUZZER = 0;                    // Turn buzzer OFF
        vTaskDelay(BUZZER_OFF_TIME);   // Keep OFF for 1 LED cycle (4s)
    }
}


