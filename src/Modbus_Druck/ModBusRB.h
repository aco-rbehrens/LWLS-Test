#ifndef h_ModBusRB
#define h_ModBusRB



#define MODBUS_STATE_DEVICE_ADDRESS   1
#define MODBUS_STATE_FUNCTIONCODE     2
#define MODBUS_STATE_TELEGRAMM_LAENGE 3
#define MODBUS_STATE_GET_DATA         4 
#define MODBUS_STATE_GET_CRC1         5
#define MODBUS_STATE_GET_CRC2         6

// aktuelle Daten
#define MODBUS_DEVICE_ADDRESS  1
#define MODBUS_FUNCTION_CODE   3



void ModBus_Rx( unsigned char ucRxByte );
void ModBus_Init(void);

extern unsigned char ucRxTelegramm[300];
extern unsigned char ucRxState, ucTelegrammlaenge, ucGotByte, ucDienst;
extern char fNewCommand;



#endif
