#ifndef _FILE_H
 #define _FILE_H

#define NUMBER_OF_AXES_USED      8
#define NUMBER_OF_BUTTONS_USED   10

extern u8 GamepadLayout[30];
extern int MaxAxisValue;
extern const char* LablesOfAxis[NUMBER_OF_AXES_USED];
extern const char* LablesOfButtons[NUMBER_OF_BUTTONS_USED];
extern int file_init();
extern void write_file(void);
extern u8 FileExists;

#endif //_FILE_H