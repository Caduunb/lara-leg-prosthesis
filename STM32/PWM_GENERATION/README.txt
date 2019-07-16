READ ME file.

This program tests PWM generation.

PWM frequency is set as 72 kHz as standard, by setting 'htim2.Init.Prescaler' to the value of 9. To change PWM frequency, change this parameter accordindly, as:

PWM_frequency = (TIMER_frequency/PWM_steps)/(htim2.Init.Prescaler+1)

PWM steps can be changed accordingly by setting the 'htim2.Init.Period' variable, as:

 
PWM_steps = htim2.Init.Period + 1

In the program you can generate a PWM signal with dutycycle set according to the average voltage output value. The dutycycle is set by the CCR register of the used TIMER, as in:

TIM2->CCR2 (timer 2, channel 2)










