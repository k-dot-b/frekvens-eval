/*
 * Copyright 2025 Benedek Kiss
 * Licensed under the EUPL
 *
 * demo.cpp
 */

#include "demo.h"

uint8_t id = 0;

//Demo routine counter
int g_routine = FIRST_ROUTINE;

fadeObject demo_fade_image {0, 6, 0, false};

/**
* DEPRECATED ARRAY
* Frame bitmap array
*/
uint8_t g_bitmap[FREKVENS_DIMC][FREKVENS_DIMC];

/**
* DEPRECATED ARRAY
* Direct frame data.
* Pixels are represented by bits. The array can be transmitted directly to the LED drivers.
*/
uint8_t g_frame[FREKVENS_DIMC][FREKVENS_COLB];

//DEPRECATED FUNCTION FOR LEGACY DEMO ROUTINE (cluster stepper)
static void refresh(uint8_t (*frame)[FREKVENS_COLB], uint8_t rows, uint8_t cols, uint8_t latch, uint8_t enable){
	if (!(frame&&*frame))
		return;
	
  digitalWrite(enable, HIGH);	//blank display

  SPI.beginTransaction(SPISettings(125000, LSBFIRST, SPI_MODE0));
	//Last value first
  //SRORDER: LSBFIRST
	for(int j=cols;j>0;j--){
		for(int i=rows;i>0;i--){
    int received = SPI.transfer(frame[i-1][j-1]);
		}
	}
  SPI.endTransaction();

  digitalWrite(latch, HIGH);	//latch new values
  _NOP();
  digitalWrite(latch, LOW);

  digitalWrite(enable, LOW);	//enable display
}

//DEPRECATED TEST FUNCTION FOR LEGACY DEMO ROUTINE (cluster stepper)
static void fgen_cluster_picker(uint8_t (*frame)[FREKVENS_COLB], uint8_t rows, uint8_t cols, int segment, uint8_t data){
  if (!(frame&&*frame))
		return;

  int cnt=0;
  for(int i=0;i<rows;i++){
    for(int j=0;j<cols;j++){
      if (cnt==segment){
        frame[i][j]=data;
      }
      else
        frame[i][j]=0;
      cnt++;
    }
  }
}

void demo(uint8_t routine){
  if (routine<1 || routine>DEFINED_ROUTINES)
    return;
  Serial.print("Demo routine ");
  Serial.print(routine);
  Serial.println(" running");

  //--------------------------------------------------------------------
  // WRITE DEMO ROUTINES HERE AS CASES:
  switch (routine){
    //DEPRECATED test routine 1: cluster stepper - sequential
    case 1:
      for (int d=0;d<CLUSTER_MAX;d++){
        fgen_cluster_picker(g_frame, FREKVENS_DIMC, FREKVENS_COLB, d, CLUSTER_DATA);

        #ifdef VERBOSE_DEMO
        Serial.println("Cluster step frame:");
        for(int i=0;i<FREKVENS_DIMC;i++){
          for(int j=0;j<FREKVENS_COLB;j++){
          Serial.print(g_frame[i][j]);
          Serial.print("		");
          }
          Serial.println();
        }
        #endif

        refresh(g_frame, FREKVENS_DIMC, FREKVENS_COLB, LATCH_PIN, OE_PIN);
        delay(STEP_DELAY_1);
      }
      break;
      //end test routine 1
    
    //DEPRECATED test routine 2: mrefresh, pixel chase
    case 2:
      for (int d=0;d<PIXEL_MAX;d++){
        fgen_pixel_picker(g_bitmap, FREKVENS_DIMC, d);
        mrefresh(g_bitmap, FREKVENS_DIMC, 8, LATCH_PIN, OE_PIN);
        delay(STEP_DELAY_2);
      }
      break;
      //end test routine 2
    
    //test routine 3: mrefresh2, pixel chase -         left
    case 3:
      for (int d=PIXEL_MAX-1;d>=0;d--){
        fgen_pixel_picker(g_bitmap, FREKVENS_DIMC, d);
        mrefresh2(g_bitmap, FREKVENS_DIMC, 8);
        delay(STEP_DELAY_2);
      }
      break;
      //end test routine 3
    
    //test routine 4: new SPI algorithm, pixel chase - right
    case 4:
      for (int d=0;d<PIXEL_MAX;d++){
        fgen_pixel_picker(g_bitmap, FREKVENS_DIMC, d);
        FrekvensLoadBuffer(g_bitmap, FREKVENS_DIMC);
        FrekvensRefreshDisplay();
        delay(STEP_DELAY_2);
      }
      break;
      //end test routine 4

    //test routine 5: LoadPixel, pixel chase -         down
    case 5:
      for (int d=0;d<PIXEL_MAX;d++){
        int cnt = 0;
        for (uint8_t j=0;j<FREKVENS_DIMC;j++){
          for (uint8_t i=0;i<FREKVENS_DIMC;i++){
            if (cnt==d)
              FrekvensLoadPixel(i, j, 255);
            else
              FrekvensLoadPixel(i, j, 0);
            cnt++;
          }
        }
        FrekvensRefreshDisplay();
        delay(STEP_DELAY_2);
      }
      break;
      //end test routine 5

    //test routine 6: pixel chase -                    up
    case 6:
      for (int d=PIXEL_MAX-1;d>=0;d--){
        int cnt = 0;
        for (uint8_t j=0;j<FREKVENS_DIMC;j++){
          for (uint8_t i=0;i<FREKVENS_DIMC;i++){
            if (cnt==d)
              FrekvensLoadPixel(i, j, 255);
            else
              FrekvensLoadPixel(i, j, 0);
            cnt++;
          }
        }
        FrekvensRefreshDisplay();
        delay(STEP_DELAY_2);
      }
      break;
      //end test routine 6
    
    default:
      break;
  }

  //Clear display
  fgen_pixel_picker(g_bitmap, FREKVENS_DIMC, 404);
  mrefresh2(g_bitmap, FREKVENS_DIMC, 8);

  Serial.println("Demo routine finished");
  // END OF DEMO ROUTINES
  //--------------------------------------------------------------------
}

void multiDemo(){
  demo(g_routine);
  if (g_routine<DEFINED_ROUTINES)
    g_routine++;
  else
    g_routine=FIRST_ROUTINE;
}

void demoInterrupt(){
  fgen_pixel_picker(g_bitmap, FREKVENS_DIMC, id);
  mrefresh2(g_bitmap, FREKVENS_DIMC, 8);
  if (id<255){
    id++;
    return;
  }
  id=0;
}

void demoGrayscale(){

  //version: fullscreen image fade
  for (int i=0;i<FREKVENS_DIMC;i++){
    for (int j=0;j<FREKVENS_DIMC;j++){
      FrekvensLoadPixel(i, j, demo_fade_image.gray);
    }
  }
  if (demo_fade_image.fade_cntr<demo_fade_image.fade_prescaler){
    demo_fade_image.fade_cntr++;
  }
  else {
    demo_fade_image.fade_cntr = 0;
    //generate pulsating 'demo_fade_image.gray' value
    if (!demo_fade_image.fade_reverse){
      if (demo_fade_image.gray<15){
        demo_fade_image.gray++;
      }
      else
        demo_fade_image.fade_reverse = true;
    }
    else{
      if (demo_fade_image.gray>1){  //never fade to black entirely
        demo_fade_image.gray--;
      }
      else
        demo_fade_image.fade_reverse = false;
    }
  }
}

void fgen_pixel_picker(uint8_t (*bitmap)[FREKVENS_DIMC], uint8_t rows, int pixel){
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
      cnt++;
    }
  }
}