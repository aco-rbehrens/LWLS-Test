/****************************************************************************************
 *
 * File:                  LoRaMAC.C
 * Created on: 			  06-11-2024
 * Supported Hardware:    SAMD21 board with RFM95
 *
 * Firmware version: 1.0
 * First version
 *
 ****************************************************************************************/

/*
*****************************************************************************************
* INCLUDE FILES
*****************************************************************************************
*/
#include "LoRaMAC.h"
#include "Platine.h"

/*
*****************************************************************************************
* INCLUDE MACRO, GLOBAL VARIABLES AND PROTOTYPES
*****************************************************************************************
*/

/* Maximum retry count for joining */
#define MAX_RETRY_COUNT 2

/* RX delay */
#define RX_DELAY 5

/* Device EUI */
u1_t DEVEUI[8] = {0};

/* To reset the session status to join the lora network */
/* To create a new session */
bool reset_session_status = true;

/* To check whether the lora cycle is happen or not */
bool is_lora_cycle_active = false;

/* Join success flag
 * This flag is used to manage the join success scenario
 * It is set in the event handler of the LMIC MAC */
bool join_success_flag = false;

/* Join failure flag
 * This flag is used to manage the join failure scenario
 * It is set in the event handler of the LMIC MAC */
bool join_failure_flag = false;

/* Tx completed flag
 * This flag is used to manage the Tx completed scenario
 * It is set in the event handler of the LMIC MAC */
bool tx_complete_flag = false;

/* re-attempt count for joining */
int retry_count = 0;

/* Argument for the function which send the data (uplink) to the server */
osjob_t sendjob;

unsigned char Data_Tx[64];
unsigned char Data_Rx[64];
unsigned char Data_Length_Tx;
unsigned char Data_Length_Rx = 0x00;
unsigned char ucTxPower, ucResetQuelle;
int iTest, iNewDay;
int iNextTxSec, iSinceLastTx;

/* This EUI must be in little-endian format, so least-significant-byte
  first. When copying an EUI from ttnctl output, this means to reverse
  the bytes. */
/* Store the array in flash memory (instead of RAM) */
static const u1_t PROGMEM APPEUI[8] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
void os_getArtEui(u1_t *buf) { memcpy_P(buf, APPEUI, 8); }

/* This should be in little endian format, so least-significant-byte
 first. When copying an EUI from ttnctl output, this means to reverse
 the bytes. */
/* This device DEVEUI : 136FA65B004F4341 */
void os_getDevEui(u1_t *buf) { memcpy_P(buf, DEVEUI, 8); }

/* This key should be in big endian format. In
 practice, a key taken from ttnctl can be copied as-is. */
/* Store the array in flash memory (instead of RAM) */
static const u1_t PROGMEM APPKEY[16] = {0xA3, 0x19, 0x0D, 0x4A, 0xA1, 0xB2, 0xEF, 0x41, 0x93, 0x19, 0x7A, 0x84, 0x62, 0x06, 0x29, 0xD1};
void os_getDevKey(u1_t *buf) { memcpy_P(buf, APPKEY, 16); }

/* Pin mapping */
const lmic_pinmap lmic_pins = {
	.nss = NSS1,
	.rxtx = LMIC_UNUSED_PIN,
	.rst = RESET_RF96,
	.dio = {6, 12, 7}, /* DIO1, DIO2, DIO3 */
};

extern void Wdt_DoneImpuls(void);

/*
*****************************************************************************************
* FUNCTIONS
*****************************************************************************************
*/

/*
*****************************************************************************************
* Description : Function to reset the session data if a fresh join is needed
*****************************************************************************************
*/
void ResetSessionData(void)
{
	// Reset DevAddr, NwkSKey, and AppSKey to trigger a new join
	LMIC.devaddr = 0;
	memset(LMIC.nwkKey, 0, sizeof(LMIC.nwkKey));
	memset(LMIC.artKey, 0, sizeof(LMIC.artKey));
	LMIC.seqnoUp = 0;
	LMIC.seqnoDn = 0;
}

/*
*****************************************************************************************
* Description : Event handler
*****************************************************************************************
*/
void onEvent(ev_t ev)
{
	switch (ev)
	{
	case EV_SCAN_TIMEOUT:
		Serial1.println(F("EV_SCAN_TIMEOUT"));
		break;
	case EV_BEACON_FOUND:
		Serial1.println(F("EV_BEACON_FOUND"));
		break;
	case EV_BEACON_MISSED:
		Serial1.println(F("EV_BEACON_MISSED"));
		break;
	case EV_BEACON_TRACKED:
		Serial1.println(F("EV_BEACON_TRACKED"));
		break;
	case EV_JOINING:
		Serial1.println(F("EV_JOINING"));
		break;
	case EV_JOINED:
		Serial1.println(F("EV_JOINED"));
		join_success_flag = true;
		break;
	case EV_JOIN_FAILED:
		Serial1.println(F("EV_JOIN_FAILED"));
		join_failure_flag = true;
		break;
	case EV_REJOIN_FAILED:
		Serial1.println(F("EV_REJOIN_FAILED"));
		join_failure_flag = true;
		break;
	case EV_TXCOMPLETE:
		Serial1.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
		tx_complete_flag = true;
		break;
	case EV_LOST_TSYNC:
		Serial1.println(F("EV_LOST_TSYNC"));
		break;
	case EV_RESET:
		Serial1.println(F("EV_RESET"));
		break;
	case EV_RXCOMPLETE:
		// data received in ping slot
		Serial1.println(F("EV_RXCOMPLETE"));
		break;
	case EV_LINK_DEAD:
		Serial1.println(F("EV_LINK_DEAD"));
		break;
	case EV_LINK_ALIVE:
		Serial1.println(F("EV_LINK_ALIVE"));
		break;
	case EV_TXSTART:
		Serial1.println(F("EV_TXSTART"));
		break;
	case EV_TXCANCELED:
		Serial1.println(F("EV_TXCANCELED"));
		is_lora_cycle_active = false;
		break;
	case EV_RXSTART:
		/* do not print anything -- it wrecks timing */
		break;
	case EV_JOIN_TXCOMPLETE:
		Serial1.println(F("EV_JOIN_TXCOMPLETE: no JoinAccept"));
		join_failure_flag = true;
		break;

	default:
		Serial1.print(F("Unknown event: "));
		Serial1.println((unsigned)ev);
		break;
	}
}

