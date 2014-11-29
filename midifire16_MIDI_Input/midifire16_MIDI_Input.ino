// midifire16 control code
// Leif Bloomquist, November 29, 2014

// midifire16 appears as an Arduino Duemilanove w/ ATmega328!

#include <MIDI.h>
#include <Tlc5940.h>
#include <elapsedMillis.h>

#define MY_CHANNEL 1     // MIDI Channel (starting from 1)
#define PWM_MAX 4095
#define VEL_MAX 127

#define TIMEOUT 4000    // milliseconds

#define NUM_OUTPUTS 16

elapsedMillis timeElapsed[NUM_OUTPUTS];


// -----------------------------------------------------------------------------
void HandleNoteOn(byte channel, byte note, byte velocity)
{     
    // Filter on our channel
    if (channel != MY_CHANNEL) return;
    if (note >= NUM_OUTPUTS) return;
    
    if (velocity == 0)
    {
        // This acts like a NoteOff.
        TurnOutputOff(note);        
        return;
    }
    
    // Restart the timer for this output
    timeElapsed[note]=0;
    
    int pwm = VelocityToPWM(velocity);
    Tlc.set(note,pwm);
    Tlc.update();
}

// -----------------------------------------------------------------------------
void HandleNoteOff(byte channel, byte note, byte velocity)
{
    // Filter on our channel
    if (channel != MY_CHANNEL) return;
    if (note >= NUM_OUTPUTS) return;
    
    TurnOutputOff(note);
}

// -----------------------------------------------------------------------------
void TurnOutputOff(byte num)
{
  if (num >= NUM_OUTPUTS) return;
  
   Tlc.set(num,0);
   Tlc.update(); 
}

// -----------------------------------------------------------------------------
void setup() 
{
  Tlc.init();
  Tlc.clear();

  // Explicitly turn off active flames and reset timers
  for (byte c=0; c<NUM_OUTPUTS; c++) 
  {
      Tlc.set(c,0);
      timeElapsed[c]=0;
  }
  Tlc.update();
 
  // Initiate MIDI communications, listen to all channels
  MIDI.begin(MIDI_CHANNEL_OMNI);    

  // Connect the functions to the library, 
  MIDI.setHandleNoteOn(HandleNoteOn);    // Put only the name of the function
  MIDI.setHandleNoteOff(HandleNoteOff);  
}

 
// main loop
void loop() 
{  
  // Call MIDI.read the fastest you can for real-time performance.
  MIDI.read();
  
  // Check timeouts to prevent stuck outputs.
  CheckTimeouts();
} 


// Scale a MIDI velocity input from 0-127 to 0 to 4095  (PWM) 
int VelocityToPWM(int vel)
{
   // Zero is always zero.
   if (vel == 0)
   {   
      return 0;
   }
   
   // Clamp the upper value too.
   if (vel >= VEL_MAX)
   {   
      return PWM_MAX;
   }   
   
   // And everything in between.
   float ratio = (float)vel/(float)VEL_MAX;
   int pwm = ratio*PWM_MAX;
   
   return pwm;
}

void CheckTimeouts()
{
  for (byte c=0; c<NUM_OUTPUTS; c++) 
  {
    if (timeElapsed[c] > TIMEOUT)
    {
      TurnOutputOff(c);         
    }
  }
} 
