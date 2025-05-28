// FREKVENS LED display project
// Evaluation program

#include "shift.h"
//#include "demo.h"

//-------------------------------------------
// MACROS

//Higher number means less brightness
#define DISPLAY_DIMNESS 0

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

  uint8_t iter_cntr_init = 0;
  uint8_t iter_cntr = 0;
  uint8_t subframe_cntr = 0;
  uint8_t gray = 7;
  bool fade_reverse = false;

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
  * 1   Hz    | 1-0-1     | 15623
  */
  cli();  //Clear interrupts
  TCCR1A = 0; //Timer-Counter Control Register 1A
  TCCR1B = 0;
  TCNT1 = 0;  //initialize counter value to 0
  TCCR1B |= (1<<WGM12);
  TCCR1B |= (1<<CS12) | (0<<CS11) | (1<<CS10);
  OCR1A = 15623; //Output compare register value
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


  frekvens_bitmask_index = 3;
  for (int i=0;i<FREKVENS_GRAYSCALE_BIT_DEPTH;i++){
    //calculate 2^(bit_depth)-1 which will be the number of required subframes for BCM
    iter_cntr_init |= 1<<i;
  }
  iter_cntr = iter_cntr_init;

  for (int i=0;i<DIMC;i++){
    for (int j=0;j<DIMC;j++){
      FrekvensLoadPixel(i, j, gray);  //load current gray value to whole bitmap
    }
  }
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

  /*
  if (flag_frekvens_activity){
    for (int i=0;i<DIMC;i++){
      for (int j=0;j<DIMC;j++){
        FrekvensLoadPixel(i, j, gray);  //load current gray value to whole bitmap
      }
    }
    
    if (!fade_reverse){
      if (gray<255)
        gray++;
      else
        fade_reverse = true;
    }
    else{
      if (gray>0)
        gray--;
      else
        fade_reverse = false;
    }

    TIMSK1 |= (0<<OCIE1A);  //Disable output compare interrupt
    iter_cntr = iter_cntr_init;   //this is super sketchy code, probably better to do this straight in the interrupt
    frekvens_bitmask_index = FREKVENS_GRAYSCALE_BIT_DEPTH - 1;
    TIMSK1 |= (1<<OCIE1A);  //Enable output compare interrupt
    flag_frekvens_activity = false;
  }
  */
  if (flag_frekvens_activity){
    Serial.println(subframe_cntr);
    flag_frekvens_activity=false;
  }
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

  if (iter_cntr && frekvens_bitmask[frekvens_bitmask_index]){
      iter_cntr--;
  }
  else {
    if (iter_cntr)
      iter_cntr--;
    else
      iter_cntr = iter_cntr_init;

    if (frekvens_bitmask_index)
      frekvens_bitmask_index--;
    else
      frekvens_bitmask_index = 3; //reset to initial value
  }
  FrekvensRefreshDisplay();

  if (subframe_cntr < iter_cntr_init)
    subframe_cntr++;
  else {
    subframe_cntr = 0;
    //flag_frekvens_activity = true;  //signal frame completion
  }
  flag_frekvens_activity = true;  //signal frame completion
}