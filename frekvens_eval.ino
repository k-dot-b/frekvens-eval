// FREKVENS LED display project
// Evaluation program

#include "params.h"
#include "utils.h"
#include "shift.h"

//-------------------------------------------
// MACROS

//-------------------------------------------
// CONSTANTS
  const int LATCH_PIN = 8;  //Latch, falling edge
  const int OE_PIN = 9;     //Output Enable, ACTIVE LOW
//-------------------------------------------
// GLOBAL VARIABLES
  //frame dimensions ROWC and COLC must be defined as macros in params.h
  uint8_t g_frame[ROWC][COLC];

  //for test routine
  int cluster=0;
//-------------------------------------------
// FUNCTION DECLARATIONS
void fgen_cluster_stepper(uint8_t frm[ROWC][COLC], int px);

//-------------------------------------------

//===========================================
void setup() {
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(OE_PIN, OUTPUT);
  digitalWrite(LATCH_PIN, LOW);
  digitalWrite(OE_PIN, HIGH);

  SPI.begin();
  Serial.begin(115200);

  //reset frame
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
  Serial.println("Frame:");
  fgen_cluster_stepper(g_frame, cluster);
  refresh(g_frame, LATCH_PIN, OE_PIN);
  if (cluster<32)
    cluster++;
  else
    cluster=0;
  delay(500);
  //end test routine
}

//-------------------------------------------
// FUNCTION DEFINITIONS
void fgen_cluster_stepper(uint8_t frm[ROWC][COLC], int px){
  if (!(frm&&*frm))
		return;

  int cnt=0;
  for(int i=0;i<ROWC;i++){
    for(int j=0;j<COLC;j++){
      if (cnt==px){
        frm[i][j]=0xff;
      }
      else
        frm[i][j]=0;
      if (cnt<32)
        cnt++;
      else
        cnt=0;
    }
  }
}
