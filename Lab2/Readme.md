## 📋 Assignment Overview

You need show these tasks to the TA.

### 1. Timed Blinking
- **1.1 USART & OLED Input:** Read an integer input `X` from the PC keyboard via USART. Display the string `"I will blink every X seconds"` on the onboard OLED display.
- **1.2 Software Timer & LED:** Use a software timer to toggle an LED. The LED turns **OFF for 1 second** after every `X` seconds. This cycle repeats upon timer expiration.
- **1.3 Serial Feedback:** The microcontroller sends the string `"I am blinking"` to the Tera Term terminal on the PC every `X` seconds (synchronized with the timer).

### 2. Spontaneous Singing
- **Buzzer Control:** The onboard buzzer plays a predefined tune (see Appendix) **while** a specific button is pressed.
- **Silence:** The buzzer remains silent when the button is not pressed.

### 3. Concurrency & Priorities
- **Multi-tasking:** Run both Task 1 (Blinking) and Task 2 (Singing) simultaneously.
- **Observation:** Analyze system behavior when the button is pressed while the blinking task is active.
- **Priority Testing:** Observe and document changes in system behavior when the priorities of the tasks are swapped.
