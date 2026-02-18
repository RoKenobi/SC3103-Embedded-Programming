Questions
1. Timed Blinking: (3 marks)
   
a. Read an integer input from the keyboard in your PC using USART and display the
string: "I will blink every X seconds", on the OLED display in your board. Here X is
the input entered in the keyboard. (1 mark)

b. Use a software timer and change the period of the timer based on the input X. The
LED now turns off for 1 second after every X second i.e., when the software timer
expires, this happens on repeat. (1 mark)

c. The micro-controller also sends a string "I am blinking" to your Tera Term terminal
in PC every X seconds, this also happens on repeat. (1 mark)

2. Spontaneous Singing: The buzzer in the board plays a tune (given in
Appendix) while the button is pressed and stays silent when no button is
pressed. (1 mark)

3. Run both tasks together. Observe what happens when you press the
button (for Spontaneous Singing), while Timed Blinking is running.
Observe what happens when priorities of Tasks are changed. (1 mark)
