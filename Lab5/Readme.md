Timed Button Response System

. Green LED stays ON for 2 seconds at the start of each cycle.
· Then a random time from 3 to 10 seconds is generated and the yellow
LED turns ON.
· The button must be pressed while the yellow LED is active.
· If pressed in time, the buzzer turns ON and the red LED lights up for
2 seconds.
· If no button press is detected before timeout, the system returns to the
initial green state.

Implement this in both user space (2.5 marks) and as a Loadable Kernel
Module (2.5 marks).
