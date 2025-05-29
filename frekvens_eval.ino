// FREKVENS LED display project
// Evaluation program

#include "shift.h"
//#include "demo.h"

//-------------------------------------------
// MACROS

#define DISPLAY_DIMNESS 128

#define FADE_PRESCALER 6
#define FADE_DISPLAY_DEMO

//-------------------------------------------
// CONSTANTS

  const int LATCH_PIN = 8;  //Latch, falling edge triggered
  const int OE_PIN = 9;     //Output Enable, ACTIVE LOW

//-------------------------------------------
// GLOBAL VARIABLES

  /**
  * EXTERNAL VARIABLE
  * Frame bitmap array (from shift.h)
  */
  extern uint8_t g_bitmap[DIMC][DIMC];

  volatile uint8_t gray = 0;
  volatile int fade_cntr = 0;
  volatile bool fade_reverse = false;

  int ri, rj;

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

  //FrekvensEnableDisplayDimming(DISPLAY_DIMNESS);
  FrekvensEnableDisplayGrayscale();
  enableInterruptOC1A();

  randomSeed(analogRead(A2));
}

//===========================================

void loop() {
  //DEMO ROUTINE
  #ifdef _DEMO_H_INCLUDED
    #ifndef DEMO_INTERRUPT
      demo(0);
      multiDemo();
    #endif //DEMO_INTERRUPT
  #endif //_DEMO_H_INCLUDED
  //END DEMO ROUTINE


  if (flag_frekvens_activity){
    //This segment triggers after each frame
    
    //Fill bitmap with 'gray' value
    for (int i=0;i<DIMC;i++){
      for (int j=0;j<DIMC;j++){
        if (i==ri && j==rj)
          FrekvensLoadPixel(i, j, gray);
        else
          FrekvensLoadPixel(i, j, 0);
      }
    }

    #ifdef FADE_DISPLAY_DEMO
    if (fade_cntr<FADE_PRESCALER){
      fade_cntr++;
    }
    else {
      fade_cntr = 0;
      //generate pulsating 'gray' value
      if (!fade_reverse){
        if (gray<15){
          gray++;
        }
        else
          fade_reverse = true;
      }
      else{
        if (gray>1){  //never fade to black entirely
          gray--;
        }
        else {
          fade_reverse = false;
          ri = random(0, 15);
          rj = random(0, 15);
        }
      }
    }
    #endif

    flag_frekvens_activity=false;
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
  #if defined(_DEMO_H_INCLUDED) && defined(DEMO_INTERRUPT)
  demoInterrupt();
  return;
  #endif

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
      flag_frekvens_activity = true;                        //signal frame completion
    }
  }
}
#endif //__AVR_ATmega328P__
