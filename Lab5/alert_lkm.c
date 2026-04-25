#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/hrtimer.h>
#include <linux/random.h>

static struct timer_list my_timer;
static struct hrtimer bz_timer; // Needed for the actual sound
static int irq, state = 0, bz_on = 0, bz_state = 0;

static void set_pins(int g, int y, int r) {
    gpio_set_value(4, g); gpio_set_value(17, y); gpio_set_value(27, r);
}

/* This is the fix: It toggles pin 18 very fast to create the sound */
static enum hrtimer_restart bz_cb(struct hrtimer *t) {
    if(!bz_on) { gpio_set_value(18, 0); return HRTIMER_NORESTART; }
    bz_state = !bz_state;
    gpio_set_value(18, bz_state);
    hrtimer_forward_now(t, ktime_set(0, 250000)); // 2kHz pulse
    return HRTIMER_RESTART;
}

static void t_cb(struct timer_list *t) {
    if (state == 0) {
        unsigned char d; 
        int delay; /* Declare both variables at the very top */
        get_random_bytes(&d, 1); /* Now run the code */
        delay = (d % 8) + 3;
        set_pins(0, 1, 0);
        state = 1;
        printk(KERN_INFO "LKM: Yellow ON. Press button within %d seconds!\n", delay);
        mod_timer(&my_timer, jiffies + msecs_to_jiffies(delay * 1000));
    } else {
        if (state == 1) printk(KERN_INFO "LKM: Timeout! Missed the window.\n");
        bz_on = 0; // Turn off buzzer sound
        set_pins(1, 0, 0); 
        state = 0;
        printk(KERN_INFO "LKM: Resetting to Green for 2 seconds.\n");
        mod_timer(&my_timer, jiffies + msecs_to_jiffies(2000));
    }
}

static irq_handler_t btn_isr(unsigned int i, void *id, struct pt_regs *r) {
    if (state == 1) {
        set_pins(0, 0, 1); 
        bz_on = 1; // Start buzzer sound
        state = 2;
        printk(KERN_INFO "LKM: SUCCESS! Red & Buzzer ON for 2s.\n");
        hrtimer_start(&bz_timer, ktime_set(0, 250000), HRTIMER_MODE_REL);
        mod_timer(&my_timer, jiffies + msecs_to_jiffies(2000));
    }
    return (irq_handler_t)IRQ_HANDLED;
}

static int __init my_init(void) {
    int ret;
    gpio_request(4, ""); gpio_direction_output(4, 0);
    gpio_request(17, ""); gpio_direction_output(17, 0);
    gpio_request(27, ""); gpio_direction_output(27, 0);
    gpio_request(18, ""); gpio_direction_output(18, 0);
    gpio_request(11, ""); gpio_direction_input(11);
    
    irq = gpio_to_irq(11);
    ret = request_irq(irq, (irq_handler_t)btn_isr, IRQF_TRIGGER_RISING, "btn", NULL);
    
    hrtimer_init(&bz_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    bz_timer.function = bz_cb;
    timer_setup(&my_timer, t_cb, 0);
    
    state = 0;
    set_pins(1, 0, 0);
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(2000));
    return 0;
}

static void __exit my_exit(void) {
    del_timer_sync(&my_timer);
    bz_on = 0;
    hrtimer_cancel(&bz_timer);
    free_irq(irq, NULL);
    gpio_free(4); gpio_free(17); gpio_free(27); gpio_free(18); gpio_free(11);
}

module_init(my_init); 
module_exit(my_exit); 
MODULE_LICENSE("GPL");