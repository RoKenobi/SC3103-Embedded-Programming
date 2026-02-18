#include "system.h"


void systemInit(void)
{       

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	delay_init(168);			                                            
	
	//Initialize the hardware interface connected to the LED lamp
	LED_Init(); 
  BUZZER_Init();
  OLED_Init();
  uart3_init(115200);
	UserButton_Init();
}
