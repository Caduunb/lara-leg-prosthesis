This program tests Analog-to-Digital conversion reading.

ADC readings are used to obtain the analog output value of the current sensor.
The read value is checked by the debug tool, using the ST-link debugger. 
You can observe a variable value using debug mode and adding the variable to "watch" expressions.

To obtain value in interrupt mode, stop debug execution to check last conversion value, as it doesnt work as a real time tool.

The ADC read value goes from 0 to 4095, as it is stored in 12 bits.