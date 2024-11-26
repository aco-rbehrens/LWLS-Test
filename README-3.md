# LoRa: OTAA implementation

Updated the software to transition an IoT device with a SAM D21 microcontroller from ABP (Activation by Personalization) to OTAA (Over-the-Air Activation) in LoRaWAN. We are using MCCI LoRaWAN LMIC library as a LoRaWAN stack to enable the OTAA feature.

The OTAA feature has been successfully implemented and verified, ensuring reliable LoRa communication for both uplink and downlink messages. EEPROM functionality is integrated to store and restore session status, supporting seamless reconnections. The resend flow (sending uplink) has been designed to operate without requiring a new join request, enhancing efficiency. Failure handling mechanisms have been incorporated.

## Requirements

- [MCCI LoRaWAN LMIC library](https://github.com/mcci-catena/arduino-lmic) v4.1.1

### MCCI LoRaWAN LMIC library

- Name: MCCI LoRaWAN LMIC library
- Version: 4.1.1
- Sentence: Arduino port of the LMIC (LoraWAN-MAC-in-C) framework provided by IBM.
- Description: LoRaWAN 1.0.3 Class A devices implemented using the Semtech SX1276 (including HopeRF RFM95). Support for EU868 regional plans.
- LoRa library by: Sandeep Mistry

## Operation
### Build and program the application

1. Import our project into Arduino IDE.

2. Go to Library Manager and install the MCCI LoRaWAN LMIC library.
   
   **Figure 1. Install the MCCI LoRaWAN LMIC Library**

   ![](doc/images/Install-the-MCCI-LoRaWAN-LMIC-Library.png)
   
3. After installing the MCCI LoRaWAN LMIC Library, go to the following path **<ARDUINO_LIBRARY_PATH>\libraries\MCCI_LoRaWAN_LMIC_library\project_config\lmic_project_config.h** and Edit the **lmic_project_config.h** file.
  
   Clear all existing content in the file and replace it with the content provided below:

   ```
      /* project-specific definitions */
      #define CFG_eu868 1
      #define CFG_sx1276_radio 1
      #define LMIC_LORAWAN_SPEC_VERSION    LMIC_LORAWAN_SPEC_VERSION_1_0_3
   ```
4. Go to the following path **<PROJECT_DIRECTORY_PATH>\Modbus_Druck\LoRaMAC.C** and Edit the **LoRaMAC.C** file to modify the APP EUI, APP Key, Device EUI, and other user configurations such as RX delay and join retry count etc.

    > **Note:** Update the APP EUI, APP Key, and Device EUI to match the End Device APP EUI, APP Key, and Device EUI as configured on The Things Network.

5. Build and program the application.

6. Verify the functionality.
   
      **Figure 2. output**

   ![](doc/images/Combined-screenshot-of-end-device-logs-and-TTN-logs.png)


## The Things Network - LoRa server setup

1. Configure an end device using the settings below to verify functionality with the provided code.
   
   AppEUI : 0000000000000001 <br>
   DevEUI : 136FA65B004F4341 <br>
   AppKey : A3190D4AA1B2EF4193197A84620629D1


      **Figure 3. Create an end device on TTN**
   ![](doc/images/Create-an-end-device-on-TTN.png)


You may adjust the settings/configuration as needed, ensuring the source code matches the configuration in The Things Network (TTN).

   > **Note:** Adjust the user configuration in the **LoRaMAC.C** file to align with the End Device settings configured on The Things Network.
  

## User configuration

**Table 1. Configuration**

Configured via **<PROJECT_DIRECTORY_PATH>\Modbus_Druck\LoRaMAC.C** file.

 Variable | Description
 -------- |------------
 `APPEUI`             | This EUI must be in little-endian format, so least-significant-byte first. When copying an EUI from ttnctl output, this means to reverse
  the bytes.
 `DEVEUI`             | This should be in little endian format, so least-significant-byte first. When copying an EUI from ttnctl output, this means to reverse the bytes.
 `APPKEY`             | This key should be in big endian format. In practice, a key taken from ttnctl can be copied as-is.
  `MAX_RETRY_COUNT`             | Maximum retry count for joining.
  `RX_DELAY`             | RX delay. LoRa RX waiting time for RX window 1 and 2.
`lmic_pins`             | this structure contains pin mapping.

