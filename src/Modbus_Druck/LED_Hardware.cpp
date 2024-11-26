

// ACO LoRa hat nur LED1 an D13

# include <arduino.h>
# include "Platine.h"
# include "LED_Hardware.h"


void SetLed(int iColor)
{

 
if ( iColor > 0 )
   {
   pinMode(LED1,OUTPUT);     
   digitalWrite(LED1, LOW); 
   }
else   
   digitalWrite(LED1, HIGH); 
}
