#ifndef h_CSPPM
#define h_CSPPM


#ifndef RS485_DIR
 #define RS485_DIR 17
#endif 


int init_CSPPM(void);
void CSPPM_PowerDown(void);

uint8_t ReadPressureAndTemp(void);

float CSPPM_GetDruck(void);
float CSPPM_GetTemperatur(void);




#endif
