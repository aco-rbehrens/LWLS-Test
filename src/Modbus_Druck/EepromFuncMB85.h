#ifndef _EEPROM_FUNC_
#define _EEPROM_FUNC_

#include <lmic.h>
#include <hal/hal.h>

#define DEVICE_ADR_MB85   0x50

byte i2c_eeprom_read_byte( unsigned int eeaddress );
void i2c_eeprom_write_byte( unsigned int eeaddress, unsigned char data );


//..
//#define DEBUG_PRINT

#define EEPROM_ADR_DEVICE_ADR          0
#define EEPROM_ADR_BOOT_COUNTER        4
#define EEPROM_ADR_UP_FRAME_COUNTER    6
#define EEPROM_ADR_DOWN_FRAME_COUNTER  10
#define EEPROM_ADR_NWKS_KEY            14
#define EEPROM_ADR_APPS_KEY            30
#define EEPROM_ADR_NEXT                46


void SaveDeviceAdrToOnChipEerom(void);
void ReadDeviceAdrFromOnChipEerom(void);
int IsEEpromEmpty(void);
uint16_t AddBootCounter(void);
void SaveUplinkFrameCounter(u4_t SeqnoUp);
void RestoreUplinkFrameCounter(u4_t * SeqnoUp);
void SaveDownlinkFrameCounter(u4_t SeqnoDn);
void RestoreDownlinkFrameCounter(u4_t * SeqnoDn);
void SaveDeviceAddr(devaddr_t  devaddr);
void RestoreDeviceAddr(devaddr_t * devaddr);
void SaveNwKey(uint8_t * nwkKey);
void RestoreNwKey(u1_t * nwkKey);
void SaveAppKey(uint8_t * appKey);
void RestorAppKey(u1_t * appey);
void SaveNetId(u4_t netid);
void RestoreNetId(u4_t * netid);
void saveSessionKeys(void);
void restoreSessionKeys(void);
void restoreFrameCount(void);
void saveFrameCount(void);
void PrintHex(unsigned v);

#endif
