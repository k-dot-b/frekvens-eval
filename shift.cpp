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
bool flag_frekvens_activity = false;

/**
* GLOBAL VARIABLE
* Frame mask selector for Binary Code Modulation.
* Default value of '8' disables masking
*/
uint8_t frekvens_bitmask_index = 8;


//LIMITED SCOPE VARIABLES

struct displayPhy {
  int latch = 0;
  int enable = 0;
} displayData;  //Display control pins

uint8_t i_bitmap_buffer[DIMC][DIMC];

static const uint8_t bitmask[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0xff};


bool FrekvensAttachDisplay(int latch_pin, int enable_pin){
  if (latch_pin==enable_pin)
    return false;
  displayData.latch = latch_pin;
  displayData.enable = enable_pin;

  pinMode(latch_pin, OUTPUT);
  pinMode(enable_pin, OUTPUT);

  digitalWrite(latch_pin, LOW);
  digitalWrite(enable_pin, LOW);  //Enable display
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

  for (int i=0;i<DIMC;i++){
    for (int j=0;j<8;j++){    //read bits 0-7 in every row
      buffer[cnt] <<= 1;
      if ((i_bitmap_buffer[i][j] & bitmask[frekvens_bitmask_index]))
        buffer[cnt] |= 1;
      }
    cnt++;
  }
  for (int i=0;i<DIMC;i++){
    for (int j=8;j<16;j++){   //read bits 8-15 in every row
      buffer[cnt] <<= 1;
      if ((i_bitmap_buffer[i][j] & bitmask[frekvens_bitmask_index]))
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
        if ((bitmap[i][cnt] & bitmask[mask]))
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
        if ((bitmap[i][cnt] & bitmask[mask]))
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

void FrekvensEnableDisplayDimming(uint8_t dimness){
  frekvens_bitmask_index = 8;   //disable masking
  analogWrite(displayData.enable, dimness);
}

void FrekvensEnableDisplay(){
  digitalWrite(displayData.enable, LOW);
}

void FrekvensDisableDisplay(){
  digitalWrite(displayData.enable, HIGH);
}
