READ ME file.

This program has 3 tasks:

1. Read value from current sensor
2. Write data to SD card
3. Generate a PWM signal of a constant dutycyle for 5 seconds. In steps, the pwm generated goes from 1 to 8, turning off in between each activation. Therefore, this program implements a 'step' input of different amplitudes. 

The main idea is to obtain the current sensor readings for each step input.

Check program sampling times, as transient response should be much faster than the previously defined sampled time, but setting a sampling time that is too small could crash the application.

tips:

#set faster baud rate for uart transmission
#observe analog output of current sensor with oscilloscope

