
#ifndef h_messen
#define h_messen


#define ONE_WIRE_BUS 19
#include <DallasTemperature.h>

void InitTimeDate(void);
void PrintTimeDate(void);

void Init_Messen_Wetter(void);
void I2C_ChipTest(void);

void SetMySystemClock(int iMHz);
void Messen_Wetter(void);
void hseData2GlobalData(void);

long readVcc();

void ServiceDatenBilden(void);




extern long uiOnStd;
extern int iNextTxSec;
extern  unsigned char Data_Tx[64];
extern unsigned char Data_Length_Tx;





#endif
