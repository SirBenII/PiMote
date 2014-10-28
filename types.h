
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
	u16 datLen;

}__attribute__((packed)) data_package_st;
