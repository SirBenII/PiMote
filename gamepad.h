#ifndef _GAMEPAD_H
 #define _GAMEPAD_H

#define GAME_PAD "/dev/input/js0"

//Defines for external control
#define EC_VALID           0x01  // only valid if this is 1
#define EC_GAS_ADD         0x02  // if 1 -> use the GAS Value not as MAX
#define EC_IGNORE_RC       0x80  // if 1 -> for Flying without RC-Control 

extern void gamepad_init();
extern void read_gamepad_data();
extern void create_data_camera(serial_data_struct* pdata_package_camera_control, serial_data_struct* pdata_package_camera_control_gier);
extern void create_data_kopter(serial_data_struct* pdata_package_kopter_control);
extern int check_if_button_pushed();
extern void check_if_controls_active();
extern void check_program_switch();
extern void get_sensitivity();

extern volatile int *Axis;
extern volatile char *Button;
extern volatile float Sensitivity;
extern int NumberOfAxesAvailiable;
extern int NumberOfButtonsAvailiable;
extern char GamepadConnected[30];
extern u8 IsActive;
extern u8 ProgramSelect;

#endif //_GAMEPAD_H