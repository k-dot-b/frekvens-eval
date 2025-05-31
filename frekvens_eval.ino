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



//===========================================

void setup() {
  Serial.begin(115200);

  if (!FrekvensAttachDisplay(LATCH_PIN, OE_PIN)){
    Serial.println("Display initialization failed!");
    Serial.println("Incorrect pin definitions");
    while(1){}
  }
  FrekvensEnableDisplayGrayscale();

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



//-------------------------------------------
// INTERRUPT SERVICE ROUTINES


