#include "main.h"
#include "calibration.h"
#include "frame.h"
#include "file.h"
#include "gpio.h"


void calibrate(void){
	frame_print("Please calibrate. Begin with Layout, assign buttons and axis:\n", 1, 0);
	int i = 0;
	while(i < NUMBER_OF_AXES_USED){
			frame_print("\n    (Axis) ", 0, 0);
			frame_print(LablesOfAxis[i], 0, 0);
			int u = -1;
			int axisDefault = 0;
			while(1){if(!value_sum(1)){break;}}						//Wait until all buttons released
			while(u == -1){
				for (int p = 0; p < NumberOfAxesAvailiable; p++)
				{
					read_gamepad_data();
					if(u == -1 && Axis[p] != axisDefault){u = p;}
				}
			}
			GamepadLayout[i] = u;
			i++;
			frame_print("		check", 0, 0);
			buzzer_short(1);
	}
	while(i < (NUMBER_OF_BUTTONS_USED + NUMBER_OF_AXES_USED)){
			frame_print("\n    (Button) ", 0, 0);
			frame_print(LablesOfButtons[i - NUMBER_OF_AXES_USED], 0, 0);
			int u = -1;
			while(1){if(!value_sum(0)){break;}}
			while(u == -1){
				u = check_if_button_pushed();
			}
			GamepadLayout[i] = u;
			i++;
			frame_print("	check", 0, 0);
			buzzer_short(1);
	}
	while(1){if(!value_sum(0)){break;}}		
	frame_print("Press Button 1 to continue...", 1, 1);
	while(!check_if_button_pushed() == 0){};	
	buzzer_short(1);
	frame_print("To calibrate the maximum value of the axis, \n  pull and hold the assigned CamNick axis at the maximum \n  and store the value by pressing any button.\n\n    Maximum Value of Nick axis: ", 1, 0);
	while(1){if(!value_sum(0)){break;}}
		while(1){
			read_gamepad_data();
			if(value_sum(0))
			{
				MaxAxisValue = Axis[GamepadLayout[0]];
				break;	
			}
		}
		buzzer_short(1);
		fflush(stdout);
	frame_print("	check", 0, 0);
	write_file();
	frame_print("", 1, 0);
}

//>> Ensuring that no button is being pushed and no axis moved
//------------------------------------------------------------------------------------------------------
int value_sum(int q){
	read_gamepad_data();
	int tmp = 0;
	if(q)
	{
		for (int i = 0; i < NumberOfAxesAvailiable; ++i)
		{
			tmp += Axis[i];
		}
	}
	if(!q)
	{
		for (int i = 0; i < NumberOfButtonsAvailiable; ++i)
		{
			tmp += Button[i];
		}
	}
	return(tmp);
}

//>> Initializing calibration of Gamepad
//------------------------------------------------------------------------------------------------------
void calibration_init(){
	if(FileExists){
		frame_stay("Found Profile!\n\n  Pad connected:\n     %s\n     %d Axis\n     %d Buttons", GamepadConnected, NUMBER_OF_AXES_USED, NUMBER_OF_BUTTONS_USED);
		frame_print("Press Button 1 for recalibration .", 1, 1);
		int i = 0;
		buzzer_short(1);
		while(i < 1000){
			if(check_if_button_pushed() == 0){
				i = 1;
				buzzer_short(3);
				break;
			}
			usleep(5000);
			if(!(i%100)){
				frame_print(" .", 0, 1);
				buzzer_short(1);
			}
			i++;
		}
		if(i == 1)
			{
				calibrate();
				frame_print("START", 1, 1);
			}else
			{
				while(1){if(!value_sum(0)){break;}
			}
			frame_print("START", 1, 1);
		}
	}else{
		buzzer_short(5);
		frame_stay("No Profile found!\n\n  Pad connected:\n     %s\n     %d Axis\n     %d Buttons", GamepadConnected, NUMBER_OF_AXES_USED, NUMBER_OF_BUTTONS_USED);
		calibrate();
		frame_print("START", 1, 1);
	}
}