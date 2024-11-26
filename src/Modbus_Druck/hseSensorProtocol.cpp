/*
	THIS IS GENERATED CODE 
	DO NOT EDIT
	Developer: Max Reble
	Copyright Notice: hse-electronics GmbH, Max Reble (2020)
	License: MIT License (see LICENSE.txt)
*/
#include "HseSensorProtocol.h"
#include <stdlib.h> //malloc
#include <string.h> //memcpy
#define __HSESP_VERSION buffer[0]

HseSP::HseSP(uint8_t version, uint8_t size) : maxsize(size)
{
buffer = (uint8_t*) malloc(size);
buffer[0] = version;
buffer[1] = 255 - version;
dataCursor = 4;
}

HseSP::~HseSP(void){free(buffer);}

uint8_t HseSP::getSize(void)
{
return dataCursor;
}

uint8_t *HseSP::getBuffer(void)
{
generateCRC();
return buffer;
}

void HseSP::reset(){
	dataCursor = 4;
}

void HseSP::generateCRC()
{
	uint16_t uiCRC16 = 0;
	uint8_t nElement, nBitPos;
	//Skip first four elements as its the header
	for (nElement = 4; nElement < dataCursor; nElement++)
		for (nBitPos = 0; nBitPos < 8; nBitPos++)
			if ((uiCRC16 ^ (buffer[nElement] >> nBitPos)) & 1)
				uiCRC16 = (uiCRC16 >> 1) ^ 0xA001;
			else
				uiCRC16 >>= 1;

	memcpy(buffer + 2, &uiCRC16, 2);
}

bool HseSP::addDataHeader(uint8_t type, uint8_t dataSize, uint8_t dataLen) {
	if (dataCursor + (__HSESP_VERSION == 1 ? (dataSize + 1) * dataLen : (dataSize * dataLen) + 2) > maxsize)
	return false;
	buffer[dataCursor++] = type;
	if (__HSESP_VERSION == 2)
	buffer[dataCursor++] = dataLen;
	return true;
}


