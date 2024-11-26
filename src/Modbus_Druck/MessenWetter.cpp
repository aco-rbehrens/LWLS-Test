
// Compiler Arduino V1.8.19
// Board: Adafruit Feather M0
// Platine: ACO Main

#include <arduino.h>
#include "Platine.h"
#include <SPI.h>
#include <Wire.h>
#include "myi2c.h"
#include "messenWetter.h"  
#include "ATSAMD21_ADC.h"
#include "LED_Hardware.h"
#include <RTCZero.h>
#include "hseSensorProtocol.h"
#include <OneWire.h>
#include "SFE_BMP180.h"
#include <ArduinoLowPower.h>
#include "CSPPM.h"
#include "SensirionI2CSht4x.h"  // https://github.com/Sensirion/arduino-i2c-sht4x


RTCZero zerortc;
char szUhrzeit2[25];
char szText[30];
int iOldHour;
float t,p,h,w;
long lVisLux; 
long uiOnStd;
uint8_t uTxPower, uPowerDir;
float fUb;
uint8_t RetryPowerOn;

SFE_BMP180 bmp180;
//SensirionI2CSht4x sht4x;
SensirionI2cSht4x  sht4x;

char errorMessage[256];
float WasserTemp, WasserTiefe;

// jede Sendung die Messwerte ueber RS232 melden
//#define DEBUG_PRINT 

// hse-Protokoll Datensatz
HseSP hse(2, 60);


byte cntSendung = 5;
extern int iNextTxSec;


// Uhr anhand des Compiler Zeitpunkt setzen
// es geht nur die Uhrzeit (Monat und Jahr von Hand)
void InitTimeDate(void)
{
int h,m,t; 

     zerortc.begin(); // Set up clocks  

     // nutze das Compiler Datum zum Stellen der Uhr
     h = (__TIME__[0]-0x30)*10;
     h += (__TIME__[1]-0x30);
     
     m = (__TIME__[3]-0x30)*10;
     m += (__TIME__[4]-0x30);

     //Jun 10 2020.
     //01234567890
     t =(__DATE__[4]-0x30)*10;
     t +=(__DATE__[5]-0x30);

    // Uhrzeit stellen     
     zerortc.begin(); // Set up clocks  
     zerortc.setTime(h, m, 1);
     zerortc.setDate(t, 7, 23); 
     
     uiOnStd = 0;
     Serial1.println("InitTimeDate ");
}

//--------------------------------------------------
void PrintTimeDate(void)
{
  int i;
// Datum auch
 sprintf(szUhrzeit2, "%02d.%02d.%02d  ", zerortc.getDay(), zerortc.getMonth(), zerortc.getYear() );  
 Serial1.print(szUhrzeit2);
 
// Zum Test Uhrzeit ausgeben
 sprintf(szUhrzeit2, "%02d:%02d:%02d", zerortc.getHours(), zerortc.getMinutes(), zerortc.getSeconds() );  
 Serial1.println(szUhrzeit2);
 
// gesammt Stunden berechnen
i = zerortc.getHours();
if ( iOldHour != i)
   {
   iOldHour = i;
   Serial1.print("h ");
   Serial1.print(uiOnStd);
   Serial1.print("->");
   uiOnStd++;
   Serial1.println(uiOnStd);
   }
}


//-----------------------------
void SetMySystemClock(int iMHz)
{
int iDivider;
 iDivider = 48 / iMHz;

 /* Configure divisor to generate xMHz (divide 48MHz by 2) */
  GCLK->GENDIV.reg = GCLK_GENDIV_DIV(iDivider) | GCLK_GENDIV_ID(0);
  /* Configure DFLL48 as source and enable clock generator */
  GCLK->GENCTRL.reg = GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_DFLL48M | GCLK_GENCTRL_ID(0);
  /* Generator 0 is always connected to the CPU clock, so we are done here */
}


