// FREKVENS LED display project
// Evaluation program

#include "params.h"
#include "shift.h"
#include "demo.h"

//-------------------------------------------
// MACROS

//Higher number means less brightness
#define DISPLAY_DIMNESS 0

//-------------------------------------------
// CONSTANTS
  //frame dimensions ROWC and COLC must be defined as macros in params.h

  const int LATCH_PIN = 8;  //Latch, falling edge triggered
  const int OE_PIN = 9;     //Output Enable, ACTIVE LOW

//-------------------------------------------
// GLOBAL VARIABLES

  /**
  * EXTERNAL VARIABLE
  * Frame bitmap array (from shift.h)
  */
  extern uint8_t g_bitmap[DIMC][DIMC];

//-------------------------------------------
// FUNCTION DECLARATIONS



//===========================================

void setup() {
  //-----------------------------------------------------------
  /*
  * HARDWARE SPECIFIC CODE
  * Arduino Uno timer1 configuration
  * 
  * Current frequency: 400 Hz
  * frequency | CSx2-1-0  | OCR
  * 400 Hz    | 0-1-0     | 2499
  */
  cli();  //Clear interrupts
  TCCR1A = 0; //Timer-Counter Control Register 1A
  TCCR1B = 0;
  TCNT1 = 0;  //initialize counter value to 0
  TCCR1B |= (1<<WGM12);
  TCCR1B |= (0<<CS12) | (1<<CS11) | (0<<CS10);
  OCR1A = 2499; //Output compare register value
  TIMSK1 |= (1<<OCIE1A);  //Enable output compare interrupt
  sei();  //Enable interrupts
  //End timer configuration
  //-----------------------------------------------------------

  Serial.begin(115200);

  if (!FrekvensAttachDisplay(LATCH_PIN, OE_PIN)){
    Serial.println("Display initialization failed!");
    Serial.println("Incorrect pin definitions");
    while(1){}
  }

  FrekvensDisableDisplay();

  SPI.begin();

  FrekvensEnableDisplayDimming(DISPLAY_DIMNESS);
}

//===========================================

void loop() {
  //DEMO ROUTINE
  #ifdef _DEMO_H_INCLUDED
    #ifndef DEMO_INTERRUPT
      demo(g_routine);
      if (g_routine<DEFINED_ROUTINES)
        g_routine++;
      else
        g_routine=FIRST_ROUTINE;
      return; //Break loop here
    #endif //DEMO_INTERRUPT
  #endif //_DEMO_H_INCLUDED
  //END DEMO ROUTINE


}

//-------------------------------------------
// FUNCTION DEFINITIONS



//-------------------------------------------
// INTERRUPT SERVICE ROUTINES

ISR(TIMER1_COMPA_vect){
  #if defined(_DEMO_H_INCLUDED) && defined(DEMO_INTERRUPT)
  demoInterrupt();
  return;
  #endif
}