// Define the hardware Pins for Controller Board

#define SAMD21_SUDEEP_BOARD
// Hardware LPS2023HW1.0
//----------------------------------------- 
#ifdef SAMD21_SUDEEP_BOARD

#define NSS1 4
#define RESET_RF96  8

#define LED1  13

#define OUT1 10

#define I2C_POWER 9

#define DONE_WDT 13

#define RS485_DIR 11
// TxD2 = D4
// RxD2 = D3

#define POWER_MESS_AKKU 18
// A0 = Akku-Voltage

/*
A0 = 24
A1 = 25
A2 = 26
A3 = 27
A4 = 28
A5 = 29
*/


#endif
