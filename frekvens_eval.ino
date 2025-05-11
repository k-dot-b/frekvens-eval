

#include "params.h"
#include "utils.h"
#include "shift.h"

//-------------------------------------------
// MACROS
#define LASR 8  //latch enable pin, ACTIVE LOW
#define OESR 9  //output enable pin, ACTIVE LOW, PWM capable
//-------------------------------------------
// CONSTANTS

//-------------------------------------------
// GLOBAL VARIABLES
  //frame dimensions defined as macros in params.h
  uint8_t frame[ROWC][COLC];

  //for test routine
  int cluster=0;
//-------------------------------------------
// FUNCTION DECLARATIONS
void fgen_cluster_stepper(uint8_t frm[ROWC][COLC], int px);

//-------------------------------------------

//===========================================
void setup() {
  digitalWrite(OESR, HIGH);
  SPI.begin();
  Serial.begin(115200);

  //reset frame
  for(int i=0;i<ROWC;i++){
    for(int j=0;j<COLC;j++){
      frame[i][j]=0;
    }
  }

  digitalWrite(OESR, LOW);
}

//===========================================
void loop() {
  
  //test routine: cluster stepper
  Serial.println("Frame:");
  fgen_cluster_stepper(frame, cluster);
  //digitalWrite(OESR, HIGH);
  refresh(frame, LASR, OESR);
  //digitalWrite(OESR, LOW);
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
