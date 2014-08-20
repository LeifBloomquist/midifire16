//#include <MIDI.h>
//#include <midi_Defs.h>
//#include <midi_Namespace.h>
//#include <midi_Settings.h>

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
  
  // Little pattern to show we're alive
  
  for (int c=0; c<16; c++) 
  { 
     Tlc.set(c,4095);
     Tlc.update();
     delay(50);
     
     Tlc.set(c,0);
     Tlc.update();
     delay(20);
  }
    
  Serial.begin(31250);
}


// main loop
void loop() 
{  
    // Call MIDI.read the fastest you can for real-time performance.
    //MIDI.read();
//
//   // Send some midi data.
//   Serial.write(0x90);  // Note On
//   Serial.write(0x3c);  // Middle C
//   Serial.write(0x7f);  // Max velocity  
//   Tlc.set(0,4095);
//   Tlc.update();
//   delay(500);
//   
//   Serial.write(0x80);  // Note Off
//   Serial.write(0x3c);  // Middle C
//   Serial.write(0x7f);  // Max velocity  
//   Tlc.set(0,0);
//   Tlc.update();
//   delay(500);


  if(Serial.available() >= 1) 
  {
    //int n = Serial.read();
    Tlc.set(0,4095);
    Tlc.update();
    delay(5);
    
    int n = Serial.read();
  }
  else
  {
    Tlc.set(0,0);
    Tlc.update();
  }
} 

