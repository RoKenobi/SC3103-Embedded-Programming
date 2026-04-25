# Timed Button Response System

* Green LED stays ON for 2 seconds at the start of each cycle.
* Then a random time from 3 to 10 seconds is generated and the Yellow LED turns ON.
* The button must be pressed while the Yellow LED is active.
* If pressed in time, the buzzer turns ON and the Red LED lights up for 2 seconds.
* If no button press is detected before timeout, the system returns to the initial Green state.

## Task

Implement this system in:

1. User Space (2.5 marks)
2. Loadable Kernel Module (2.5 marks)

