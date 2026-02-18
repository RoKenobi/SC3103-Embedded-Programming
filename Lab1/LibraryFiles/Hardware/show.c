#include "show.h"
#define RATE_1_HZ 1

void show_task(void *pvParameters)
{
   u32 lastWakeTime = getSysTickCnt();
   while(1)
    { 

   vTaskDelayUntil(&lastWakeTime, F2T(RATE_1_HZ));

   oled_show();    
    }
}  

static void dec_to_octal_2digit(uint8_t dec, char *buf) //helper function for decimal to octal
{
    uint8_t high = dec / 8;
    uint8_t low  = dec % 8;

    buf[0] = high + '0';
    buf[1] = low  + '0';
}


void oled_show(void)
{
    static uint32_t elapsed_sec = 0;

    uint8_t sec;
    uint8_t min;
    char time_str[6];   // "MM:SS"
    char mm[2];
    char ss[2];

    elapsed_sec++;   // called every 1 second

    sec = elapsed_sec % 60;
    min = (elapsed_sec / 60) % 60;

    // convert decimal to octal digits
    dec_to_octal_2digit(min, mm);
    dec_to_octal_2digit(sec, ss);

    time_str[0] = mm[0];
    time_str[1] = mm[1];
    time_str[2] = ':';
    time_str[3] = ss[0];
    time_str[4] = ss[1];
    time_str[5] = '\0';

    OLED_Clear();

    OLED_ShowString(16, 8,  (uint8_t *)"Octal Clock");
    OLED_ShowString(32, 32, (uint8_t *)time_str);

    OLED_Refresh_Gram();
}