#ifndef _TYPES_H
 #define _TYPES_H

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct
{
	u8 address;
	u8 cmdID;
 	u8 data[1024];
 	u8 txrxdata[1024];
	u8 collecting;
 	u8 position_package;
	u16 cmdLength;

}__attribute__((packed)) serial_data_struct;


typedef struct
{
	u8 c;
	u8 data[1024];
	u16 position_rx;
	u16 count;
}__attribute__((packed)) data_rx_st;


struct str_ExternControl
{
 signed char   Nick;
 signed char   Roll;
 signed char   Gier;
 signed char Gas;
 unsigned char Frame;   // will return a confirm frame with this value
 unsigned char Config;
 unsigned char free;
};

#endif //_TYPES_H