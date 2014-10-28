#include <curses.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>		
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "frame.h"
#include "types.h"
#include "main.h"
#include "file.h"
#include "uart.h"
#include "gamepad.h"
#include "calibration.h"
#include "gpio.h"

int main(void){
	frame_init();
    file_exists = file_init();
	gamepad_init();
	gpio_init();
	calibration_init();
	uart_init();

	buzzer_short(3);
	
	for (;;)
	{
		get_gamepad();
		data_package_st data_package_serv;
		prepare_data_servo(&data_package_serv);
		encode(&data_package_serv);
		tx(&data_package_serv);
		
		usleep(5000); //200hz
	}
}