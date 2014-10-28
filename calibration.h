int value_sum();
void calibrate();


void calibration_init(){
	if(file_exists){
		frame_stay("Found Profile!\n\n  Pad connected:\n     %s\n     %d Axis\n     %d Buttons", pad_connected, num_of_axis, num_of_buttons);
		frame_print("Press Button 1 for recalibration .", 1, 1);
		//printf("\n\nFound Profile!\n\nPad connected:\n   %s\n   %d Axis\n   %d Buttons\n\nPress Button 1 for recalibration.\n\n", pad_connected, num_of_axis, num_of_buttons);
		int i = 0;
		buzzer_short(1);
		while(i < 1000){
			if(button_pushed() == 0){
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
		if(i == 1){calibrate();}else{while(1){if(!value_sum(0)){break;}} frame_print("", 1, 0);}
	}else{
		buzzer_short(5);
		frame_stay("No Profile found!\n\n  Pad connected:\n     %s\n     %d Axis\n     %d Buttons", pad_connected, num_of_axis, num_of_buttons);
		calibrate();
	}
}

void calibrate(void){
	frame_print("Please calibrate. Begin with Layout, assign buttons and axis:\n", 1, 0);
	int i = 0;
	while(i < NUMBER_OF_AXIS){
			frame_print("\n    (Axis) ", 0, 0);
			frame_print(lablesAxis[i], 0, 0);
			int u = -1;
			int axis_default = 0;
			while(1){if(!value_sum(1)){break;}}
			while(u == -1){
				for (int p = 0; p < num_of_axis; p++)
				{
					get_gamepad();
					if(u == -1 && axis[p] != axis_default){u = p;}
				}
			}
			layout[i] = u;
			i++;
			frame_print("		check", 0, 0);
			buzzer_short(1);
	}
	while(i < (NUMBER_OF_BUTTONS + NUMBER_OF_AXIS)){
			frame_print("\n    (Button) ", 0, 0);
			frame_print(lablesButtons[i - NUMBER_OF_AXIS], 0, 0);
			int u = -1;
			while(1){if(!value_sum(0)){break;}}
			while(u == -1){
				u = button_pushed();
			}
			layout[i] = u;
			i++;
			frame_print("	check", 0, 0);
			buzzer_short(1);
	}
	while(1){if(!value_sum(0)){break;}}
	frame_print("Press Button 1 to continue...", 1, 1);
	while(!button_pushed() == 0){};
	buzzer_short(1);
	frame_print("To calibrate the maximum value of the axis, \n  pull and hold the assigned CamNick axis at the maximum \n  and store the value by pressing any button.\n\n    Maximum Value of Nick axis: ", 1, 0);
	while(1){if(!value_sum(0)){break;}}
		while(1){
			get_gamepad();
			if(value_sum(0))
			{
				axisValue = axis[layout[0]];
				break;	
			}
		}
		buzzer_short(1);
		fflush(stdout);
	frame_print("	check", 0, 0);
	write_file();
	frame_print("", 1, 0);
}

int value_sum(int q){
	get_gamepad();
	int tmp = 0;
	if(q)
	{
		for (int i = 0; i < num_of_axis; ++i)
		{
			tmp += axis[i];
		}
	}
	if(!q)
	{
		for (int i = 0; i < num_of_buttons; ++i)
		{
			tmp += button[i];
		}
	}
	return(tmp);
}