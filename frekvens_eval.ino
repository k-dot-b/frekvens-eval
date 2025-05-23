// FREKVENS LED display project
// Evaluation program

#include "params.h"
#include "shift.h"
#include "demo.h"

//-------------------------------------------
// MACROS

//The starting point of the demo sequence
#define FIRST_ROUTINE 2

//Higher number means less brightness
#define DISPLAY_DIMNESS 240

//-------------------------------------------
// CONSTANTS
  //frame dimensions ROWC and COLC must be defined as macros in params.h

  const int LATCH_PIN = 8;  //Latch, falling edge triggered
  const int OE_PIN = 9;     //Output Enable, ACTIVE LOW

//-------------------------------------------
// GLOBAL VARIABLES

  /**
  * Direct frame data.
  * Pixels are represented by bits. The array can be transmitted directly to the LED drivers.
  */
  uint8_t g_frame[ROWC][COLC];

  /**
  * EXTERNAL VARIABLE
  * Frame bitmap array (from shift.h)
  */
  extern uint8_t g_bitmap[DIMC][DIMC];

  /**
  * Define semaphores here if needed
  */


  //test routine (demo)
  int g_routine = FIRST_ROUTINE;

//-------------------------------------------
// FUNCTION DECLARATIONS

void blank_bitmap(uint8_t (*bitmap)[DIMC], uint8_t dim);

void blank_frame(uint8_t (*frame)[COLC], uint8_t rows, uint8_t cols);

//===========================================

void setup() {
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(OE_PIN, OUTPUT);
  digitalWrite(LATCH_PIN, LOW);
  disableDisplay(OE_PIN);

  SPI.begin();
  Serial.begin(115200);

  blank_bitmap(g_bitmap, DIMC);
  blank_frame(g_frame, ROWC, COLC);

  enableDisplay(OE_PIN, DISPLAY_DIMNESS);
}

//===========================================

void loop() {

#ifdef _DEMO_H_INCLUDED
  //DEMO ROUTINE
  Serial.print("Demo routine ");
  Serial.print(g_routine);
  Serial.println(" running");
  demo(g_routine);
  if (g_routine<DEFINED_ROUTINES)
    g_routine++;
  else
    g_routine=FIRST_ROUTINE;
  blank_bitmap(g_bitmap, DIMC);
  blank_frame(g_frame, ROWC, COLC);
  mrefresh(g_bitmap, DIMC, 8, LATCH_PIN, OE_PIN);
  Serial.println("Demo routine finished");
  //END DEMO ROUTINE
#endif

}

//-------------------------------------------
// FUNCTION DEFINITIONS

void blank_bitmap(uint8_t (*bitmap)[DIMC], uint8_t dim){
  for(int i=0;i<dim;i++){
    for(int j=0;j<dim;j++){
      bitmap[i][j]=0;
    }
  }
}

void blank_frame(uint8_t (*frame)[COLC], uint8_t rows, uint8_t cols){
  for(int i=0;i<rows;i++){
    for(int j=0;j<cols;j++){
      frame[i][j]=0;
    }
  }
}
