
// EEPROM Funktionen 24AA02

#include <Arduino.h>
#include <Wire.h>
#include "LoRaMAC.h"
#include "eepromFuncMB85.h"

// #define DEBUG_PRINT

#ifndef TAKT_KORREKTUR
#define TAKT_KORREKTUR 1
#endif

//----------------------------------------------------------------------------------
byte i2c_eeprom_read_byte(unsigned int eeaddress)
{
  byte rdata = 0x88; // 88 = error

  Wire.beginTransmission((uint8_t)0x50);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB

  Wire.endTransmission();
  Wire.requestFrom((uint8_t)0x50, (byte)1);
  if (Wire.available())
    rdata = Wire.read();

  return rdata;
}
//-------------------------------------------------------------------------------------
void i2c_eeprom_write_byte(unsigned int eeaddress, unsigned char data)
{
  // Beschleunigung, wenn es nichts zu tun gibt
  if (i2c_eeprom_read_byte(eeaddress) == data)
    return;

  // ja du musst doch schreiben und warten
  Wire.beginTransmission(0x50);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  // Wire.endTransmission();
  Wire.write(data); // Data byte
  Wire.endTransmission();
  delay(20 / TAKT_KORREKTUR);
}
//----------------------------------------

//----------------------------------------
// Ist das EEPROM noch leer
int IsEEpromEmpty(void)
{
  if ((i2c_eeprom_read_byte(0) == 0xFF) &&
      (i2c_eeprom_read_byte(1) == 0xFF) &&
      (i2c_eeprom_read_byte(2) == 0xFF) &&
      (i2c_eeprom_read_byte(3) == 0xFF))
    return (true);
  else
    return (false);
}
//----------------------------------------
// Boot counter
// wird bei jedem Reset +1
// um Framecounter nach Reset zu veraendern
uint16_t AddBootCounter(void)
{
  uint16_t n;

  n = i2c_eeprom_read_byte(EEPROM_ADR_BOOT_COUNTER);
  n <<= 8;
  n |= i2c_eeprom_read_byte(EEPROM_ADR_BOOT_COUNTER + 1);
  n++;

  i2c_eeprom_write_byte(EEPROM_ADR_BOOT_COUNTER + 1, (uint8_t)n);
  i2c_eeprom_write_byte(EEPROM_ADR_BOOT_COUNTER, n >> 8);

  return (n);
}
//----------------------------------------
// Save Uplink Frame Counter to EEPROM
void SaveUplinkFrameCounter(u4_t SeqnoUp)
{
  i2c_eeprom_write_byte(EEPROM_ADR_UP_FRAME_COUNTER + 1, (uint8_t)SeqnoUp);
  i2c_eeprom_write_byte(EEPROM_ADR_UP_FRAME_COUNTER, SeqnoUp >> 8);

#ifdef DEBUG_PRINT
  Serial1.print(F("save uplink frame count= "));
  Serial1.println(SeqnoUp);
  Serial1.println("");
#endif
}
//----------------------------------------
// Restore Uplink Frame Counter to EEPROM
void RestoreUplinkFrameCounter(u4_t *SeqnoUp)
{
  uint32_t n;

  n = i2c_eeprom_read_byte(EEPROM_ADR_UP_FRAME_COUNTER);
  n <<= 8;
  n |= i2c_eeprom_read_byte(EEPROM_ADR_UP_FRAME_COUNTER + 1);

  *SeqnoUp = n;

#ifdef DEBUG_PRINT
  Serial1.print(F("restore uplink frame count= "));
  Serial1.println(*SeqnoUp);
  Serial1.println("");
#endif
}
//----------------------------------------
// Save Downlink Frame Counter to EEPROM
void SaveDownlinkFrameCounter(u4_t SeqnoDn)
{
  i2c_eeprom_write_byte(EEPROM_ADR_DOWN_FRAME_COUNTER + 1, (uint8_t)SeqnoDn);
  i2c_eeprom_write_byte(EEPROM_ADR_DOWN_FRAME_COUNTER, SeqnoDn >> 8);

#ifdef DEBUG_PRINT
  Serial1.print(F("save Downlink frame count= "));
  Serial1.println(SeqnoDn);
  Serial1.println("");
#endif
}
//----------------------------------------
// Restore Downlink Frame Counter to EEPROM
void RestoreDownlinkFrameCounter(u4_t *SeqnoDn)
{
  uint32_t n;

  n = i2c_eeprom_read_byte(EEPROM_ADR_DOWN_FRAME_COUNTER);
  n <<= 8;
  n |= i2c_eeprom_read_byte(EEPROM_ADR_DOWN_FRAME_COUNTER + 1);

  *SeqnoDn = n;

#ifdef DEBUG_PRINT
  Serial1.print(F("restore Downlink frame count= "));
  Serial1.println(*SeqnoDn);
  Serial1.println("");
#endif
}
//----------------------------------------
// Save Device Address to EEPROM
void SaveDeviceAddr(devaddr_t devaddr)
{
  i2c_eeprom_write_byte(EEPROM_ADR_DEVICE_ADR + 3, (uint8_t)devaddr);
  i2c_eeprom_write_byte(EEPROM_ADR_DEVICE_ADR + 2, (uint8_t)(devaddr >> 8));
  i2c_eeprom_write_byte(EEPROM_ADR_DEVICE_ADR + 1, (uint8_t)(devaddr >> 16));
  i2c_eeprom_write_byte(EEPROM_ADR_DEVICE_ADR, (uint8_t)(devaddr >> 24));

#ifdef DEBUG_PRINT
  Serial1.print(F("Save Device Addr= "));
  Serial1.println(devaddr, HEX);
  Serial1.println("");
#endif
}
//----------------------------------------
// Restore Device Address to EEPROM
void RestoreDeviceAddr(devaddr_t *devaddr)
{
  uint32_t n;

  n = i2c_eeprom_read_byte(EEPROM_ADR_DEVICE_ADR);
  n <<= 8;
  n |= i2c_eeprom_read_byte(EEPROM_ADR_DEVICE_ADR + 1);
  n <<= 8;
  n |= i2c_eeprom_read_byte(EEPROM_ADR_DEVICE_ADR + 2);
  n <<= 8;
  n |= i2c_eeprom_read_byte(EEPROM_ADR_DEVICE_ADR + 3);

  *devaddr = n;

#ifdef DEBUG_PRINT
  Serial1.print(F("restored Device Addr= "));
  Serial1.println(*devaddr, HEX);
  Serial1.println("");
#endif
}
//----------------------------------------
// Save Net ID to EEPROM
void SaveNetId(u4_t netid)
{
  i2c_eeprom_write_byte(EEPROM_ADR_DEVICE_ADR + 3, (uint8_t)netid);
  i2c_eeprom_write_byte(EEPROM_ADR_DEVICE_ADR + 2, (uint8_t)(netid >> 8));
  i2c_eeprom_write_byte(EEPROM_ADR_DEVICE_ADR + 1, (uint8_t)(netid >> 16));
  i2c_eeprom_write_byte(EEPROM_ADR_DEVICE_ADR, (uint8_t)(netid >> 24));

#ifdef DEBUG_PRINT
  Serial1.print(F("Net Id = "));
  Serial1.println(netid, HEX);
  Serial1.println("");
#endif
}
//----------------------------------------
// Restore Net ID to EEPROM
void RestoreNetId(u4_t *netid)
{
  uint32_t n;

  n = i2c_eeprom_read_byte(EEPROM_ADR_DEVICE_ADR);
  n <<= 8;
  n |= i2c_eeprom_read_byte(EEPROM_ADR_DEVICE_ADR + 1);
  n <<= 8;
  n |= i2c_eeprom_read_byte(EEPROM_ADR_DEVICE_ADR + 2);
  n <<= 8;
  n |= i2c_eeprom_read_byte(EEPROM_ADR_DEVICE_ADR + 3);

  *netid = n;

#ifdef DEBUG_PRINT
  Serial1.print(F("Net Id= "));
  Serial1.println(*netid, HEX);
  Serial1.println("");
#endif
}
//----------------------------------------
// Save Network Key to EEPROM
void SaveNwKey(uint8_t *nwkKey)
{
  uint8_t n[16] = {0};

  memcpy(n, nwkKey, sizeof(n));
  for (int i = 0; i <= 15; i++)
  {
    i2c_eeprom_write_byte(EEPROM_ADR_NWKS_KEY + i, (uint8_t)n[i]);
  }

#ifdef DEBUG_PRINT
  Serial1.println(F("Save nwkey= "));
  for (int i = 0; i <= 15; i++)
  {
    Serial1.print(n[i], HEX);
  }
  Serial1.println("");
#endif
}
//----------------------------------------
// Restore Newtork Key to EEPROM
void RestoreNwKey(u1_t *nwkKey)
{
  uint8_t n[16];

  for (int i = 0; i <= 15; i++)
  {
    n[i] = i2c_eeprom_read_byte(EEPROM_ADR_NWKS_KEY + i);
  }
  memcpy(nwkKey, n, sizeof(n));

#ifdef DEBUG_PRINT
  Serial1.println(F("restored nwkey= "));
  for (int i = 0; i <= 15; i++)
  {
    Serial1.print(LMIC.nwkKey[i], HEX);
  }
  Serial1.println("");
#endif
}
//----------------------------------------
// Save App Key to EEPROM
void SaveAppKey(uint8_t *appKey)
{
  uint8_t n[16] = {0};

  memcpy(n, appKey, sizeof(n));
  for (int i = 0; i <= 15; i++)
  {
    i2c_eeprom_write_byte(EEPROM_ADR_APPS_KEY + i, (uint8_t)n[i]);
  }

#ifdef DEBUG_PRINT
  Serial1.println(F("Save appkey= "));
  for (int i = 0; i <= 15; i++)
  {
    Serial1.print(n[i], HEX);
  }
  Serial1.println("");
#endif
}
//----------------------------------------
// Restore App Key to EEPROM
void RestorAppKey(u1_t *artKey)
{
  uint8_t n[16];

  for (int i = 0; i <= 15; i++)
  {
    n[i] = i2c_eeprom_read_byte(EEPROM_ADR_APPS_KEY + i);
  }
  memcpy(artKey, n, sizeof(n));

#ifdef DEBUG_PRINT
  Serial1.println(F("restored appkey= "));
  for (int i = 0; i <= 15; i++)
  {
    Serial1.print(LMIC.artKey[i], HEX);
  }
  Serial1.println("");
#endif
}
//----------------------------------------
// Save Session Keys to EEPROM
void saveSessionKeys()
{
  u4_t netid = 0;
  devaddr_t devaddr = 0;
  u1_t nwkKey[16] = {0};
  u1_t artKey[16] = {0};

  LMIC_getSessionKeys(&netid, &devaddr, nwkKey, artKey);

  Serial1.print("Net ID: ");
  Serial1.println(netid, HEX);
  Serial1.print("Dev Addr: ");
  Serial1.println(devaddr, HEX);
  Serial1.print("AppS Key: ");
  for (size_t i = 0; i < sizeof(artKey); ++i)
  {
    if (i != 0)
      Serial1.print("-");
    PrintHex(artKey[i]);
  }
  Serial1.println("");
  Serial1.print("NwkS Key: ");
  for (size_t i = 0; i < sizeof(nwkKey); ++i)
  {
    if (i != 0)
      Serial1.print("-");
    PrintHex(nwkKey[i]);
  }
  Serial1.println();

  SaveNetId(netid);
  SaveDeviceAddr(devaddr);
  SaveNwKey(nwkKey);
  SaveAppKey(artKey);
}
//----------------------------------------
// Restore Session Keys to EEPROM
void restoreSessionKeys()
{
  u4_t netid = 0;
  devaddr_t devaddr = 0;
  u1_t nwkKey[16] = {0};
  u1_t artKey[16] = {0};

  RestoreNetId(&netid);
  RestoreDeviceAddr(&devaddr);
  RestoreNwKey(nwkKey);
  RestorAppKey(artKey);

  Serial1.print("Net Id: ");
  Serial1.println(netid, HEX);
  Serial1.print("Dev Addr: ");
  Serial1.println(devaddr, HEX);
  Serial1.print("AppS Key: ");
  for (size_t i = 0; i < sizeof(artKey); ++i)
  {
    if (i != 0)
      Serial1.print("-");
    PrintHex(artKey[i]);
  }
  Serial1.println("");
  Serial1.print("NwkS Key: ");
  for (size_t i = 0; i < sizeof(nwkKey); ++i)
  {
    if (i != 0)
      Serial1.print("-");
    PrintHex(nwkKey[i]);
  }
  Serial1.println();

  LMIC_setSession(netid, devaddr, nwkKey, artKey);
}
//----------------------------------------
// Save Frame Count to EEPROM
void saveFrameCount()
{
  u4_t SeqnUp = LMIC_getSeqnoUp();
  u4_t SeqnoDn = LMIC.seqnoDn;

  SaveUplinkFrameCounter(SeqnUp);
  SaveDownlinkFrameCounter(SeqnoDn);
}
//----------------------------------------
// Restore Frame Count to EEPROM
void restoreFrameCount()
{
  u4_t SeqnUp = 0;
  u4_t SeqnoDn = 0;

  RestoreUplinkFrameCounter(&SeqnUp);
  RestoreDownlinkFrameCounter(&SeqnoDn);
  LMIC_setSeqnoUp(SeqnUp);
  LMIC.seqnoDn = SeqnoDn;
}
//----------------------------------------
// To print the hex value
void PrintHex(unsigned v)
{
  v &= 0xff;

  if (v < 16)
    Serial1.print('0');
  Serial1.print(v, HEX);
}
