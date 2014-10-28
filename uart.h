#define FC_ADDRESS      1  //(b)
#define NC_ADDRESS      2  //(c)
#define MK3MAG_ADDRESS  3  //(d)
#define BL_CTRL_ADDRESS 5  //(f)
#define SERIAL "/dev/ttyUSB0"

struct js_event e;
int uart0_filestream = -1; 
int usb_stream = -1;
int txlen;

void tx(data_package_st* pdata_package){
	txlen = pdata_package->datLen + 6;
	u8 data[1024] = {'#', pdata_package->address, pdata_package->cmdID};	
	for (int i = 3; i < txlen - 3; i++)
	{
		data[i] = pdata_package->txrxdata[i-3];
	}
	//Add Checksum
	u16 crc = 0;
	u8 	crc1, crc2;
	for (int i = 0; i < txlen - 3; i++)
	{
		crc += data[i];
	}
	crc %= 4096;
	crc1 = '=' + crc / 64;
	crc2 = '=' + crc % 64;
	data[txlen-3] = crc1;
	data[txlen-2] = crc2;
	data[txlen-1] = '\r';

	if (uart0_filestream != -1)
	{
		int count = write(uart0_filestream, data, txlen);		//Filestream, bytes to write, number of bytes to write
		if (count < 0)		
		{
			printf("UART TX error\n");
		}
	}
}

void encode(data_package_st* pdata_package){
	u8 a,b,c;
	int i = 0;
	int u = 0;
	while(pdata_package->datLen){
		if(pdata_package->datLen)
			{
				pdata_package->datLen--; 
				a = pdata_package->data[i++];
			}else{a = 0; i++;};
		if(pdata_package->datLen)
			{
				pdata_package->datLen--; 
				b = pdata_package->data[i++];
			}else{b = 0; i++;};
		if(pdata_package->datLen)
			{
				pdata_package->datLen--; 
				c = pdata_package->data[i++];
			}else{c = 0; i++;};
		pdata_package->txrxdata[u++] = '=' + (a >> 2);
		pdata_package->txrxdata[u++] = '=' + (((a & 0x03) << 4) | ((b & 0xf0) >> 4));
		pdata_package->txrxdata[u++] = '=' + (((b & 0x0f) << 2) | ((c & 0xc0) >> 6));
		pdata_package->txrxdata[u++] = '=' + (c & 0x3f);

	}
	pdata_package->datLen = u;	
}

void uart_init(){
	uart0_filestream = open(SERIAL, O_RDWR | O_NOCTTY | O_NDELAY);
	if (uart0_filestream == -1)
	{
		printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
	}
	//UART Options
	struct termios options;
	tcgetattr(uart0_filestream, &options);
	options.c_cflag = B57600 | CS8 | CLOCAL | CREAD;		
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);
	//UART Options
}