

#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function
#include "Platine.h"
#include "CSPPM.h"
#include "ModBusRB.h"

// new Command 5.2023 + 115200 Baud
uint8_t TxCmdReadTempAndPressure[8]={ 0x01, 0x03, 0x10, 0x00, 0x00, 0x04, 0x40, 0xC9 };


int32_t RawPressure, RawTemperatur;
float fRawPressure, fRawTemperatur;


// Adafruit Feather M0 => TxD2 = PA14
// Arduino Zero => TxD2 = PA14
Uart Serial2 (&sercom2, 3, 2, SERCOM_RX_PAD_1, UART_TX_PAD_2);

void SERCOM2_Handler()
{
  Serial2.IrqHandler();
}



int init_CSPPM(void)
{
 pinMode(OUT1,OUTPUT);       // +5V Wandler einschalten
 digitalWrite(OUT1, HIGH);  
 
 
 pinMode(RS485_DIR,OUTPUT);       // 0=Senden/1=Empfang
 digitalWrite(RS485_DIR, HIGH);  

 
 // TxD2
 Serial2.begin(115200);
 // Assign pins 3 & 2 SERCOM functionality
 pinPeripheral(3, PIO_SERCOM_ALT);
 pinPeripheral(2, PIO_SERCOM);

 delay(3000);
 
return(0);
}


// Strom abschalten
void CSPPM_PowerDown(void)
{
 digitalWrite(OUT1, LOW);     // Druck Sensor ausschalten
 digitalWrite(RS485_DIR, LOW);

 //pinPeripheral(3, PIO_INPUT); => 1V
 
 pinPeripheral(3, PIO_OUTPUT);
 digitalWrite(3, LOW);

 //pinPeripheral(4, PIO_OUTPUT);
 //digitalWrite(4, LOW);

 pinPeripheral(2, PIO_OUTPUT); 
 digitalWrite(2, LOW);
}



uint8_t ReadPressureAndTemp(void)
{
int i;  
// switch TX = Low am PIO und damit High am Open Kollektor und MAX485
 digitalWrite(RS485_DIR, LOW); 


fRawTemperatur = 0.0;
fRawPressure = 0.0;
fNewCommand = 0;

 Serial2.write(TxCmdReadTempAndPressure, 8);
 Serial2.flush();

//delay(1);    
digitalWrite(RS485_DIR, HIGH); 

// Empfang seine Antwort
delay(100);    
while( Serial2.available() > 0 )
     {
     //Serial1.print( Serial2.read(), HEX);
     ModBus_Rx( Serial2.read());
     }
// war das eine gueltige Nachricht     
if ( fNewCommand )
   {
   Serial1.println("good response");  
   //ucRxTelegramm[3..n] = Data 
   RawPressure = ucRxTelegramm[7];
   RawPressure <<= 8;
   RawPressure |= ucRxTelegramm[8];
   RawPressure <<= 8;
   RawPressure |= ucRxTelegramm[9];
   RawPressure <<= 8;
   RawPressure |= ucRxTelegramm[10];
   fRawPressure = *(float*) &RawPressure;
   
   RawTemperatur = ucRxTelegramm[3];
   RawTemperatur <<= 8;
   RawTemperatur |= ucRxTelegramm[4];
   RawTemperatur <<= 8;
   RawTemperatur |= ucRxTelegramm[5];
   RawTemperatur <<= 8;
   RawTemperatur |= ucRxTelegramm[6];
   fRawTemperatur = *(float*) &RawTemperatur;

   //Serial1.print("Len= ");
   //Serial1.print(ucRxTelegramm[ 2 ]);

   Serial1.print(", P= ");
   //Serial1.print(RawPressure);
   //Serial1.print(" = ");
   Serial1.print(fRawPressure);
   
   Serial1.print(", T= ");
   //Serial1.print(RawTemperatur);
   //Serial1.print(" = ");
   Serial1.println(fRawTemperatur);
   return(0);
   }
else
   {
   //Serial1.println("no good response");
   return(1);
   }
Serial1.println("no response");
return(2);
}

//--------------------------------
float CSPPM_GetDruck(void)
{
 return (fRawPressure);
}
//-----------------------
float CSPPM_GetTemperatur(void)
{
 return(fRawTemperatur);
}
