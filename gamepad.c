#include <linux/joystick.h>
#include "main.h"
#include "gamepad.h"
#include "frame.h"

volatile int *Axis = NULL;
volatile char *Button = NULL;
volatile float Sensitivity = 1.0;
int NumberOfAxesAvailiable;
int NumberOfButtonsAvailiable;
char GamepadConnected[30];
u8 IsActive = 0;
u8 ProgramSelect = 0;

int recordingButtonTemp = 125;
int padFd;
struct js_event js;

struct str_ExternControl externControl;			//struct for controlling MikroKopter
struct str_ExternControl externControlGier;		//struct for controlling Yaw (Gier) only

u8 gamepad_values_convert_for_cam(int tmpvalue){
	u8 tmp;
	double tmpdouble = tmpvalue;
	tmp = ((((tmpdouble/(MaxAxisValue))-1)*125)-131)*-1;
	if(tmp == 0){tmp = 255;}
	return(tmp);
}

//>> Converting gamepad values to values readable by a MikroKopter
//------------------------------------------------------------------------------------------------------
u8 gamepad_values_convert_for_kopter(int tmpvalue){
	u8 tmp;
	double tmpdouble = tmpvalue;
	tmp = ((((tmpdouble/(MaxAxisValue*Sensitivity))-1)*125)-131)*-1;
	/*Increase values exponentially
	if(tmpvalue < 0){
		tmpdouble = tmpvalue*-1;
	}else{tmpdouble = tmpvalue;}
	tmp = ((30497*tmpdouble*tmpdouble*tmpdouble)/11187226521286200+(10667778*tmpdouble*tmpdouble)/532725072442200+(2592340312*tmpdouble)/11187226521286);
	if(tmpvalue < 0){
		tmp = (tmp*-1)+255;
	}
	*/
	return(tmp);
}

u8 gamepad_values_convert_for_kopter_gas(int tmpvalue){
	u8 tmp;
	double tmpdouble = tmpvalue;
	tmp = ((((tmpdouble/(MaxAxisValue))-1)*125)-131)*-1;
	return(tmp);
}

int check_if_button_pushed(){
	read_gamepad_data();
	int buttonpushed = -1;
	for (int i = 0; i < NumberOfButtonsAvailiable; i++)
	{
		if(Button[i] != 0){
			buttonpushed = i;
		}
	}
	return(buttonpushed);
}

//>> Check if controls are active
//------------------------------------------------------------------------------------------------------
void check_if_controls_active(){
	if(check_if_button_pushed() == GamepadLayout[17] && IsActive == 0){
		IsActive = 1;
		while(1){if(!value_sum(0)){break;};}
	}else if(check_if_button_pushed() == GamepadLayout[17] && IsActive == 1){
		IsActive = 0;
		while(1){if(!value_sum(0)){break;};}
	}
}
//>> Check if program has been switched
//------------------------------------------------------------------------------------------------------
void check_program_switch(){
	if(check_if_button_pushed() == GamepadLayout[11]){
		frame_print("", 1, 0);
		ProgramSelect = 1;
	}else if(check_if_button_pushed() == GamepadLayout[12]){
		frame_print("", 1, 0);
		ProgramSelect = 2;
	}
}

//>> Check if sensitivity of the controls has been adjusted
//------------------------------------------------------------------------------------------------------
void get_sensitivity(){
	if(check_if_button_pushed() == GamepadLayout[15] && Sensitivity <= 2.0){
		if(Sensitivity < 2){buzzer_short(1);}
		Sensitivity += 0.2;
		while(1){if(!value_sum(0)){break;};}
	}else if(check_if_button_pushed() == GamepadLayout[16] && Sensitivity > 1.0){
		if(Sensitivity > 1){buzzer_short(1);}
		Sensitivity -= 0.2;
		while(1){if(!value_sum(0)){break;};}
	}
}

