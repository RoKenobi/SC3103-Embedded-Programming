#include <fcntl.h>
#include <linux/gpio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#include <poll.h>

int main() {
    int fd = open("/dev/gpiochip0", O_RDWR);
    srand(time(NULL));

    /* [0]=Green, [1]=Yellow, [2]=Red, [3]=Buzzer */
    struct gpiohandle_request out = {.lines=4, .lineoffsets={4,17,27,18}, .flags=GPIOHANDLE_REQUEST_OUTPUT};
    ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &out);
    
    struct gpioevent_request btn = {.lineoffset=11, .handleflags=GPIOHANDLE_REQUEST_INPUT, .eventflags=GPIOEVENT_REQUEST_RISING_EDGE};
    ioctl(fd, GPIO_GET_LINEEVENT_IOCTL, &btn);
    
    struct gpiohandle_data d;
    struct gpioevent_data ev;

    while(1) {
        /* Phase 1: Green ON for 2s */
        d.values[0]=1; d.values[1]=0; d.values[2]=0; d.values[3]=0;
        ioctl(out.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &d);
        sleep(2);

        /* Phase 2: Yellow ON, Random 3-10s window */
        d.values[0]=0; d.values[1]=1;
        ioctl(out.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &d);
        
        int delay_ms = ((rand() % 8) + 3) * 1000;
        struct pollfd pfd = {.fd = btn.fd, .events = POLLIN};
        
        /* Flush old button presses before starting the timer */
        while(poll(&pfd, 1, 0) > 0) read(btn.fd, &ev, sizeof(ev));

        /* Wait for button press OR timeout */
        if (poll(&pfd, 1, delay_ms) > 0) { 
            /* Button pressed in time! */
            read(btn.fd, &ev, sizeof(ev)); 
            d.values[1]=0; d.values[2]=1; /* Yellow OFF, Red ON */
            
            /* Pulse the buzzer for 2 seconds (2000 loops * 1ms = 2s) */
            int buzz = 0;
            for(int i = 0; i < 2000; i++) {
                buzz = !buzz;
                d.values[3] = buzz;
                ioctl(out.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &d);
                usleep(1000);
            }
        }
        /* If no press, it just loops back to Green automatically */
    }
}