bool HseSP::addDigitalInput(uint8_t value)
{
	return addDigitalInput(&value, 1);
}
bool HseSP::addDigitalInput(uint8_t *values, uint8_t len)
{
	if (!addDataHeader(HSESP_DIGITAL_INPUT, HSESP_DIGITAL_INPUT_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_DIGITAL_INPUT;
		memcpy(buffer + dataCursor, &values[i], sizeof(uint8_t));
		dataCursor += sizeof(uint8_t);
	}
	return true;
}
bool HseSP::addDigitalOutput(uint8_t value)
{
	return addDigitalOutput(&value, 1);
}
bool HseSP::addDigitalOutput(uint8_t *values, uint8_t len)
{
	if (!addDataHeader(HSESP_DIGITAL_OUTPUT, HSESP_DIGITAL_OUTPUT_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_DIGITAL_OUTPUT;
		memcpy(buffer + dataCursor, &values[i], sizeof(uint8_t));
		dataCursor += sizeof(uint8_t);
	}
	return true;
}
bool HseSP::addAnalogInput(float value)
{
	return addAnalogInput(&value, 1);
}
bool HseSP::addAnalogInput(float *values, uint8_t len)
{
	if (!addDataHeader(HSESP_ANALOG_INPUT, HSESP_ANALOG_INPUT_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_ANALOG_INPUT;
		int16_t val = values[i] * 100;
		memcpy(buffer + dataCursor, &val, sizeof(val));
		dataCursor += sizeof(val);
	}
	return true;
}
bool HseSP::addAnalogOutput(float value)
{
	return addAnalogOutput(&value, 1);
}
bool HseSP::addAnalogOutput(float *values, uint8_t len)
{
	if (!addDataHeader(HSESP_ANALOG_OUTPUT, HSESP_ANALOG_OUTPUT_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_ANALOG_OUTPUT;
		int16_t val = values[i] * 100;
		memcpy(buffer + dataCursor, &val, sizeof(val));
		dataCursor += sizeof(val);
	}
	return true;
}
bool HseSP::addTemperature(float value)
{
	return addTemperature(&value, 1);
}
bool HseSP::addTemperature(float *values, uint8_t len)
{
	if (!addDataHeader(HSESP_TEMPERATURE, HSESP_TEMPERATURE_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_TEMPERATURE;
		int16_t val = values[i] * 100;
		memcpy(buffer + dataCursor, &val, sizeof(val));
		dataCursor += sizeof(val);
	}
	return true;
}
bool HseSP::addHumidity(float value)
{
	return addHumidity(&value, 1);
}
bool HseSP::addHumidity(float *values, uint8_t len)
{
	if (!addDataHeader(HSESP_HUMIDITY, HSESP_HUMIDITY_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_HUMIDITY;
		uint8_t val = values[i] * 2;
		memcpy(buffer + dataCursor, &val, sizeof(val));
		dataCursor += sizeof(val);
	}
	return true;
}
bool HseSP::addPressure(float value)
{
	return addPressure(&value, 1);
}
bool HseSP::addPressure(float *values, uint8_t len)
{
	if (!addDataHeader(HSESP_PRESSURE, HSESP_PRESSURE_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_PRESSURE;
		uint16_t val = values[i] * 10;
		memcpy(buffer + dataCursor, &val, sizeof(val));
		dataCursor += sizeof(val);
	}
	return true;
}
bool HseSP::addIlluminance(uint16_t value)
{
	return addIlluminance(&value, 1);
}
bool HseSP::addIlluminance(uint16_t *values, uint8_t len)
{
	if (!addDataHeader(HSESP_ILLUMINANCE, HSESP_ILLUMINANCE_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_ILLUMINANCE;
		memcpy(buffer + dataCursor, &values[i], sizeof(uint16_t));
		dataCursor += sizeof(uint16_t);
	}
	return true;
}
bool HseSP::addConcentration(float value)
{
	return addConcentration(&value, 1);
}
bool HseSP::addConcentration(float *values, uint8_t len)
{
	if (!addDataHeader(HSESP_CONCENTRATION, HSESP_CONCENTRATION_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_CONCENTRATION;
		uint16_t val = values[i] * 10;
		memcpy(buffer + dataCursor, &val, sizeof(val));
		dataCursor += sizeof(val);
	}
	return true;
}
bool HseSP::addDirection(float value)
{
	return addDirection(&value, 1);
}
bool HseSP::addDirection(float *values, uint8_t len)
{
	if (!addDataHeader(HSESP_DIRECTION, HSESP_DIRECTION_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_DIRECTION;
		int16_t val = values[i] * 10;
		memcpy(buffer + dataCursor, &val, sizeof(val));
		dataCursor += sizeof(val);
	}
	return true;
}
bool HseSP::addRSSI(int16_t value)
{
	return addRSSI(&value, 1);
}
bool HseSP::addRSSI(int16_t *values, uint8_t len)
{
	if (!addDataHeader(HSESP_RSSI, HSESP_RSSI_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_RSSI;
		uint8_t val = values[i] * -1;
		memcpy(buffer + dataCursor, &val, sizeof(val));
		dataCursor += sizeof(val);
	}
	return true;
}
bool HseSP::addLoudness(uint8_t value)
{
	return addLoudness(&value, 1);
}
bool HseSP::addLoudness(uint8_t *values, uint8_t len)
{
	if (!addDataHeader(HSESP_LOUDNESS, HSESP_LOUDNESS_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_LOUDNESS;
		memcpy(buffer + dataCursor, &values[i], sizeof(uint8_t));
		dataCursor += sizeof(uint8_t);
	}
	return true;
}
bool HseSP::addDistance(float value)
{
	return addDistance(&value, 1);
}
bool HseSP::addDistance(float *values, uint8_t len)
{
	if (!addDataHeader(HSESP_DISTANCE, HSESP_DISTANCE_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_DISTANCE;
		uint16_t val = values[i] * 1000;
		memcpy(buffer + dataCursor, &val, sizeof(val));
		dataCursor += sizeof(val);
	}
	return true;
}
bool HseSP::addCounter(int16_t value)
{
	return addCounter(&value, 1);
}
bool HseSP::addCounter(int16_t *values, uint8_t len)
{
	if (!addDataHeader(HSESP_COUNTER, HSESP_COUNTER_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_COUNTER;
		memcpy(buffer + dataCursor, &values[i], sizeof(int16_t));
		dataCursor += sizeof(int16_t);
	}
	return true;
}
bool HseSP::addVoltage(float value)
{
	return addVoltage(&value, 1);
}
bool HseSP::addVoltage(float *values, uint8_t len)
{
	if (!addDataHeader(HSESP_VOLTAGE, HSESP_VOLTAGE_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_VOLTAGE;
		int16_t val = values[i] * 1000;
		memcpy(buffer + dataCursor, &val, sizeof(val));
		dataCursor += sizeof(val);
	}
	return true;
}
bool HseSP::addConductivity(uint16_t value)
{
	return addConductivity(&value, 1);
}
bool HseSP::addConductivity(uint16_t *values, uint8_t len)
{
	if (!addDataHeader(HSESP_CONDUCTIVITY, HSESP_CONDUCTIVITY_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_CONDUCTIVITY;
		memcpy(buffer + dataCursor, &values[i], sizeof(uint16_t));
		dataCursor += sizeof(uint16_t);
	}
	return true;
}
bool HseSP::addLevel(uint8_t value)
{
	return addLevel(&value, 1);
}
bool HseSP::addLevel(uint8_t *values, uint8_t len)
{
	if (!addDataHeader(HSESP_LEVEL, HSESP_LEVEL_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_LEVEL;
		memcpy(buffer + dataCursor, &values[i], sizeof(uint8_t));
		dataCursor += sizeof(uint8_t);
	}
	return true;
}
bool HseSP::addPrecipitation(float value)
{
	return addPrecipitation(&value, 1);
}
bool HseSP::addPrecipitation(float *values, uint8_t len)
{
	if (!addDataHeader(HSESP_PRECIPITATION, HSESP_PRECIPITATION_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_PRECIPITATION;
		uint16_t val = values[i] * 10;
		memcpy(buffer + dataCursor, &val, sizeof(val));
		dataCursor += sizeof(val);
	}
	return true;
}
bool HseSP::addLocation(float value)
{
	return addLocation(&value, 1);
}
bool HseSP::addLocation(float *values, uint8_t len)
{
	if (!addDataHeader(HSESP_LOCATION, HSESP_LOCATION_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_LOCATION;
		memcpy(buffer + dataCursor, &values[i], sizeof(float));
		dataCursor += sizeof(float);
	}
	return true;
}
bool HseSP::addGenericUInt8(uint8_t value)
{
	return addGenericUInt8(&value, 1);
}
bool HseSP::addGenericUInt8(uint8_t *values, uint8_t len)
{
	if (!addDataHeader(HSESP_GENERIC_UINT8, HSESP_GENERIC_UINT8_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_GENERIC_UINT8;
		memcpy(buffer + dataCursor, &values[i], sizeof(uint8_t));
		dataCursor += sizeof(uint8_t);
	}
	return true;
}
bool HseSP::addGenericUInt16(uint16_t value)
{
	return addGenericUInt16(&value, 1);
}
bool HseSP::addGenericUInt16(uint16_t *values, uint8_t len)
{
	if (!addDataHeader(HSESP_GENERIC_UINT16, HSESP_GENERIC_UINT16_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_GENERIC_UINT16;
		memcpy(buffer + dataCursor, &values[i], sizeof(uint16_t));
		dataCursor += sizeof(uint16_t);
	}
	return true;
}
bool HseSP::addGenericInt16(int16_t value)
{
	return addGenericInt16(&value, 1);
}
bool HseSP::addGenericInt16(int16_t *values, uint8_t len)
{
	if (!addDataHeader(HSESP_GENERIC_INT16, HSESP_GENERIC_INT16_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_GENERIC_INT16;
		memcpy(buffer + dataCursor, &values[i], sizeof(int16_t));
		dataCursor += sizeof(int16_t);
	}
	return true;
}
bool HseSP::addGenericUInt32(uint32_t value)
{
	return addGenericUInt32(&value, 1);
}
bool HseSP::addGenericUInt32(uint32_t *values, uint8_t len)
{
	if (!addDataHeader(HSESP_GENERIC_UINT32, HSESP_GENERIC_UINT32_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_GENERIC_UINT32;
		memcpy(buffer + dataCursor, &values[i], sizeof(uint32_t));
		dataCursor += sizeof(uint32_t);
	}
	return true;
}
bool HseSP::addGenericInt32(int32_t value)
{
	return addGenericInt32(&value, 1);
}
bool HseSP::addGenericInt32(int32_t *values, uint8_t len)
{
	if (!addDataHeader(HSESP_GENERIC_INT32, HSESP_GENERIC_INT32_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_GENERIC_INT32;
		memcpy(buffer + dataCursor, &values[i], sizeof(int32_t));
		dataCursor += sizeof(int32_t);
	}
	return true;
}
bool HseSP::addGenericFloat(float value)
{
	return addGenericFloat(&value, 1);
}
bool HseSP::addGenericFloat(float *values, uint8_t len)
{
	if (!addDataHeader(HSESP_GENERIC_FLOAT, HSESP_GENERIC_FLOAT_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_GENERIC_FLOAT;
		memcpy(buffer + dataCursor, &values[i], sizeof(float));
		dataCursor += sizeof(float);
	}
	return true;
}
bool HseSP::addGenericANSIChar(char value)
{
	return addGenericANSIChar(&value, 1);
}
bool HseSP::addGenericANSIChar(char *values, uint8_t len)
{
	if (!addDataHeader(HSESP_GENERIC_A_N_S_I_CHAR, HSESP_GENERIC_A_N_S_I_CHAR_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_GENERIC_A_N_S_I_CHAR;
		memcpy(buffer + dataCursor, &values[i], sizeof(char));
		dataCursor += sizeof(char);
	}
	return true;
}
bool HseSP::addGenericUTF16Char(wchar_t value)
{
	return addGenericUTF16Char(&value, 1);
}
bool HseSP::addGenericUTF16Char(wchar_t *values, uint8_t len)
{
	if (!addDataHeader(HSESP_GENERIC_U_T_F16_CHAR, HSESP_GENERIC_U_T_F16_CHAR_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_GENERIC_U_T_F16_CHAR;
		memcpy(buffer + dataCursor, &values[i], sizeof(wchar_t));
		dataCursor += sizeof(wchar_t);
	}
	return true;
}
bool HseSP::addGenericBoolean(bool value)
{
	return addGenericBoolean(&value, 1);
}
bool HseSP::addGenericBoolean(bool *values, uint8_t len)
{
	if (!addDataHeader(HSESP_GENERIC_BOOLEAN, HSESP_GENERIC_BOOLEAN_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_GENERIC_BOOLEAN;
		memcpy(buffer + dataCursor, &values[i], sizeof(bool));
		dataCursor += sizeof(bool);
	}
	return true;
}
bool HseSP::addClimateSensor(ClimateSensor_t *values, uint8_t len)
{
	if (!addDataHeader(HSESP_ClimateSensor, HSESP_ClimateSensor_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_ClimateSensor;
		{
			int16_t val = values->Temperature * 100;
			memcpy(buffer + dataCursor, &val, sizeof(val));
			dataCursor += sizeof(val);
		}
		{
			uint8_t val = values->Humidity * 2;
			memcpy(buffer + dataCursor, &val, sizeof(val));
			dataCursor += sizeof(val);
		}
		{
			uint16_t val = values->Pressure * 10;
			memcpy(buffer + dataCursor, &val, sizeof(val));
			dataCursor += sizeof(val);
		}
		{
			memcpy(buffer + dataCursor, &values->Illuminance, sizeof(values->Illuminance));
			dataCursor += sizeof(values->Illuminance);
		}
	}
	return true;
}
bool HseSP::addGPSSensor(GPSSensor_t *values, uint8_t len)
{
	if (!addDataHeader(HSESP_GPSSensor, HSESP_GPSSensor_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_GPSSensor;
		{
			float val = values->Latitude * 1;
			memcpy(buffer + dataCursor, &val, sizeof(val));
			dataCursor += sizeof(val);
		}
		{
			float val = values->Longitude * 1;
			memcpy(buffer + dataCursor, &val, sizeof(val));
			dataCursor += sizeof(val);
		}
		{
			uint8_t val = values->SignalStrength * -1;
			memcpy(buffer + dataCursor, &val, sizeof(val));
			dataCursor += sizeof(val);
		}
	}
	return true;
}
bool HseSP::addPositionSensor(PositionSensor_t *values, uint8_t len)
{
	if (!addDataHeader(HSESP_PositionSensor, HSESP_PositionSensor_SIZE, len))
		return false;
	for (uint8_t i = 0; i < len; i++) {
		if (__HSESP_VERSION == 1 && i > 0)
			buffer[dataCursor++] = HSESP_PositionSensor;
		{
			int16_t val = values->GSensorX * 10;
			memcpy(buffer + dataCursor, &val, sizeof(val));
			dataCursor += sizeof(val);
		}
		{
			int16_t val = values->GSensorY * 10;
			memcpy(buffer + dataCursor, &val, sizeof(val));
			dataCursor += sizeof(val);
		}
		{
			int16_t val = values->GSensorZ * 10;
			memcpy(buffer + dataCursor, &val, sizeof(val));
			dataCursor += sizeof(val);
		}
		{
			memcpy(buffer + dataCursor, &values->Totalofshocks, sizeof(values->Totalofshocks));
			dataCursor += sizeof(values->Totalofshocks);
		}
		{
			memcpy(buffer + dataCursor, &values->Maxshocks, sizeof(values->Maxshocks));
			dataCursor += sizeof(values->Maxshocks);
		}
	}
	return true;
}
