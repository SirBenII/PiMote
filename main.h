#ifndef _MAIN_H
 #define _MAIN_H

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
#include "file.h"
#include "uart.h"
#include "gamepad.h"
#include "calibration.h"
#include "gpio.h"

extern void usleep(int);

#endif //_MAIN_H