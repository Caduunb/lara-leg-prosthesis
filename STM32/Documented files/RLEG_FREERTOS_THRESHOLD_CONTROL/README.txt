READ ME file.

The program RLEG_FREERTOS_THRESHOLD_CONTROL implements a finite state machine with two actuation states. 

s == 0 means no actuation;

s == 1 means fixed actuation, with 20% PWM dutycycle.

The program defines the state based on angular velocity calculated from encoder readings.
If angular velocity is above UPPER_OMEGA and state == 0 (no actuation), the actuation state is changed to 1.
If angular velocity is below LOWER_OMEGA and state == 1 (fixed actuation), the actuation state is changed to 0.
The sampling time is 100 ms.
PWM frequency is set to 72 kHz.


For better system performance, activate PWM in steps until max PWM definition (20% in this program).

