
void Set_I2C_Adresse(uint8_t I2C_Adr);

uint8_t I2C_read8(byte reg);
void I2C_write8(byte reg, byte value);

uint16_t I2C_read16(byte reg);
int16_t I2C_readS16(byte reg);
int16_t I2C_readS16_LE(byte reg);
int16_t I2C_readS16_LE(byte reg);
uint16_t I2C_read16_LE(byte reg) ;
uint32_t I2C_Read24(byte reg);

void I2C_write8A(byte value);
uint16_t I2C_read16A(void);
uint8_t I2C_Test(byte adr);

extern uint8_t _i2caddr;


