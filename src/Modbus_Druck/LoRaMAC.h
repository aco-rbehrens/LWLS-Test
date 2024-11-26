/****************************************************************************************
 *
 * File:                  LoRaMAC.h
 * Created on: 			  06-11-2024
 * Supported Hardware:    SAMD21 board with RFM95
 *
 * Firmware version: 1.0
 * First version
 *
 ****************************************************************************************/

#ifndef LORAMAC_H
#define LORAMAC_H

#include <Arduino.h>
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include "eepromFuncMB85.h"

/*
*****************************************************************************************
* FUNCTION PROTOTYPES
*****************************************************************************************
*/
void ResetSessionData(void);
void DoSend(osjob_t *j);
void LoraCycle(void);
void RFM95Shutdown(void);

#endif
