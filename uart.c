#include "main.h"
#include "uart.h"

int uart0_filestream = -1; 
int usb_stream = -1;
int data_length;		//length of transmitting data string


//>> Adding checksum and transmitting  data
//------------------------------------------------------------------------------------------------------
void transmit_data(serial_data_struct* pdata_package){
	if (uart0_filestream != -1)
	{
		int count = write(uart0_filestream, pdata_package->txrxdata, pdata_package->cmdLength);		
		if (count < 0)		
		{
			printf("UART TX error\n");
		}
	}
}

//>> Adding address and command ID, encoding and checksum
//------------------------------------------------------------------------------------------------------
void create_serial_frame(u8 address, u8 cmdID, u16 cmdLength, serial_data_struct* pdata_package){
	pdata_package->txrxdata[0] = '#';
	pdata_package->txrxdata[1] = address + 'a';
	pdata_package->txrxdata[2] = cmdID;

	//encode data
	u8 a,b,c;
	int counter = 0;
	int u = 3;
	while(cmdLength){
		if(cmdLength)
			{
				cmdLength--; 
				a = pdata_package->data[counter++];
			}else{a = 0; counter++;};
		if(cmdLength)
			{
				cmdLength--; 
				b = pdata_package->data[counter++];
			}else{b = 0; counter++;};
		if(cmdLength)
			{
				cmdLength--; 
				c = pdata_package->data[counter++];
			}else{c = 0; counter++;};
		pdata_package->txrxdata[u++] = '=' + (a >> 2);
		pdata_package->txrxdata[u++] = '=' + (((a & 0x03) << 4) | ((b & 0xf0) >> 4));
		pdata_package->txrxdata[u++] = '=' + (((b & 0x0f) << 2) | ((c & 0xc0) >> 6));
		pdata_package->txrxdata[u++] = '=' + (c & 0x3f);
	}	

	//add Checksum
	u16 crc = 0;
	u8 	crc1, crc2;
	for (int i = 0; i < u; i++)
	{
		crc += pdata_package->txrxdata[i];
	}
	crc %= 4096;
	crc1 = '=' + crc / 64;
	crc2 = '=' + crc % 64;
	pdata_package->txrxdata[u++] = crc1;
	pdata_package->txrxdata[u++] = crc2;
	pdata_package->txrxdata[u++] = '\r';

	pdata_package->cmdLength = u;
}


//>> Initializing serial interface
//------------------------------------------------------------------------------------------------------
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