/*
*****************************************************************************************
* Description : To send the LoRa packet to the server. To do a uplink
*****************************************************************************************
*/
void DoSend(osjob_t *j)
{
	// Check if there is a current TX/RX job running
	if (LMIC.opmode & OP_TXRXPEND)
	{
		Serial1.println(F("OP_TXRXPEND, not sending"));
	}
	else
	{
		// Prepare upstream data transmission at the next possible time.
		LMIC_setTxData2(1, Data_Tx, Data_Length_Tx, 0);
		Serial1.println(F("Packet queued"));
	}
}

/*
*****************************************************************************************
* Description : Function that handles one cycle of sending and receiving with the LoRaWAN protocol.
*               In this function the timing of receive slot 2 is handeld
*****************************************************************************************
*/
void LoraCycle(void)
{
	/* Enabling the flag, to start the lora cycle */
	is_lora_cycle_active = true;

	/* LMIC init */
	os_init();

	/* Reset the MAC state. Session and pending data transfers will be discarded. */
	LMIC_reset();

	/* Set RX delay to match your network's settings */
	LMIC.rxDelay = RX_DELAY;

	/* Verify whether we need to reset the session status */
	/* To start a new connection with new session keys and address */
	if (reset_session_status)
	{
		/* Reset the session data if a fresh join is needed */
		ResetSessionData();

		/* Start the joining process */
		LMIC_startJoining();
	}
	else
	{
		LMIC.opmode &= ~OP_JOINING; /* Clear the joining flag */

		/* Restore the session data to send the data (uplink) to the network without joining again */
		restoreSessionKeys();
		restoreFrameCount();

		/* To send the uplink */
		DoSend(&sendjob);
	}

	/* Waiting to finish the lora operation (joining, to sending uplink, to receiving downlink etc.,) */
	while (is_lora_cycle_active)
	{
		os_runloop_once();

		if (join_success_flag)
		{
			{
				/* Save the session keys and addr into the EEPROM */
				saveSessionKeys();
				saveFrameCount();
			}
			/* Disable link check validation (automatically enabled
			   during join */
			LMIC_setLinkCheckMode(0);
			/* Mark as joined to skip rejoin */
			reset_session_status = false;
			/* To send the uplink */
			DoSend(&sendjob);
			/* reset the LoRa join success flag*/
			join_success_flag = false;
		}

		if (join_failure_flag)
		{
			if (retry_count == MAX_RETRY_COUNT)
			{
				/* reset the retry count */
				retry_count = 0;
				is_lora_cycle_active = false;
			}
			else
			{
				/* increment the retry count */
				retry_count++;
				/* Kick the watchdog timer */
				Wdt_DoneImpuls();
				/* Start the joining process */
				LMIC_startJoining();
				reset_session_status = true;
			}
			/* reset the LoRa join failure flag*/
			join_failure_flag = false;
		}

		if (tx_complete_flag)
		{
			if (LMIC.txrxFlags & TXRX_ACK)
				Serial1.println(F("Received ack"));
			if (LMIC.dataLen)
			{
				Serial1.print(F("Received "));
				Data_Length_Rx = LMIC.dataLen;
				Serial1.print(Data_Length_Rx);
				Serial1.println(F(" bytes of payload"));
				// Print received downlink data
				Serial1.print(F("Downlink data Data_Rx: "));

				memcpy(Data_Rx, &LMIC.frame[LMIC.dataBeg], Data_Length_Rx);
				for (int i = 0; i < Data_Length_Rx; i++)
				{
					Serial1.print(Data_Rx[i], HEX);
					Serial1.print(" ");
				}
				Serial1.println();
			}
			Serial1.println(F("TXCOMPLETE Done"));
			/* Save the current frame count into the EEPROM */
			saveFrameCount();
			is_lora_cycle_active = false;
			/* reset the LoRa tx complete flag*/
			tx_complete_flag = false;
		}
	}

	Serial1.println(F("Lora cycle completed "));
}

/*
*****************************************************************************************
* Description : To shutdown the RFM95 module, to keep power consumption low .
*               Save power and turn off the RFM95.
*****************************************************************************************
*/
void RFM95Shutdown(void)
{
	/* shutdown the RFM95 module */
	LMIC_shutdown();
}
