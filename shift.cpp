// frekvens_driver.cpp
// Display driver for FREKVENS LED array

#include "shift.h"

//NOP instruction
//inline assembly
#define NOP __asm__ __volatile__ ("nop\n\t")

//SPISettings parameters
#define FREKVENS_SRSPEED 125000      //speedMaximum

//GLOBAL VARIABLES

/**
* DEPRECATED GLOBAL VARIABLE
* Frame bitmap array
*/
uint8_t g_bitmap[DIMC][DIMC];

/**
* GLOBAL FLAG
* Bitmap buffer access control flag
*/
bool flag_bitmap_available = true;

/**
* GLOBAL FLAG
* Frame buffer access control flag
*/
bool flag_frame_available = true;

/**
* GLOBAL VARIABLE
* Frame mask selector for Binary Code Modulation.
* Default value of '8' disables masking
*/
uint8_t frekvens_bitmask_index = 8;

//LIMITED SCOPE VARIABLES
uint8_t i_bitmap_buffer[DIMC][DIMC];
uint8_t i_frame_buffer[DIMC][COLB];
struct displayPhy {
  int latch = 0;
  int enable = 0;
} displayData;  //Display parameters
static const uint8_t bitmask[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0xff};

//Local version of map function
static inline bool i_map(uint8_t mask){
  //Compile a frame from the bitmap
  for (int i=0;i<DIMC;i++){
    uint8_t cnt = 0;
    for (int j=0;j<COLB;j++){
      for (int k=0;k<8;k++){
        i_frame_buffer[i][j] <<= 1;
        if ((i_bitmap_buffer[i][cnt] & bitmask[mask]))
          i_frame_buffer[i][j] |= 1;
        cnt++;
      }
    }
  }
  return true;
}

//Local version of refresh function
static inline bool i_refresh(){
  //Transmit the frame through SPI
  SPI.beginTransaction(SPISettings(FREKVENS_SRSPEED, MSBFIRST, SPI_MODE0));
	for(int j=0;j<COLB;j++){
		for(int i=0;i<DIMC;i++){
    int received = SPI.transfer(i_frame_buffer[i][j]);
		}
	}
  SPI.endTransaction();
  //Latch new data
  digitalWrite(displayData.latch, HIGH);
  NOP;
  digitalWrite(displayData.latch, LOW);
  return true;
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
  return true;
}

void FrekvensLoadBuffer(uint8_t (*bitmap)[DIMC], uint8_t dimension){
  if (!(bitmap&&*bitmap) && dimension!=DIMC)
		return;
  //if (dimension!=DIMC)
    //return;
  if (!flag_bitmap_available)
    return;

  flag_bitmap_available = false;
  memcpy(i_bitmap_buffer, bitmap, DIMC*DIMC);
  flag_bitmap_available = true;
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
  NOP;
  digitalWrite(displayData.latch, LOW);
}

void mrefresh(uint8_t (*bitmap)[DIMC], uint8_t dimension, uint8_t mask, uint8_t latch, uint8_t enable){
	if (!(bitmap&&*bitmap))
		return;
  if (dimension!=DIMC)
    return;
	
  //Compile a frame from the bitmap
  uint8_t i_frame_buffer[DIMC][COLB];
  for (int i=0;i<dimension;i++){
    uint8_t cnt = 0;
    for (int j=0;j<COLB;j++){
      for (int k=0;k<8;k++){
        i_frame_buffer[i][j] >>= 1;
        if ((bitmap[i][cnt] & bitmask[mask]))
          i_frame_buffer[i][j] |= 128;
        cnt++;
      }
    }
  }
  //Transmit frame through SPI
  SPI.beginTransaction(SPISettings(FREKVENS_SRSPEED, LSBFIRST, SPI_MODE0));
	//Last value first
  //SRORDER: LSBFIRST
	for(int j=0;j<COLB;j++){
		for(int i=0;i<DIMC;i++){
    int received = SPI.transfer(i_frame_buffer[i][j]);
		}
	}
  SPI.endTransaction();

  digitalWrite(latch, HIGH);	//latch new values
  NOP;
  digitalWrite(latch, LOW);
}

void mrefresh2(uint8_t (*bitmap)[DIMC], uint8_t dimension, uint8_t mask){
  if (!(bitmap&&*bitmap))
		return;
  if (dimension!=DIMC)
    return;
	
  //Compile a frame from the bitmap
  uint8_t i_frame_buffer[DIMC][COLB];
  for (int i=0;i<dimension;i++){
    uint8_t cnt = 0;
    for (int j=0;j<COLB;j++){
      for (int k=0;k<8;k++){
        i_frame_buffer[i][j] >>= 1;
        if ((bitmap[i][cnt] & bitmask[mask]))
          i_frame_buffer[i][j] |= 128;
        cnt++;
      }
    }
  }
  //Transmit frame through SPI
  SPI.beginTransaction(SPISettings(FREKVENS_SRSPEED, LSBFIRST, SPI_MODE0));
	//Last value first
  //SRORDER: LSBFIRST
	for(int j=0;j<COLB;j++){
		for(int i=0;i<DIMC;i++){
    int received = SPI.transfer(i_frame_buffer[i][j]);
		}
	}
  SPI.endTransaction();

  digitalWrite(displayData.latch, HIGH);	//latch new values
  NOP;
  digitalWrite(displayData.latch, LOW);
}

//Deprecated function!
static inline uint8_t mapb(uint8_t* address, uint8_t buffer){
    buffer <<= 1;
    if (*address)
      buffer |= 1;
    address++;
  return buffer;
}

void FrekvensEnableDisplayDimming(uint8_t dimness){
  if (dimness>0 && dimness<255){
    analogWrite(displayData.enable, dimness);
    return;
  }
  frekvens_bitmask_index = 8;   //disable masking
  digitalWrite(displayData.enable, LOW);
}

void FrekvensEnableDisplay(){
  digitalWrite(displayData.enable, LOW);
}

void FrekvensDisableDisplay(){
  digitalWrite(displayData.enable, HIGH);
}
