// frekvens_driver.cpp
// Display driver for FREKVENS LED array

#include "shift.h"

//SPISettings parameters
#define FREKVENS_SRSPEED 2000000      //speedMaximum

//GLOBAL VARIABLES

/**
* DEPRECATED GLOBAL VARIABLE
* Frame bitmap array
*/
uint8_t g_bitmap[DIMC][DIMC];

/**
* GLOBAL FLAG
* Activity indicator for synchronising display related operations
*/
volatile bool flag_frekvens_activity = false;

/**
* GLOBAL STRUCT
* Contains all global parameters for the Binary Code Modulation algorithm
* 
* .iter_max         Number of required iterations. Depends on bit depth.
* .iter_index       Iteration counter.
* .bitmask_max      Default index value. Depends on bit depth.
* .bitmask_index    Frame mask selector. Default value of '8' disables masking.
* .bitmask[9]       Constant array with binary masking values
*/
struct displayBCM FrekvensBCM;

//LIMITED SCOPE VARIABLES

struct displayPhy {
  int latch = 0;
  int enable = 0;
} displayData;  //Display control pins

uint8_t i_bitmap_buffer[DIMC][DIMC];


uint8_t debug_read_buffer(uint8_t row, uint8_t col){
  return i_bitmap_buffer[row][col];
}

bool FrekvensAttachDisplay(int latch_pin, int enable_pin){
  if (latch_pin==enable_pin)
    return false;
  displayData.latch = latch_pin;
  displayData.enable = enable_pin;

  pinMode(latch_pin, OUTPUT);
  pinMode(enable_pin, OUTPUT);

  digitalWrite(latch_pin, LOW);
  digitalWrite(enable_pin, LOW);  //Enable display

  //Grayscale parameters
  for (uint8_t i=1;i<FREKVENS_GRAYSCALE_BIT_DEPTH;i++){
    //calculate 2^(bit_depth)-1 which will be the number of required subframes for BCM
    FrekvensBCM.iter_max |= 1<<i;
  }
  FrekvensBCM.iter_index = FrekvensBCM.iter_max;
  FrekvensBCM.bitmask_max = FREKVENS_GRAYSCALE_BIT_DEPTH - 1;

  return true;
}

void FrekvensLoadBuffer(uint8_t (*bitmap)[DIMC], uint8_t dimension){
  if (!(bitmap&&*bitmap) && dimension!=DIMC)
		return;

  memcpy(i_bitmap_buffer, bitmap, DIMC*DIMC);
}

void FrekvensLoadPixel(uint8_t row, uint8_t col, uint8_t data){
  if (row>DIMC || col>DIMC)
    return;

  i_bitmap_buffer[row][col] = data;
}

void FrekvensRefreshDisplay(){
  uint8_t buffer[DIMC*COLB];
  uint8_t cnt = 0;

  for (uint8_t i=0;i<DIMC;i++){
    for (uint8_t j=0;j<8;j++){    //read bits 0-7 in every row
      buffer[cnt] <<= 1;
      if ((i_bitmap_buffer[i][j] & FrekvensBCM.bitmask[FrekvensBCM.bitmask_index]))
        buffer[cnt] |= 1;
      }
    cnt++;
  }
  for (uint8_t i=0;i<DIMC;i++){
    for (uint8_t j=8;j<16;j++){   //read bits 8-15 in every row
      buffer[cnt] <<= 1;
      if ((i_bitmap_buffer[i][j] & FrekvensBCM.bitmask[FrekvensBCM.bitmask_index]))
        buffer[cnt] |= 1;
      }
    cnt++;
  }
  SPI.beginTransaction(SPISettings(FREKVENS_SRSPEED, MSBFIRST, SPI_MODE0));
  SPI.transfer(buffer, (DIMC*COLB));
  SPI.endTransaction();

  digitalWrite(displayData.latch, HIGH);
  _NOP();
  digitalWrite(displayData.latch, LOW);
}

void mrefresh(uint8_t (*bitmap)[DIMC], uint8_t dimension, uint8_t mask, uint8_t latch, uint8_t enable){
	if (!(bitmap&&*bitmap))
		return;
  if (dimension!=DIMC)
    return;
	
  //Compile a frame from the bitmap
  uint8_t frame_buffer[DIMC][COLB];
  for (int i=0;i<dimension;i++){
    uint8_t cnt = 0;
    for (int j=0;j<COLB;j++){
      for (int k=0;k<8;k++){
        frame_buffer[i][j] <<= 1;
        if ((bitmap[i][cnt] & FrekvensBCM.bitmask[mask]))
          frame_buffer[i][j] |= 1;
        cnt++;
      }
    }
  }
  //Transmit frame through SPI
  SPI.beginTransaction(SPISettings(FREKVENS_SRSPEED, MSBFIRST, SPI_MODE0));
	//Last value first
  //SRORDER: LSBFIRST
	for(int j=0;j<COLB;j++){
		for(int i=0;i<DIMC;i++){
    int received = SPI.transfer(frame_buffer[i][j]);
		}
	}
  SPI.endTransaction();

  digitalWrite(latch, HIGH);	//latch new values
  _NOP();
  digitalWrite(latch, LOW);
}

void mrefresh2(uint8_t (*bitmap)[DIMC], uint8_t dimension, uint8_t mask){
  if (!(bitmap&&*bitmap))
		return;
  if (dimension!=DIMC)
    return;
	
  //Compile a frame from the bitmap
  uint8_t frame_buffer[DIMC][COLB];
  for (int i=0;i<dimension;i++){
    uint8_t cnt = 0;
    for (int j=0;j<COLB;j++){
      for (int k=0;k<8;k++){
        frame_buffer[i][j] <<= 1;
        if ((bitmap[i][cnt] & FrekvensBCM.bitmask[mask]))
          frame_buffer[i][j] |= 1;
        cnt++;
      }
    }
  }
  //Transmit frame through SPI
  SPI.beginTransaction(SPISettings(FREKVENS_SRSPEED, MSBFIRST, SPI_MODE0));
	//Last value first
  //SRORDER: LSBFIRST
	for(int j=0;j<COLB;j++){
		for(int i=0;i<DIMC;i++){
    int received = SPI.transfer(frame_buffer[i][j]);
		}
	}
  SPI.endTransaction();

  digitalWrite(displayData.latch, HIGH);	//latch new values
  _NOP();
  digitalWrite(displayData.latch, LOW);
}

void FrekvensEnableDisplayGrayscale(){
  FrekvensBCM.iter_index = FrekvensBCM.iter_max;
  FrekvensBCM.bitmask_index = FrekvensBCM.bitmask_max;
  digitalWrite(displayData.enable, LOW);
}

void FrekvensEnableDisplayDimming(uint8_t dimness){
  FrekvensBCM.bitmask_index = 8;   //disable masking
  analogWrite(displayData.enable, dimness);
}

void FrekvensEnableDisplayStatic(){
  FrekvensBCM.bitmask_index = 8;   //disable masking
  digitalWrite(displayData.enable, LOW);
}

void FrekvensDisableDisplay(){
  digitalWrite(displayData.enable, HIGH);
}
