/*
	THIS IS GENERATED CODE 
	DO NOT EDIT
	Developer: Max Reble
	Copyright Notice: hse-electronics GmbH, Max Reble (2020)
	License: MIT License (see LICENSE.txt)
*/
#include <stdint.h>
#ifndef _HSE_SENSOR_PROTOCOL_H_
#define _HSE_SENSOR_PROTOCOL_H_


#define HSESP_DIGITAL_INPUT 0x01		//uint8_t
#define HSESP_DIGITAL_INPUT_SIZE sizeof(uint8_t)

#define HSESP_DIGITAL_OUTPUT 0x02		//uint8_t
#define HSESP_DIGITAL_OUTPUT_SIZE sizeof(uint8_t)

#define HSESP_ANALOG_INPUT 0x03		//int16_t
#define HSESP_ANALOG_INPUT_SIZE sizeof(int16_t)

#define HSESP_ANALOG_OUTPUT 0x04		//int16_t
#define HSESP_ANALOG_OUTPUT_SIZE sizeof(int16_t)

#define HSESP_TEMPERATURE 0x10		//int16_t
#define HSESP_TEMPERATURE_SIZE sizeof(int16_t)

#define HSESP_HUMIDITY 0x11		//uint8_t
#define HSESP_HUMIDITY_SIZE sizeof(uint8_t)

#define HSESP_PRESSURE 0x12		//uint16_t
#define HSESP_PRESSURE_SIZE sizeof(uint16_t)

#define HSESP_ILLUMINANCE 0x13		//uint16_t
#define HSESP_ILLUMINANCE_SIZE sizeof(uint16_t)

#define HSESP_CONCENTRATION 0x14		//uint16_t
#define HSESP_CONCENTRATION_SIZE sizeof(uint16_t)

#define HSESP_DIRECTION 0x15		//int16_t
#define HSESP_DIRECTION_SIZE sizeof(int16_t)

#define HSESP_RSSI 0x16		//uint8_t
#define HSESP_RSSI_SIZE sizeof(uint8_t)

#define HSESP_LOUDNESS 0x17		//uint8_t
#define HSESP_LOUDNESS_SIZE sizeof(uint8_t)

#define HSESP_DISTANCE 0x18		//uint16_t
#define HSESP_DISTANCE_SIZE sizeof(uint16_t)

#define HSESP_COUNTER 0x19		//int16_t
#define HSESP_COUNTER_SIZE sizeof(int16_t)

#define HSESP_VOLTAGE 0x1A		//int16_t
#define HSESP_VOLTAGE_SIZE sizeof(int16_t)

#define HSESP_CONDUCTIVITY 0x1B		//uint16_t
#define HSESP_CONDUCTIVITY_SIZE sizeof(uint16_t)

#define HSESP_LEVEL 0x1C		//uint8_t
#define HSESP_LEVEL_SIZE sizeof(uint8_t)

#define HSESP_PRECIPITATION 0x1D		//uint16_t
#define HSESP_PRECIPITATION_SIZE sizeof(uint16_t)

#define HSESP_LOCATION 0x1E		//float
#define HSESP_LOCATION_SIZE sizeof(float)

#define HSESP_GENERIC_UINT8 0x70		//uint8_t
#define HSESP_GENERIC_UINT8_SIZE sizeof(uint8_t)

#define HSESP_GENERIC_UINT16 0x71		//uint16_t
#define HSESP_GENERIC_UINT16_SIZE sizeof(uint16_t)

#define HSESP_GENERIC_INT16 0x72		//int16_t
#define HSESP_GENERIC_INT16_SIZE sizeof(int16_t)

#define HSESP_GENERIC_UINT32 0x73		//uint32_t
#define HSESP_GENERIC_UINT32_SIZE sizeof(uint32_t)

#define HSESP_GENERIC_INT32 0x74		//int32_t
#define HSESP_GENERIC_INT32_SIZE sizeof(int32_t)

#define HSESP_GENERIC_FLOAT 0x75		//float
#define HSESP_GENERIC_FLOAT_SIZE sizeof(float)

#define HSESP_GENERIC_A_N_S_I_CHAR 0x76		//char
#define HSESP_GENERIC_A_N_S_I_CHAR_SIZE sizeof(char)

#define HSESP_GENERIC_U_T_F16_CHAR 0x77		//wchar_t
#define HSESP_GENERIC_U_T_F16_CHAR_SIZE sizeof(wchar_t)

