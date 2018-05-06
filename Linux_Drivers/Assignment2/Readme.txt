Author - Sasanka Sekhar Dash
Assignment2 - Device Drivers
BITS Pilani
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Procedure to Compile and run the code.Commands are mentioned below.
	1) make all 
	2) sudo insmode kernel_ioport.ko
		2.a) If you want to see the sleep / delay functionality in kernel space , then uncomment the code mentioned
                     in kernel_ioport.c file and inside the char_driver_init function.
		2.b) Also the same thing can be achieved using the function "sleep_ms_kernel_space".It take the delay input
                     in milliseconds.
	3) To compile the user space program write "make compile".
	4) To run the user space program write "make run".
		4.a) Also the user space program has functionality to introduce sleep using the sleep function designed for 
		     kernel spce.(The delay function has been commented in user space program)
	5) To remove the module write "sudo rmmode kernel_ioport"

Note:-
     1)Here channel2 and mode 0 has been used to generate the 1millisecond delay.In mode 0, the counter starts decrementing 
       from the load counter value till zero and at the same time the output pin(5th bit of port 61) remains zero and once 
       the counter reaches 0, the output pin goes to one and maintains one till the next load value is loaded.

***  2)The counter value fetched may be any value between 0 to 65535. As in mode 0, after the counter goes to zero from load
       value(here 1192),the counter presets to the maximum value (here 65535)and starts decrementing till the next load value
       getting loaded in the appropriate port (for channel2 it is 0x42).


--------------------------------------------------------------------(Must read Note - 2)--------------------------------------------------------------------------------------
