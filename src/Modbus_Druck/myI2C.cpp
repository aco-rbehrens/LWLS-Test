#include "Arduino.h"
#include <Wire.h> 

uint8_t _i2caddr;

//-----------------------------------
void Set_I2C_Adresse(uint8_t I2C_Adr)
{
 _i2caddr = I2C_Adr; 
 //Wire.setClock(10000);
}


//--------------------------------------------------
uint8_t I2C_read8(byte reg)
{
  uint8_t value;
  
    Wire.beginTransmission((uint8_t)_i2caddr);
    Wire.write((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom((uint8_t)_i2caddr, (byte)1);
    value = Wire.read();

  return value;
} 
//----------------------------------------------
void I2C_write8(byte reg, byte value)
{
    Wire.beginTransmission((uint8_t)_i2caddr);
    Wire.write((uint8_t)reg);
    Wire.write((uint8_t)value);
    Wire.endTransmission();
} 
//---------------------------------------------

// 8bit ohne Register schreiben
void I2C_write8A(byte value)
{
    Wire.beginTransmission((uint8_t)_i2caddr);    
    Wire.write((uint8_t)value);
    Wire.endTransmission();
} 
//---------------------------------------------
uint16_t I2C_read16(byte reg)
{
  uint16_t value;
 
    Wire.beginTransmission((uint8_t)_i2caddr);
    Wire.write((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom((uint8_t)_i2caddr, (byte)2);
    value = (Wire.read() << 8) | Wire.read();

  return value;
}
//------------------------------------------
// 16 bit lesen ohne Register
uint16_t I2C_read16A(void)
{
  uint16_t value;
 
    Wire.beginTransmission((uint8_t)_i2caddr);
    Wire.requestFrom((uint8_t)_i2caddr, (byte)2);
    value = (Wire.read() << 8) | Wire.read();

  return value;
}
//------------------------------------------

int16_t I2C_readS16(byte reg)
{
  return (int16_t)I2C_read16(reg);
}
//---------------------------------------------
uint16_t I2C_read16_LE(byte reg) 
{
  uint16_t temp = I2C_read16(reg);
  return (temp >> 8) | (temp << 8);

} 
//------------------------------------------
int16_t I2C_readS16_LE(byte reg)
{
  return (int16_t)I2C_read16_LE(reg);

}  
//----------------------------------------------
uint32_t I2C_Read24(byte reg)
{
  uint32_t value;

    Wire.beginTransmission((uint8_t)_i2caddr);
    Wire.write((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom((uint8_t)_i2caddr, (byte)3);
    
    value = Wire.read();
    value <<= 8;
    value |= Wire.read();
    value <<= 8;
    value |= Wire.read();

  return value;
} 
//--------------------------
uint8_t I2C_Test(byte adr)
{
  uint8_t value;
  
    Wire.beginTransmission((uint8_t)adr);
    Wire.write((uint8_t)0);
    Wire.endTransmission();
    Wire.requestFrom((uint8_t)adr, (byte)1);
    value = Wire.read();

  return value;
} 
//----------------------------------------------
