// FREKVENS LED display project
// Evaluation program

#include "params.h"
#include "utils.h"
#include "shift.h"
#include "demo.h"

//-------------------------------------------
// MACROS
  //Global macros in params.h

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
  * Full frame bitmap.
  * Defines a byte for each pixel.
  */
  uint8_t g_bitmap[ROWC][ROWC];

  /**
  * Semaphore for map function
  */
  volatile bool sem_map;

  /**
  * Semaphore for refresh function
  */
  volatile bool sem_tx;

  //test routine (demo)
  int g_routine = 1;

//-------------------------------------------
// FUNCTION DECLARATIONS
void blank_bitmap(uint8_t (*bitmap)[ROWC], uint8_t rows);

void blank_frame(uint8_t (*frame)[COLC], uint8_t rows, uint8_t cols);

//===========================================
void setup() {
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(OE_PIN, OUTPUT);
  digitalWrite(LATCH_PIN, LOW);
  digitalWrite(OE_PIN, HIGH);

  SPI.begin();
  Serial.begin(115200);

  blank_bitmap(g_bitmap, ROWC);
  blank_frame(g_frame, ROWC, COLC);

  digitalWrite(OE_PIN, LOW);  //Enable display
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
    g_routine=1;
  blank_bitmap(g_bitmap, ROWC);
  blank_frame(g_frame, ROWC, COLC);
  refresh(g_frame, ROWC, COLC, LATCH_PIN, OE_PIN);
  Serial.println("Demo routine finished");
  //END DEMO ROUTINE
#endif

}

//-------------------------------------------
// FUNCTION DEFINITIONS
void blank_bitmap(uint8_t (*bitmap)[ROWC], uint8_t rows){
  uint8_t cols = rows;
  for(int i=0;i<rows;i++){
    for(int j=0;j<cols;j++){
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
