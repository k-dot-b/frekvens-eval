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

  uint8_t gray = 8;
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
  OCR1A = 3906; //Output compare register value
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


  for (int i=1;i<FREKVENS_GRAYSCALE_BIT_DEPTH;i++){
    //calculate 2^(bit_depth)-1 which will be the number of required subframes for BCM
    FrekvensBCM.iter_max |= 1<<i;
  }
  FrekvensBCM.iter_index = FrekvensBCM.iter_max;
  FrekvensBCM.bitmask_max = FREKVENS_GRAYSCALE_BIT_DEPTH - 1;
  FrekvensBCM.bitmask_index= FrekvensBCM.bitmask_max;
  flag_frekvens_activity=true;  //This is required to compute a frame immediately after program start (mom, the compiler is optimizing out useful code again!)

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

    flag_frekvens_activity = false;
  }
  */

  if (flag_frekvens_activity){
    //This segment triggers after each frame
    
    //Fill bitmap with 'gray' value
    for (int i=0;i<DIMC;i++){
      for (int j=0;j<DIMC;j++){
        FrekvensLoadPixel(i, j, gray);
      }
    }

    /*
    //generate pulsating 'gray' value
    Serial.print("frame: ");
    if (!fade_reverse){
      if (gray<15){
        gray++;
        Serial.println(gray);
      }
      else
        fade_reverse = true;
    }
    else{
      if (gray>0){
        gray--;
        Serial.println(gray);
      }
      else
        fade_reverse = false;
    }
    */

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

  Serial.print(FrekvensBCM.iter_index);
  Serial.print("  ");
  Serial.println(FrekvensBCM.bitmask_index);
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
      FrekvensBCM.iter_index = FrekvensBCM.iter_max;  //reload counter
      FrekvensBCM.bitmask_index = 3;                  //reload bitmask
      flag_frekvens_activity = true;                  //signal frame completion
    }
  }
}
