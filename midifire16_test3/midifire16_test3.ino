// This example tests MIDI Out.
#include <Tlc5940.h>

// setup
void setup() 
{
  Tlc.init();
  Tlc.clear();
    
  // Explicitly turn off active flames
  for(int c=0; c<16 ;c++) 
  {
      Tlc.set(c,0);
  }
  Tlc.update();
    
  Serial.begin(31250);
}

// main loop
void loop() 
{  
   // Send some midi data.
   Serial.write(0x90);  // Note On
   Serial.write(0x3c);  // Middle C
   Serial.write(0x7f);  // Max velocity  
   Tlc.set(0,4095);
   Tlc.update();
   delay(500);
   
   Serial.write(0x80);  // Note Off
   Serial.write(0x3c);  // Middle C
   Serial.write(0x7f);  // Max velocity  
   Tlc.set(0,0);
   Tlc.update();
   delay(500);
}