//>> Preparing Data for remotely controlling a Camera-Gimbal
//------------------------------------------------------------------------------------------------------
void create_data_camera(serial_data_struct* pdata_package_camera_control, serial_data_struct* pdata_package_camera_control_gier){
	pdata_package_camera_control->data[0] = gamepad_values_convert_for_cam(Axis[GamepadLayout[0]]);												//Nick
	pdata_package_camera_control->data[1] = gamepad_values_convert_for_cam(Axis[GamepadLayout[1]]);												//Roll
	pdata_package_camera_control->data[2] = gamepad_values_convert_for_cam(Axis[GamepadLayout[3]]);												//
	if(Button[GamepadLayout[8]] > 0){recordingButtonTemp = 131;}else if(Button[GamepadLayout[9]] > 0){recordingButtonTemp = 125;}
	pdata_package_camera_control->data[3] = recordingButtonTemp;
	if(Button[GamepadLayout[10]]){pdata_package_camera_control->data[4] = 131;}else{pdata_package_camera_control->data[4] = 125;}
	int i = 5;
	while(i < 12){
		pdata_package_camera_control->data[i] = 0;
		i++;
	}
	//Add Yaw (Gier)-funcionality
	externControlGier.Gier = gamepad_values_convert_for_kopter(Axis[GamepadLayout[2]]);
	externControlGier.Config = EC_VALID | EC_GAS_ADD;
	externControlGier.Frame = 'z';										//character for echo
	u8 *tmpData = (unsigned char *) &externControlGier;
	for (int i = 0; i < pdata_package_camera_control_gier->cmdLength; ++i)
	{
		pdata_package_camera_control_gier->data[i] = tmpData[i];
	}

	//Print out values on screen
	char tmpbuffer[255];
	sprintf(tmpbuffer, "Nick: %d    \n     Roll: %d     \n     Gier: %d     \n     Zoom: %d     \n     Aufnahme: %d    \n     Auslösung: %d", pdata_package_camera_control->data[0], pdata_package_camera_control->data[1], pdata_package_camera_control_gier->data[2], pdata_package_camera_control->data[2], pdata_package_camera_control->data[3], pdata_package_camera_control->data[4]);
	frame_values("VALUES", 5, 2);
	frame_values(tmpbuffer, 5, 4);
}

//>> Preparing Data for remotely controlling a MikroKopter
//------------------------------------------------------------------------------------------------------
void create_data_kopter(serial_data_struct* pdata_package_kopter_control){
	if(check_if_button_pushed() == GamepadLayout[13]){				//Motor start
		externControl.Nick = 131;
		externControl.Roll = 131;
		externControl.Gier = 131;
		externControl.Gas = 131;	
	}else if(check_if_button_pushed() == GamepadLayout[14]){		//Motor stop
		externControl.Nick = 131;
		externControl.Roll = 125;
		externControl.Gier = 125;
		externControl.Gas = 131;
	}else{
		externControl.Nick = gamepad_values_convert_for_kopter(Axis[GamepadLayout[4]]);
		externControl.Roll = gamepad_values_convert_for_kopter(Axis[GamepadLayout[5]]);
		externControl.Gier = gamepad_values_convert_for_kopter(Axis[GamepadLayout[6]]);
		externControl.Gas = gamepad_values_convert_for_kopter_gas(Axis[GamepadLayout[7]]);
	}
	externControl.Config = EC_VALID | EC_GAS_ADD | EC_IGNORE_RC;
	externControl.Frame = 'z';										//character for echo
	u8 *tmpData = (unsigned char *) &externControl;
	for (int i = 0; i < pdata_package_kopter_control->cmdLength; ++i)
	{
		pdata_package_kopter_control->data[i] = tmpData[i];
	}
	//Print out values on screen
	char tmpbuffer[255];
	sprintf(tmpbuffer, "Nick: %d    \n     Roll: %d     \n     Gier: %d     \n     Gas : %d   \n", pdata_package_kopter_control->data[0], pdata_package_kopter_control->data[1], pdata_package_kopter_control->data[2], pdata_package_kopter_control->data[3]);
	frame_values("VALUES", 5, 2);
	frame_values(tmpbuffer, 5, 4);
}

void read_gamepad_data(){
	read(padFd, &js, sizeof(struct js_event));
	switch (js.type & ~JS_EVENT_INIT)
	{
		case JS_EVENT_AXIS:
			Axis   [ js.number ] = js.value;
			break;
		case JS_EVENT_BUTTON:
			Button [ js.number ] = js.value;
			break;
	} 
}

//>> Initialize Gamepad
//------------------------------------------------------------------------------------------------------
void gamepad_init(){
	if((padFd = open(GAME_PAD, O_RDONLY)) == -1)
	{
		printf( "Couldn't open gamepad\n" );
	}
	ioctl( padFd, JSIOCGAXES, &NumberOfAxesAvailiable );
	ioctl( padFd, JSIOCGBUTTONS, &NumberOfButtonsAvailiable );
	ioctl( padFd, JSIOCGNAME(80), &GamepadConnected );
	Axis = (int *) calloc( NumberOfAxesAvailiable, sizeof( int ) );
	Button = (char *) calloc( NumberOfButtonsAvailiable, sizeof( char ) );
	fcntl( padFd, F_SETFL, O_NONBLOCK );	/* use non-blocking mode */
}