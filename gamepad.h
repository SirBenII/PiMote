#define GAME_PAD "/dev/input/js0"

u8 convert(int tmpwert);

int pad_fd, *axis=NULL, num_of_axis=0, num_of_buttons=0, x, stay_pushed = 125;
char *button=NULL, pad_connected[30];
struct js_event js;

void gamepad_init(){
	if((pad_fd = open(GAME_PAD, O_RDONLY)) == -1)
	{
		printf( "Couldn't open gamepad\n" );
	}
	ioctl( pad_fd, JSIOCGAXES, &num_of_axis );
	ioctl( pad_fd, JSIOCGBUTTONS, &num_of_buttons );
	ioctl( pad_fd, JSIOCGNAME(80), &pad_connected );
	axis = (int *) calloc( num_of_axis, sizeof( int ) );
	button = (char *) calloc( num_of_buttons, sizeof( char ) );
	fcntl( pad_fd, F_SETFL, O_NONBLOCK );	/* use non-blocking mode */
}

void get_gamepad(){
	read(pad_fd, &js, sizeof(struct js_event));
	switch (js.type & ~JS_EVENT_INIT)
	{
		case JS_EVENT_AXIS:
			axis   [ js.number ] = js.value;
			break;
		case JS_EVENT_BUTTON:
			button [ js.number ] = js.value;
			break;
	} 
}

void prepare_data_servo(data_package_st* pdata_package_serv){
	int i = 0;
	while(i < 3)
	{
		pdata_package_serv->data[i] = convert(axis[layout[i]]);
		i++;
	}
	if(button[layout[i]] > 0){stay_pushed = 131;}else if(button[layout[i+1]] > 0){stay_pushed = 125;}
	pdata_package_serv->data[i] = stay_pushed;
	if(button[layout[++i+1]]){pdata_package_serv->data[i] = 131;}else{pdata_package_serv->data[i] = 125;}
	++i;
	while(i < 12){
		pdata_package_serv->data[i++] = 0;
	}
	pdata_package_serv->address = FC_ADDRESS + 'a';
	pdata_package_serv->cmdID = 'y';
	pdata_package_serv->datLen = 12;
	
	char tmpbuffer[255];
	sprintf(tmpbuffer, "Nick: %d    \n     Roll: %d     \n     Zoom: %d     \n     Aufnahme: %d    \n     Auslösung: %d", pdata_package_serv->data[0], pdata_package_serv->data[1], pdata_package_serv->data[2], pdata_package_serv->data[3], pdata_package_serv->data[4]);
	
	frame_values("VALUES", 5, 2);
	frame_values(tmpbuffer, 5, 4);
}

u8 convert(int tmpvalue){
	u8 tmp;
	double tmpdouble = tmpvalue;
	tmp = ((((tmpdouble/(axisValue))-1)*125)-131)*-1;
	if(tmp == 0){tmp = 255;}
	return(tmp);
}

int button_pushed(){
	get_gamepad();
	int buttonpushed = -1;
	for (int i = 0; i < num_of_buttons; i++)
	{
		if(button[i] != 0){
			buttonpushed = i;
		}
	}
	return(buttonpushed);
}