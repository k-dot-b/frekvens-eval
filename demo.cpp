#include "demo.h"

//NOP instruction
//inline assembly
#define NOP __asm__ __volatile__ ("nop\n\t")

uint8_t id = 0;

//DEPRECATED FUNCTION FOR LEGACY DEMO ROUTINE (cluster stepper)
void refresh(uint8_t (*frame)[COLB], uint8_t rows, uint8_t cols, uint8_t latch, uint8_t enable){
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
  NOP;
  digitalWrite(latch, LOW);

  digitalWrite(enable, LOW);	//enable display
}

void demo(uint8_t routine){
  if (routine<1 || routine>DEFINED_ROUTINES)
    return;
  
  //--------------------------------------------------------------------
  // WRITE DEMO ROUTINES HERE AS CASES:
  switch (routine){
    //DEPRECATED test routine 1: cluster stepper - sequential
    case 1:
      for (int d=0;d<CLUSTER_MAX;d++){
        fgen_cluster_picker(g_frame, ROWC, COLC, d, CLUSTER_DATA);

        #ifdef VERBOSE_DEMO
        Serial.println("Cluster step frame:");
        for(int i=0;i<ROWC;i++){
          for(int j=0;j<COLC;j++){
          Serial.print(g_frame[i][j]);
          Serial.print("		");
          }
          Serial.println();
        }
        #endif

        refresh(g_frame, ROWC, COLC, LATCH_PIN, OE_PIN);
        delay(STEP_DELAY_1);
      }
      break;
      //end test routine 1
    
    //test routine 2: pixel chase - sequential
    case 2:
      for (int d=0;d<PIXEL_MAX;d++){
        fgen_pixel_picker(g_bitmap, DIMC, d);
        mrefresh(g_bitmap, DIMC, 8, LATCH_PIN, OE_PIN);
        delay(STEP_DELAY_2);
      }
      break;
      //end test routine 2
    
    //test routine 3: pixel chase - reverse
    case 3:
      for (int d=PIXEL_MAX;d>0;d--){
        fgen_pixel_picker(g_bitmap, DIMC, d);
        mrefresh(g_bitmap, DIMC, 8, LATCH_PIN, OE_PIN);
        delay(STEP_DELAY_2);
      }
      break;
    
    //test routine 4: new SPI algorithm, pixel chase - forward
    case 4:
      for (int d=0;d<PIXEL_MAX;d++){
        fgen_pixel_picker(g_bitmap, DIMC, d);
        FrekvensLoadBuffer(g_bitmap, DIMC);
        tempmrefresh(8);
        delay(STEP_DELAY_2);
      }

    default:
      break;
  }
  // END OF DEMO ROUTINES
  //--------------------------------------------------------------------
}

void demoInterrupt(){
  fgen_pixel_picker(g_bitmap, DIMC, id);
  mrefresh(g_bitmap, DIMC, 8, LATCH_PIN, OE_PIN);
  if (id<255){
    id++;
    return;
  }
  id=0;
}

void fgen_cluster_picker(uint8_t (*frame)[COLC], uint8_t rows, uint8_t cols, int segment, uint8_t data){
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

void fgen_pixel_picker(uint8_t (*bitmap)[ROWC], uint8_t rows, int pixel){
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