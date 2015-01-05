#ifndef _UART_H
 #define _UART_H

//addresses
#define FC_address      1  //(b)
#define NC_address      2  //(c)
#define MK3MAG_address  3  //(d)
#define BL_CTRL_address 5  //(f)

//Command IDs
#define SERIAL_CHANNELS		'y'
#define EXTERNAL_CONTROL	'b'


#define SERIAL "/dev/ttyAMA0"

//struct js_event e;

extern void create_serial_frame(u8 address, u8 cmdID, u16 cmdLength, serial_data_struct* pdata_package);
extern void transmit_data(serial_data_struct* pdata_package);
extern void encode_data(serial_data_struct* pdata_package);
extern void uart_init();

#endif //_UART_H