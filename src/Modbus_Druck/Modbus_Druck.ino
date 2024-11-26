
// Modbus Drucksensor CSPPM
// Red   = Vdd = 3,3V
// Black  = GND
// Blue  = A
// White = B

// Compiler Arduino V1.8.19
// Board: Arduino zero nativ
// Platine: ACO LoRa Main

#include "Platine.h"
#include <Wire.h>
#include "LoRaMAC.h"
#include "LED_Hardware.h"
#include <ArduinoLowPower.h>
#include "messenWetter.h"
#include "eepromFuncMB85.h"

// jede Sendung die Messwerte ueber RS232 melden
// #define DEBUG_PRINT

/*
*****************************************************************************************
* GLOBAL VARIABLES
*****************************************************************************************
*/

/* Device EUI */
extern u1_t DEVEUI[8];

unsigned char DevAddr[4];
extern unsigned char Data_Tx[64];
extern unsigned char Data_Rx[64];
extern unsigned char Data_Length_Tx;
extern unsigned char Data_Length_Rx;

extern unsigned char ucTxPower, ucResetQuelle;
extern int iTest, iNewDay;
extern int iNextTxSec, iSinceLastTx;

/*
*****************************************************************************************
* FUNCTION PROTOTYPE
*****************************************************************************************
*/
void Wdt_DoneImpuls(void);

//------------------------------------------
// LoRa DevADDR build from CPU ID
//------------------------------------------
void Build_DevAddr(void)
{
    volatile uint32_t val1;
    uint8_t val2[4] = {0};

    volatile uint32_t *ptr1 = (volatile uint32_t *)0x0080A00C;

    // Ask the CPU for his unique number
    val1 = *ptr1;

    // ACO als Marker in die DevEUI und eine Nummer die mit 0 startet
    val2[0] = 0x41;
    val2[1] = 0x43;
    val2[2] = 0x4F;
    val2[3] = 0x00; // Geraete Art

    // use 4 Byte from that for the DEVADDR
    DevAddr[0] = val1 & 0xFF;
    DevAddr[1] = (val1 >> 8) & 0xFF;
    DevAddr[2] = (val1 >> 16) & 0xFF;
    DevAddr[3] = (val1 >> 24) & 0xFF;

    // JSON String
    Serial1.print(F("{\"Model\":\"LWLS-01\",\"HardwareVersion\":\"1.0\",\"SerialNumber\":\"2023070xx\","));

    /* Storing the Device EUI (ACO + 0 + DEVICE ADRRESS) into the DEVEUI buffer */
    /* This should be in little endian format, so least-significant-byte
    first. When copying an EUI from ttnctl output, this means to reverse
    the bytes. */
    memcpy(DEVEUI, val2, 4);
    memcpy(&DEVEUI[4], DevAddr, 4);
    /* Print the DEVEUI */
    Serial1.print(F("\"DevEui\":\""));
    for (int i = 7; i > 0; i--)
    {
        Serial1.print(DEVEUI[i], HEX);
    }
    Serial1.println("\"");
}

//----------------------------------------
// Show Device Addr
void PrintDeviceID(void)
{
    Serial1.print(F("DevAddr = "));
    if (DevAddr[3] < 0x10)
        Serial1.print(F("0"));
    Serial1.print(DevAddr[3], HEX);
    if (DevAddr[2] < 0x10)
        Serial1.print(F("0"));
    Serial1.print(DevAddr[2], HEX);
    if (DevAddr[1] < 0x10)
        Serial1.print(F("0"));
    Serial1.print(DevAddr[1], HEX);
    if (DevAddr[0] < 0x10)
        Serial1.print(F("0"));
    Serial1.println(DevAddr[0], HEX);
}

void Wdt_DoneImpuls(void)
{
    // Done Impuls
    pinMode(DONE_WDT, OUTPUT);
    delayMicroseconds(50);
    digitalWrite(DONE_WDT, LOW);
    delayMicroseconds(50);
    digitalWrite(DONE_WDT, HIGH);
    delay(50);
    digitalWrite(DONE_WDT, LOW);
    delay(2);
    digitalWrite(DONE_WDT, HIGH);
}

