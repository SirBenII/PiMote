#define NUMBER_OF_AXIS		3
#define NUMBER_OF_BUTTONS	3

u8 layout[30];
int axisValue;
//0: nick  1: roll  2: zoom  3: startRecord  4: stopRecord  5: trigger  
//   Axis     Axis     Axis     Button          Button         Button     
char* lablesAxis[NUMBER_OF_AXIS] = 	{   "camNick:",
                                                "camRoll:",
                                                "camZoom:"     };
char* lablesButtons[NUMBER_OF_BUTTONS] = {"startRecord:",
                                                "stopRecord:",
                                                "trigger:"     };

int file_init(){
    char line[50];
    FILE *fp = fopen("/home/pi/PiMote/configf.txt","r");
    if(fp)
    {
    	while( fgets(line,50,fp) ) {
        	//Layout and values
        	for (int i = 0; i < NUMBER_OF_AXIS; i++)
        	{
        		if(strstr(line, lablesAxis[i]) != NULL){
        			char *token = strtok(line, " ");
        			token = strtok(NULL, " ");
        			layout[i] = token[0] - '0';
        		}
        	}
        	for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
        	{
        		if(strstr(line, lablesButtons[i]) != NULL){
        			char *token = strtok(line, " ");
        			token = strtok(NULL, " ");
        			layout[i + NUMBER_OF_AXIS] = token[0] - '0';
        		}
        	}
            if(strstr(line, "axisValue") != NULL){
                char *token = strtok(line, " ");
                token = strtok(NULL, " ");
                axisValue = strtol(token, (char **)NULL, 10);
            }
        	memset(line, 0, 49);
    	}
    	fclose(fp);
    	return(1);
    }
    return(0);
}

void write_file(void){
    FILE *fp = fopen("/home/pi/PiMote/configf.txt","w+");
    fprintf(fp, "#Gamepad Configuration File\r\n\n");
    fprintf(fp, "#layout\r\n\n");
    for (int i = 0; i < NUMBER_OF_AXIS; i++){fprintf(fp, "%s %d\r\n", lablesAxis[i], layout[i]);}
    for (int i = 0; i < NUMBER_OF_BUTTONS; i++){fprintf(fp, "%s %d\r\n", lablesButtons[i], layout[i + NUMBER_OF_AXIS]);}
    fprintf(fp, "\n#Max value of axis\r\n\naxisValue: ");
    fprintf(fp, "%d", axisValue);
    fclose(fp);
}
