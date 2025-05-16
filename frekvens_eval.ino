// FREKVENS LED display project
// Evaluation program

#include "params.h"
#include "utils.h"
#include "shift.h"

//-------------------------------------------
// MACROS
  //Global macros in params.h

//-------------------------------------------
// CONSTANTS
  //frame dimensions ROWC and COLC must be defined as macros in params.h

  const int LATCH_PIN = 8;  //Latch, falling edge triggered
  const int OE_PIN = 9;     //Output Enable, ACTIVE LOW

  //Display refresh period
  const int STEP_DELAY = 200;

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

  //for test routine
  int cluster=0;
  uint8_t cluster_data = 0xff;

//-------------------------------------------
// FUNCTION DECLARATIONS
/**
* TEST FUNCTION
* Writes data into the selected byte, and fills the rest of the array with zeroes
* 
* segment:    The selected byte of the Direct data frame
* data:       Data to be written into the segment
*/
void fgen_cluster_stepper(uint8_t (*frm)[COLC], uint8_t rows, uint8_t cols, int segment, uint8_t data);



//===========================================
void setup() {
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(OE_PIN, OUTPUT);
  digitalWrite(LATCH_PIN, LOW);
  digitalWrite(OE_PIN, HIGH);

  SPI.begin();
  Serial.begin(115200);

  //reset frame with zeros
  for(int i=0;i<ROWC;i++){
    for(int j=0;j<COLC;j++){
      g_frame[i][j]=0;
    }
  }

  digitalWrite(OE_PIN, LOW);
}

//===========================================
void loop() {
  
  //test routine: cluster stepper
  fgen_cluster_stepper(g_frame, ROWC, COLC, cluster, cluster_data);
  if (cluster<31)
    cluster++;
  else
    cluster=0;

  Serial.println("Frame:");
	for(int i=0;i<ROWC;i++){
		for(int j=0;j<COLC;j++){
		Serial.print(g_frame[i][j]);
		Serial.print("		");
		}
		Serial.println();
	}

  refresh(g_frame, ROWC, COLC, LATCH_PIN, OE_PIN);

  delay(STEP_DELAY);
  //end test routine
}

//-------------------------------------------
// FUNCTION DEFINITIONS
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
      if (cnt<31)
        cnt++;
      else
        cnt=0;
    }
  }
}
