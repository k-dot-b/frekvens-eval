// FREKVENS LED display project
// Evaluation program

#include "frekvens_driver.h"
#include "demo.h"

//-------------------------------------------
// MACROS



//-------------------------------------------
// CONSTANTS

  const int LATCH_PIN = 8;  //Latch, falling edge triggered
  const int OE_PIN = 9;     //Output Enable, ACTIVE LOW

//-------------------------------------------
// GLOBAL VARIABLES



//-------------------------------------------
// FUNCTION DECLARATIONS

#if defined (__AVR_ATmega328P__)
static inline void disableInterruptOC1A();
static inline void enableInterruptOC1A();
#endif //__AVR_ATmega328P__

//===========================================

void setup() {
  //-----------------------------------------------------------
  #if defined (__AVR_ATmega328P__)
  /*
  * HARDWARE SPECIFIC CODE
  * Arduino Uno timer1 configuration
  * 
  * Current frequency: 1600 Hz
  * frequency | CSx2-1-0  | OCR
  * 1600 Hz   | 0-1-0     | 1249    BEST EXPERIENCE
  * 1200 Hz   | 0-1-0     | 1666
  * 1000 Hz   | 0-1-0     | 1999
  * 800 Hz    | 0-1-0     | 2499
  * 1   Hz    | 1-0-1     | 15623
  */
  cli();  //Clear interrupts
  TCCR1A = 0; //Timer-Counter Control Register 1A
  TCCR1B = 0;
  TCNT1 = 0;  //initialize counter value to 0
  TCCR1B |= (1<<WGM12);
  TCCR1B |= (0<<CS12) | (1<<CS11) | (0<<CS10);
  OCR1A = 1249; // 1600Hz subframe refresh frequency
  OCR1B = 1999; // 1000Hz / 1ms intervals, millis() substitute
  //Disable interrupts by default:
  TIMSK1 = (0<<ICIE1) | (0<<OCIE1B) | (0<<OCIE1A) | (0<<TOIE1);
  sei();  //Enable interrupts
  //End timer configuration
  #endif //AVR_ATmega328P
  //-----------------------------------------------------------

  Serial.begin(115200);

  if (!FrekvensAttachDisplay(LATCH_PIN, OE_PIN)){
    Serial.println("Display initialization failed!");
    Serial.println("Incorrect pin definitions");
    while(1){}
  }
  FrekvensDisableDisplay();

  SPI.begin();

  //FrekvensEnableDisplayDimming(0);
  
  FrekvensEnableDisplayGrayscale();
  enableInterruptOC1A();

}

//===========================================

void loop() {

  if (frekvens_vsync_ready){
    //This segment will run after each complete frame draw
    //Update the frame buffer here to prevent screen tear
    demoGrayscale();

    frekvens_vsync_ready = false;
  }


}

//-------------------------------------------
// FUNCTION DEFINITIONS

#if defined (__AVR_ATmega328P__)
static inline void disableInterruptOC1A(){
  cli();
  TIMSK1  &= ~(1<<OCIE1A);
  TIFR1   |= (1<<OCF1A);
  sei();
}
static inline void enableInterruptOC1A(){
  cli();
  TIMSK1  |= (1<<OCIE1A);
  TIFR1   |= (1<<OCF1A);
  sei();
}
#endif //__AVR_ATmega328P__

//-------------------------------------------
// INTERRUPT SERVICE ROUTINES

#if defined (__AVR_ATmega328P__)
ISR(TIMER1_COMPA_vect){

  FrekvensRefreshDisplay();

  if ((FrekvensBCM.iter_index & FrekvensBCM.bitmask[FrekvensBCM.bitmask_index])){
      FrekvensBCM.iter_index--;
  }
  else {
    if (FrekvensBCM.iter_index){
      FrekvensBCM.iter_index--;
      FrekvensBCM.bitmask_index--;
    }
    else {
      FrekvensBCM.iter_index = FrekvensBCM.iter_max;        //reload counter
      FrekvensBCM.bitmask_index = FrekvensBCM.bitmask_max;  //reload bitmask
      frekvens_vsync_ready = true;                        //signal frame completion
    }
  }
}
#endif //__AVR_ATmega328P__
