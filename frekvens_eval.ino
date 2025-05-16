// FREKVENS LED display project
// Evaluation program

#include "params.h"
#include "utils.h"
#include "shift.h"

//-------------------------------------------
// MACROS
  //Global macros in params.h
  #define CLUSTER_MAX 31
  #define PIXEL_MAX   255

//-------------------------------------------
// CONSTANTS
  //frame dimensions ROWC and COLC must be defined as macros in params.h

  const int LATCH_PIN = 8;  //Latch, falling edge triggered
  const int OE_PIN = 9;     //Output Enable, ACTIVE LOW

  //Display refresh period
  const int STEP_DELAY_1 = 240;
  const int STEP_DELAY_2 = 30;

//-------------------------------------------
// GLOBAL VARIABLES
  /**
  * Direct frame data
  * Pixels are represented by bits. The array can be transmitted directly to the LED drivers.
  */
  uint8_t g_frame[ROWC][COLC];

  /**
  * Full frame bitmap
  * Defines a byte for each pixel for grayscale data (future use).
  */
  uint8_t g_bitmap[ROWC][ROWC];

  /**
  * Semaphore for map function
  */
  volatile bool sem_map;

  /**
  * Semaphore for refresh function
  * Frame transmission cannot be interrupted.
  */
  volatile bool sem_tx;

  //for test routines
  int g_routine = 1;
  int g_cluster=0;
  uint8_t cluster_data = 0xff;
  int g_pixel=0;

//-------------------------------------------
// FUNCTION DECLARATIONS

void blank_bitmap(uint8_t (*bitmap)[ROWC], uint8_t rows);

void blank_frame(uint8_t (*frame)[COLC], uint8_t rows, uint8_t cols);

/**
* TEST FUNCTION
* Writes the given data to the selected byte, and fills the rest of the array with zeroes
* 
* segment:    The selected byte of the frame array
* data:       Data to be written into the segment
*/
void fgen_cluster_stepper(uint8_t (*frm)[COLC], uint8_t rows, uint8_t cols, int segment, uint8_t data);

/**
* TEST FUNCTION
* Writes 1 to the selected byte, and fills the rest of the array with zeroes
* 
* pixel:      The selected pixel in the bitmap array
*/
void fgen_pixel_stepper(uint8_t (*bitmap)[ROWC], uint8_t rows, int pixel);

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
  
  //test routine 1: cluster stepper
  if (g_routine==1){
    fgen_cluster_stepper(g_frame, ROWC, COLC, g_cluster, cluster_data);

    Serial.println("Cluster step frame:");
    for(int i=0;i<ROWC;i++){
      for(int j=0;j<COLC;j++){
      Serial.print(g_frame[i][j]);
      Serial.print("		");
      }
      Serial.println();
    }

    refresh(g_frame, ROWC, COLC, LATCH_PIN, OE_PIN);
    delay(STEP_DELAY_1);

    if (g_cluster<CLUSTER_MAX)
      g_cluster++;
    else {
      g_cluster=0;
      g_routine=2;
      Serial.println();
      blank_bitmap(g_bitmap, ROWC);
      blank_frame(g_frame, ROWC, COLC);
      delay(1000);
    }
    //end test routine 1
  }
  
  //test routine 2: bit stepper
  if (g_routine==2) {
    fgen_pixel_stepper(g_bitmap, ROWC, g_pixel);
    map(g_bitmap, g_frame, ROWC, COLC);

    Serial.println("Pixel step frame:");
    for(int i=0;i<ROWC;i++){
      for(int j=0;j<COLC;j++){
      Serial.print(g_frame[i][j]);
      Serial.print("		");
      }
      Serial.println();
    }

    refresh(g_frame, ROWC, COLC, LATCH_PIN, OE_PIN);
    delay(STEP_DELAY_2);

    if (g_pixel<PIXEL_MAX)
      g_pixel++;
    else {
      //reset test routines
      g_pixel=0;
      g_routine=1;  //switch routine
      Serial.println();
      blank_bitmap(g_bitmap, ROWC);
      blank_frame(g_frame, ROWC, COLC);
      delay(1000);
    }
    //end test routine 2
  }

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

void fgen_cluster_stepper(uint8_t (*frm)[COLC], uint8_t rows, uint8_t cols, int segment, uint8_t data){
  if (!(frm&&*frm))
		return;

  int cnt=0;
  for(int i=0;i<rows;i++){
    for(int j=0;j<cols;j++){
      if (cnt==segment){
        frm[i][j]=data;
      }
      else
        frm[i][j]=0;
      if (cnt<CLUSTER_MAX)
        cnt++;
      else
        cnt=0;
    }
  }
}

void fgen_pixel_stepper(uint8_t (*bitmap)[ROWC], uint8_t rows, int pixel){
  if (!(bitmap&&*bitmap))
		return;

  uint8_t cols = rows;
  int cnt=0;
  for(int i=0;i<rows;i++){
    for(int j=0;j<cols;j++){
      if (cnt==pixel){
        bitmap[i][j]=1;
      }
      else
        bitmap[i][j]=0;
      if (cnt<PIXEL_MAX)
        cnt++;
      else
        cnt=0;
    }
  }
}
