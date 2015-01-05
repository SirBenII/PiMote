#include "main.h"
#include "file.h"

int MaxAxisValue;
u8 GamepadLayout[30];
u8 FileExists;

const char* LablesOfAxis[NUMBER_OF_AXES_USED] = 
{
    "camNick:",     //  GamepadLayout Position [0]      Nick-Axis of Camera-Gimbal
    "camRoll:",     //  GamepadLayout Position [1]      Roll-Axis of Camera-Gimbal
    "camGier:",     //  GamepadLayout Positoin [2]      Yaw (Gier)-Axis of MikroKopter (for Camera Control)
    "camZoom:",     //  GamepadLayout Position [3]      Camera-Zoom
    "extNick:",     //  GamepadLayout Position [4]      Nick-Axis of MikroKopter
    "extRoll:",     //  GamepadLayout Position [5]      Roll-Axis of MikroKopter
    "extGier:",     //  GamepadLayout Position [6]      Yaw(Gier)-Axis of MikroKopter
    "extGas:"       //  GamepadLayout Position [7]      Gas of MikroKopter
};
const char* LablesOfButtons[NUMBER_OF_BUTTONS_USED] = 
{
    "startRecord:", //  GamepadLayout Position [8]      Start Recording Camcorder
    "stopRecord:",  //  GamepadLayout Position [9]      Stop Recording Camcorder
    "trigger:",     //  GamepadLayout Position [10]      Trigger a Camera
    "programOne:",  //  GamepadLayout Position [11]     Program 1: Remote Control Camera
    "programTwo:",  //  GamepadLayout Position [12]     Program 2: Remote Control MikroKopter
    "motorStart:",  //  GamepadLayout Position [13]     Start Motors
    "motorStop:",   //  GamepadLayout Position [14]     Stop Motors
    "sensPlus:",    //  GamepadLayout Position [15]     Increase sensitivity of the controls
    "sensMinus:",   //  GamepadLayout Position [16]     Decrease sensitivity of the controls
    "activate:"     //  GamepadLayout Position [17]     Activate the controls
};

//>> Reading or creating configuration file
//------------------------------------------------------------------------------------------------------
int file_init(){
    char line[50];
    FILE *fp = fopen("/home/pi/PiMoteSwitch/configf.txt","r");
    if(fp)
    {
    	while( fgets(line,50,fp) ) {
        	for (int i = 0; i < NUMBER_OF_AXES_USED; i++)
        	{
        		if(strstr(line, LablesOfAxis[i]) != NULL){
        			char *token = strtok(line, " ");
        			token = strtok(NULL, " ");
        			GamepadLayout[i] = token[0] - '0';
        		}
        	}
        	for (int i = 0; i < NUMBER_OF_BUTTONS_USED; i++)
        	{
        		if(strstr(line, LablesOfButtons[i]) != NULL){
        			char *token = strtok(line, " ");
        			token = strtok(NULL, " ");
        			GamepadLayout[i + NUMBER_OF_AXES_USED] = token[0] - '0';
        		}
        	}
            if(strstr(line, "MaxAxisValue") != NULL){
                char *token = strtok(line, " ");
                token = strtok(NULL, " ");
                MaxAxisValue = strtol(token, (char **)NULL, 10);
            }
        	memset(line, 0, 49);
    	}
    	fclose(fp);
    	return(1);
    }
    return(0);
}

//>> Write new configuration to file
//------------------------------------------------------------------------------------------------------
void write_file(void){
    FILE *fp = fopen("/home/pi/PiMoteSwitch/configf.txt","w+");
    fprintf(fp, "#Gamepad Configuration File\r\n\n");
    fprintf(fp, "#layout\r\n\n");
    for (int i = 0; i < NUMBER_OF_AXES_USED; i++){fprintf(fp, "%s %d\r\n", LablesOfAxis[i], GamepadLayout[i]);}
    for (int i = 0; i < NUMBER_OF_BUTTONS_USED; i++){fprintf(fp, "%s %d\r\n", LablesOfButtons[i], GamepadLayout[i + NUMBER_OF_AXES_USED]);}
    fprintf(fp, "\n#Max value of axis\r\n\nMaxAxisValue: ");
    fprintf(fp, "%d", MaxAxisValue);
    fclose(fp);
}