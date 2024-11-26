#include <Arduino.h> 
#include "ModBusRB.h"



unsigned char ucRxTelegramm[300];
unsigned char ucRxState, ucTelegrammlaenge, ucGotByte;
char fNewCommand;
uint16_t uiCalcBCC, uiRxBCC;

/*
ucRxTelegramm[0] = Device Adresse
ucRxTelegramm[1] = Function code
ucRxTelegramm[2] = Number of byte ohne CRC
ucRxTelegramm[3..n] = Data
ucRxTelegramm[n+1,n+2] = CRC
*/

//-----------------------------------------------------
// ModBus Rx-Routinen
//-----------------------------------------------------

void ModBus_Init(void)
{
fNewCommand = 0;
ucRxState = MODBUS_STATE_DEVICE_ADDRESS;
}



void ModBus_Rx( unsigned char ucRxByte )
{
uint8_t n;

 switch (ucRxState)
        {
        case MODBUS_STATE_DEVICE_ADDRESS: 
             if ( ucRxByte == MODBUS_DEVICE_ADDRESS ) ucRxState = MODBUS_STATE_FUNCTIONCODE;
             ucRxTelegramm[ 0 ]= ucRxByte;
             break;

        case MODBUS_STATE_FUNCTIONCODE:
             if ( ucRxByte == MODBUS_FUNCTION_CODE ) ucRxState = MODBUS_STATE_TELEGRAMM_LAENGE;
             else  ucRxState = MODBUS_STATE_DEVICE_ADDRESS;
             ucRxTelegramm[ 1 ]= ucRxByte;
             break;

        case MODBUS_STATE_TELEGRAMM_LAENGE:
             ucTelegrammlaenge = ucRxByte;             
             ucRxTelegramm[ 2 ]= ucRxByte;
             
             if ( ucTelegrammlaenge < 3 )
                ucRxState = MODBUS_STATE_DEVICE_ADDRESS;
             else   
                ucRxState = MODBUS_STATE_GET_DATA;
             ucGotByte = 0;  					// Index fuer Rx
             break;

        case MODBUS_STATE_GET_DATA:
             if  ( ucGotByte < sizeof(ucRxTelegramm) -4)
                 {
                 ucRxTelegramm[ 3+ ucGotByte++ ] = ucRxByte;
                 }
             else     
                {
                ucGotByte = 0;           // reset
                ucRxState = MODBUS_STATE_DEVICE_ADDRESS;
                }
                
             if ( ucGotByte == ucTelegrammlaenge )
                {
                ucRxState = MODBUS_STATE_GET_CRC1;
                }
              break;
          
        case MODBUS_STATE_GET_CRC1:
             uiRxBCC = ucRxByte;
             ucRxState = MODBUS_STATE_GET_CRC2;                
             break;

        case MODBUS_STATE_GET_CRC2:
             uiRxBCC = ucRxByte;
             // Daten sind fertig
             //...
             fNewCommand = 1;       // Message is true
             
/*                
             // ---------------------------
             // CRC Compare
             // ---------------------------                
				     if ( ucCalcBCC == ucRxBCC )
					      {
					      fNewCommand = 1;				// Message is true
					      }
				     else
 					      {
					      // CRC Error
					      //USS_Send_ErrorMessage( ucDienst, USS_ERROR_CRC_FAILT );
					      }				                            
*/                
             ucRxState = MODBUS_STATE_DEVICE_ADDRESS;                   
             break;

        default:
             ucRxState = MODBUS_STATE_DEVICE_ADDRESS;
             break;
        }
} 