//-----------------------------------------
//
//-----------------------------------------
void enterSleep(void)
{
    // Serial1.flush(); nicht mehr hier, er hängt sich seit 01.07.21 hier auf
    SetLed(0);
    Wdt_DoneImpuls();

    pinMode(0, INPUT_PULLDOWN); // RxD -20uA
    // D1 = TxD1
    // D2 = SS
    // D3 = RxD2 siehe CSPPM_PowerDown()
    // D4 = TxD2 dito

    // pinMode(2, OUTPUT);
    // digitalWrite(2, LOW);

    pinMode(5, OUTPUT);
    digitalWrite(5, LOW); // Messspannung ausschalten

    pinMode(6, INPUT); // DIO0
    // pinMode(6,OUTPUT);
    // digitalWrite(6, LOW);

    pinMode(7, INPUT); // DIO2
    // pinMode(7,OUTPUT);
    // digitalWrite(7, LOW);

    // D8 = RFM Reset

    // D9 = I2C ausschalten
    digitalWrite(I2C_POWER, LOW); // 3V Power Pin

    // D10 = OUT1 = schaltet 3->5V

    pinMode(11, OUTPUT);
    digitalWrite(11, LOW);

    pinMode(12, INPUT); // DIO1
    // pinMode(12,OUTPUT);
    // digitalWrite(12, LOW);

    // D13 = LED

    // A0 = D14 Analog Akkuspannung

    // pinMode(15, INPUT_PULLDOWN);     // => 7uA weniger
    pinMode(15, OUTPUT);
    digitalWrite(15, LOW);

    // A2 = D16
    // pinMode(16, INPUT_PULLDOWN);
    pinMode(16, OUTPUT);
    digitalWrite(16, LOW);

    // A3 = D17
    // pinMode(17, INPUT_PULLDOWN);
    pinMode(17, OUTPUT);
    digitalWrite(17, LOW);

    pinMode(18, OUTPUT); // Akku enable
    digitalWrite(18, LOW);

    pinMode(19, OUTPUT);
    digitalWrite(19, LOW);

    for (int i = 17; i < 20; i++)
        pinMode(i, OUTPUT);

    // 20+21 = I2C
    pinMode(20, INPUT_PULLDOWN);
    pinMode(21, INPUT_PULLDOWN);

    // MISO
    pinMode(22, INPUT_PULLDOWN);

    // pinMode(42, INPUT_PULLDOWN);

    // SetMySystemClock(1);  //1MHz
}
//-------------------------------------------------

//-------------------------------------------------------------------
void PowerUp(void)
{
    int i;

    Serial1.begin(19200);
    // SetMySystemClock(6);

    // I2C Power einschalten LoRa Transponder
    pinMode(I2C_POWER, OUTPUT);
    digitalWrite(I2C_POWER, HIGH);
    delay(10 / 8);

    Wire.begin();
}

//----------------------------------------------------------------------------------------
void Sendung(void)
{
    int i;
    Serial1.println(F("Sendung "));

    PowerUp();

    Messen_Wetter(); // messen

    hseData2GlobalData(); // kopier hse2020 in global Sende Buffer

    // manual reset RF96 Modul
    pinMode(RESET_RF96, OUTPUT);
    digitalWrite(RESET_RF96, LOW);
    delay(2); // nur 1 weil CPU mit 6MHz laeuft
    digitalWrite(RESET_RF96, HIGH);
    delay(2);

    SetLed(2);

    /* To do the lora cycle*/
    LoraCycle();

    SetLed(0); // LED aus
    Serial1.println(F("aus "));

    // Strom sparen und RF95 ausschalten
    /* Shutdown the RFM95 module */
    /* Save power and turn off the RFM95 */
    RFM95Shutdown();

    Serial1.flush();
}

//-----------------------------------------------------------------------------------------
// Setup Setup Setup Setup Setup Setup Setup Setup Setup Setup
//-----------------------------------------------------------------------------------------
void setup()
{
    int i;
    // usb auschalten
    USBDevice.detach();

    Serial1.begin(19200); // *4 weil CPU mit 6MHz laeuft
    Serial1.println(F("Tx Modbus-Druck LoRaWan V1.10-6"));
    Serial1.println(F(__FILE__));
    Serial1.println(F(__DATE__));

    Build_DevAddr();

    PrintDeviceID();
    InitTimeDate();

    //----------------------------------
    Serial1.println(F("PowerUp"));
    Serial1.flush();
    PowerUp();

    SetLed(0);
    ucTxPower = 5;
    Serial1.println(F("ChipTest"));
    Serial1.flush();

    Init_Messen_Wetter();

    Sendung();

    iNextTxSec = 10;

    /* Shutdown the RFM95 module */
    RFM95Shutdown();

    iSinceLastTx = 0;
    Serial1.println(F("Ende Setup"));
    Serial1.flush();
}

//----------------------------------------
void loop()
{

    // 8s Blitz
    SetLed(1);

    // delay(1);

    iSinceLastTx += 10;

    if (iSinceLastTx > iNextTxSec)
    {
        Sendung();
        iSinceLastTx = 0;
    }

    enterSleep();
    LowPower.sleep(3000); // nun kannst ihn aus schalten
}