//--------------------------------------------------------------------------------------------------
// Read current supply voltage
//--------------------------------------------------------------------------------------------------
long readVcc() 
{
int result;

analogReference2(ADC_REF_INT1V);
analogReadExtended(12);   // 12bit
// lese Vdd io zu GND mit Vorteiler 1/4
result = 1000.0*analogDifferentialRaw(ADC_PIN_SCALEDIOVCC, ADC_PIN_IOGND)/256.0;
return result;
}
//----------------------------------------------
void Messen_Wetter(void)
{
int i,status, error, n;
float t, p, h,d,r;
//float DruckArray[4];
float rawCppmDruck;
float tempArray[5];

double P,T;

Serial1.println(F("messe"));
Serial1.flush();
PrintTimeDate();
Serial1.flush();

 //------------------
 Serial1.println(F("CSPPM-MessenDruck"));
 Serial1.flush();

 init_CSPPM();  // und RS485 einschalten
 
 n=0;
 do{
   error = ReadPressureAndTemp();
   if ( error == 0 )
      {
      break;
      }
   n++;
   } while(n<20);
 
 
 CSPPM_PowerDown();   // und RS485 ausschalten

 rawCppmDruck = CSPPM_GetDruck();  
 tempArray[2] = CSPPM_GetTemperatur();    

 WasserTiefe = 0.0;   // Fehler
   
 Serial1.print(F("Druck: "));
 Serial1.print( rawCppmDruck );
 Serial1.print(F(", Temp: "));
 Serial1.println( tempArray[2] );

 WasserTiefe = rawCppmDruck * 10.0*10.0;

 if ( WasserTiefe <0.0 ) WasserTiefe = 0.0;  // Weil Distanz keine neg Zahlen kann
 Serial1.print(F("Temptiefe= "));
 Serial1.println(WasserTiefe);  // mm
 Serial1.flush();   // wegen Taktumschaltung
/*  

 //Serial1.print(F("TempWasser= "));
 //Serial1.println(WasserTemp);
 //tempArray[2] = WasserTemp;

if (bmp180.begin())
   {
   Serial1.print(F("BMP180: ")); 
   status = bmp180.startTemperature();
   if (status != 0)
      {
      // Wait for the measurement to complete:
      delay(status);      
      bmp180.getTemperature(T);
      Serial1.print(F("Temp = "));
      Serial1.print(T);      
      tempArray[1] = T;
      t = T;
      }
   else
      Serial1.print(F("Error Temp"));   
      
   status = bmp180.startPressure(3);
   if (status != 0)
      {              
      delay(status);  
      
      bmp180.getPressure(P,T);
      Serial1.print(F(", Druck = "));
      Serial1.println(P);      
      DruckArray[0] = P;
      p = P;
      }
   else
      Serial1.print(F(", Error Druck"));   

   }
 */ 
  // SHT41 
  //sht4x.begin(Wire);
  sht4x.begin(Wire, SHT40_I2C_ADDR_44);

  error = sht4x.measureHighPrecision(t, h);
  if (error) {
      Serial1.print("Error trying to execute measureHighPrecision(): ");
      errorToString(error, errorMessage, 256);
      Serial1.println(errorMessage);
    } else {
        Serial1.print("Temperature:");
        Serial1.print(t);
        Serial1.print(",  ");
        Serial1.print("Humidity:");
        Serial1.println(h);
        tempArray[0] = t;   
    }


// schaltet den ADC Kanal richtig, fuer nachfolgende Akku Messung
Serial1.print(F("Ubatt = "));
fUb = readVcc();   // T3 und T5
Serial1.print( fUb );
Serial1.println(F("mV "));

// Ubatt / Uakku und Betriebsstunden
 ServiceDatenBilden();   

// Structur bauen und senden
// hse Protokoll zusammen bauen
hse.reset();
//Ein Klimasensor hinzufügen
  HseSP::ClimateSensor_t cs;
  cs.Temperature = t; //°C
  cs.Humidity = h;     //%
  cs.Pressure = p;   //hPa
  cs.Illuminance = 0;   //lux
  hse.addClimateSensor(&cs);

  // Daten vom Drucksensor
  //hse.addPressure(DruckArray,3); geht nicht weil keine negativen Zahlen möglich sind im Protokoll
  hse.addPressure(max(rawCppmDruck, 0.0) * 10.0);
  hse.addGenericFloat(rawCppmDruck);
  //hse.addGenericFloat(DruckArray,2);
  hse.addDistance(WasserTiefe/1000.0);  // Einheit m
  hse.addTemperature(tempArray,3);
 
  hse.addVoltage(fUb/1000.0);  // in Volt
  hse.addCounter(uiOnStd);    // Zeit On
  hse.addGenericUInt8(RetryPowerOn);

 i = hse.getSize();
 Serial1.print(F("SizeOf(hse) = "));
 Serial1.println(i); 



}
//------------------------------------------------------------------- 

//---------------------------------------------------
void I2C_ChipTest(void)
{
//..



}

//-----------------------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------------------
void Init_Messen_Wetter(void) 
{
   Serial1.println(F("Init Messen")); 


}


//----------------------------------------------
// hse Struct in LoRa Sende Struct umkopieren
//----------------------------------------------
void hseData2GlobalData(void)
{
int i;
unsigned char *ucPointer;


 // wieviel nutz Daten Byte
 Data_Length_Tx = hse.getSize();

 // User-Datenbuffer in globalen Buffer uebertragen
 ucPointer = (unsigned char*)hse.getBuffer();
 for ( i=0; i< Data_Length_Tx; i++)    
     Data_Tx[i]= *ucPointer++;

}

//--------------------------------------------------------------------
void ServiceDatenBilden(void) {
    float fUMcu;
    int uAdc, n;

    // schaltet den ADC Kanal richtig, fuer nachfolgende Akku Messung
    fUb = readVcc();   // hier lassen!

    // Akkuspannung zuschalten
    pinMode(POWER_MESS_AKKU, OUTPUT);
    digitalWrite(POWER_MESS_AKKU, HIGH);
    //analogReference(AR_INTERNAL); // 2,23V
    analogReadExtended(12);   // 12bit 
    uAdc = analogRead(A0);  // A4 ist nicht nutzbar! deswegen A0
    delay(10);

    // 1M Ohm und 511k Ohm => dann kann ich bis 6,9V messen
    // UAkku = A0 * 3300 / 4096.0 * 151/511 [mV] 

    // Kurve angepasst > 3,3 bis 5V
    fUb = 3.3 + (uAdc - 603) * 1.7  / 279.0;
    fUb *= 1000.0;

    Serial1.print("U adc= ");
    Serial1.print(uAdc);

    // CPU Spannung
    fUMcu = readVcc();

    Serial1.print(", U cpu= ");
    Serial1.print(fUMcu);  // in Volt

    Serial1.print(", U batt= ");
    Serial1.println(fUb);  // in Volt

// unter 3,3V laest sicht die Akkuspannung nicht richtig messen
// deswegen nehme ich dann die CPU Spannung
    if (fUb < 3300.0) // 3,3V Regler
    {
        fUb = fUMcu;
    }

    digitalWrite(POWER_MESS_AKKU, LOW);  // Messspannung ausschalten

// Low Batt Grenze ist 2,27V
    if (fUMcu < 2700.0) {
        iNextTxSec = 1800;   // 30min
    }

    Serial1.print("final UBatt= ");
    Serial1.println(fUb);  // in Volt

    Serial1.print("OnStd= ");
    Serial1.println(uiOnStd);  // Stunden
}
//----------------------------------