#define HSESP_GENERIC_BOOLEAN 0x78		//bool
#define HSESP_GENERIC_BOOLEAN_SIZE sizeof(bool)

#define HSESP_ClimateSensor 0x80
#define HSESP_ClimateSensor_SIZE 7

#define HSESP_GPSSensor 0x81
#define HSESP_GPSSensor_SIZE 9

#define HSESP_PositionSensor 0x82
#define HSESP_PositionSensor_SIZE 10

class HseSP{
public:
HseSP(uint8_t version, uint8_t size);
~HseSP();

uint8_t getSize(void);
uint8_t *getBuffer(void);
void reset();


bool addDigitalInput(uint8_t value);
bool addDigitalInput(uint8_t *values, uint8_t len);
bool addDigitalOutput(uint8_t value);
bool addDigitalOutput(uint8_t *values, uint8_t len);
bool addAnalogInput(float value);
bool addAnalogInput(float *values, uint8_t len);
bool addAnalogOutput(float value);
bool addAnalogOutput(float *values, uint8_t len);
bool addTemperature(float value);
bool addTemperature(float *values, uint8_t len);
bool addHumidity(float value);
bool addHumidity(float *values, uint8_t len);
bool addPressure(float value);
bool addPressure(float *values, uint8_t len);
bool addIlluminance(uint16_t value);
bool addIlluminance(uint16_t *values, uint8_t len);
bool addConcentration(float value);
bool addConcentration(float *values, uint8_t len);
bool addDirection(float value);
bool addDirection(float *values, uint8_t len);
bool addRSSI(int16_t value);
bool addRSSI(int16_t *values, uint8_t len);
bool addLoudness(uint8_t value);
bool addLoudness(uint8_t *values, uint8_t len);
bool addDistance(float value);
bool addDistance(float *values, uint8_t len);
bool addCounter(int16_t value);
bool addCounter(int16_t *values, uint8_t len);
bool addVoltage(float value);
bool addVoltage(float *values, uint8_t len);
bool addConductivity(uint16_t value);
bool addConductivity(uint16_t *values, uint8_t len);
bool addLevel(uint8_t value);
bool addLevel(uint8_t *values, uint8_t len);
bool addPrecipitation(float value);
bool addPrecipitation(float *values, uint8_t len);
bool addLocation(float value);
bool addLocation(float *values, uint8_t len);
bool addGenericUInt8(uint8_t value);
bool addGenericUInt8(uint8_t *values, uint8_t len);
bool addGenericUInt16(uint16_t value);
bool addGenericUInt16(uint16_t *values, uint8_t len);
bool addGenericInt16(int16_t value);
bool addGenericInt16(int16_t *values, uint8_t len);
bool addGenericUInt32(uint32_t value);
bool addGenericUInt32(uint32_t *values, uint8_t len);
bool addGenericInt32(int32_t value);
bool addGenericInt32(int32_t *values, uint8_t len);
bool addGenericFloat(float value);
bool addGenericFloat(float *values, uint8_t len);
bool addGenericANSIChar(char value);
bool addGenericANSIChar(char *values, uint8_t len);
bool addGenericUTF16Char(wchar_t value);
bool addGenericUTF16Char(wchar_t *values, uint8_t len);
bool addGenericBoolean(bool value);
bool addGenericBoolean(bool *values, uint8_t len);
//ClimateSensor
typedef struct {
float Temperature;
float Humidity;
float Pressure;
uint16_t Illuminance;
} ClimateSensor_t;
bool addClimateSensor(ClimateSensor_t *values, uint8_t len = 1);

//GPSSensor
typedef struct {
float Latitude;
float Longitude;
int16_t SignalStrength;
} GPSSensor_t;
bool addGPSSensor(GPSSensor_t *values, uint8_t len = 1);

//PositionSensor
typedef struct {
float GSensorX;
float GSensorY;
float GSensorZ;
uint16_t Totalofshocks;
uint16_t Maxshocks;
} PositionSensor_t;
bool addPositionSensor(PositionSensor_t *values, uint8_t len = 1);


private:
uint8_t *buffer;
uint8_t maxsize;
uint8_t dataCursor;

void generateCRC(void);
bool addDataHeader(uint8_t type, uint8_t dataSize, uint8_t dataLen);

};
#endif
