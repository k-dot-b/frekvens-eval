// frekvens_driver.cpp
// Display driver for FREKVENS LED array

#include "shift.h"

//NOP instruction
//inline assembly
#define NOP __asm__ __volatile__ ("nop\n\t")

//SPISettings parameters
#define SRSPEED 125000      //speedMaximum
#define SRORDER LSBFIRST    //dataOrder
#define SRMODE SPI_MODE0    //dataMode

uint8_t g_bitmap[DIMC][DIMC];

//For future use
uint8_t bitmapBuffer[DIMC][DIMC];

//Display parameters
struct displayPhy {
  int latch = 0;
  int enable = 0;
} displayData;

static const uint8_t bitmask[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0xff};

bool attachDisplay(int latch_pin, int enable_pin){
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

//DEPRECATED FUNCTION!
void refresh(uint8_t (*frame)[COLB], uint8_t rows, uint8_t cols, uint8_t latch, uint8_t enable){
	if (!(frame&&*frame))
		return;
	
  digitalWrite(enable, HIGH);	//blank display

  SPI.beginTransaction(SPISettings(SRSPEED, SRORDER, SRMODE));
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

//DEPRECATED FUNCTION!
bool map(uint8_t (*bitmap)[DIMC], uint8_t (*frame)[COLB], uint8_t rows, uint8_t cols){
  if (!(bitmap&&*bitmap) || !(frame&&*frame))
    return false;

  for (int i=0;i<rows;i++){
    uint8_t cnt = 0;
    for (int j=0;j<cols;j++){
      for (int k=0;k<8;k++){
        frame[i][j] <<= 1;
        if (bitmap[i][cnt])
          frame[i][j] |= 1;
        cnt++;
      }
    }
  }
  return true;
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
        frame_buffer[i][j] >>= 1;
        if ((bitmap[i][cnt] & bitmask[mask]))
          frame_buffer[i][j] |= 128;
        cnt++;
      }
    }
  }
  //Transmit frame through SPI
  SPI.beginTransaction(SPISettings(SRSPEED, SRORDER, SRMODE));
	//Last value first
  //SRORDER: LSBFIRST
	for(int j=0;j<COLB;j++){
		for(int i=0;i<DIMC;i++){
    int received = SPI.transfer(frame_buffer[i][j]);
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
  uint8_t frame_buffer[DIMC][COLB];
  for (int i=0;i<dimension;i++){
    uint8_t cnt = 0;
    for (int j=0;j<COLB;j++){
      for (int k=0;k<8;k++){
        frame_buffer[i][j] >>= 1;
        if ((bitmap[i][cnt] & bitmask[mask]))
          frame_buffer[i][j] |= 128;
        cnt++;
      }
    }
  }
  //Transmit frame through SPI
  SPI.beginTransaction(SPISettings(SRSPEED, SRORDER, SRMODE));
	//Last value first
  //SRORDER: LSBFIRST
	for(int j=0;j<COLB;j++){
		for(int i=0;i<DIMC;i++){
    int received = SPI.transfer(frame_buffer[i][j]);
		}
	}
  SPI.endTransaction();

  digitalWrite(displayData.latch, HIGH);	//latch new values
  NOP;
  digitalWrite(displayData.latch, LOW);
}

//DEPRECATED FUNCTION!
void gmrefresh(uint8_t mask, uint8_t latch, uint8_t enable){
  if (!g_bitmap)
		return;
	
  //Compile a frame from the bitmap
  uint8_t frame_buffer[DIMC][COLB];
  for (int i=0;i<DIMC;i++){
    uint8_t cnt = 0;
    for (int j=0;j<COLB;j++){
      for (int k=0;k<8;k++){
        frame_buffer[i][j] >>= 1;
        if ((g_bitmap[i][cnt] & bitmask[mask]))
          frame_buffer[i][j] |= 128;
        cnt++;
      }
    }
  }
  //Transmit frame through SPI
  SPI.beginTransaction(SPISettings(SRSPEED, SRORDER, SRMODE));
	//Last value first
  //SRORDER: LSBFIRST
	for(int j=COLB-1;j>=0;j--){
		for(int i=DIMC-1;i>=0;i--){
    int received = SPI.transfer(frame_buffer[i][j]);
		}
	}
  SPI.endTransaction();

  digitalWrite(latch, HIGH);	//latch new values
  NOP;
  digitalWrite(latch, LOW);
}

static inline uint8_t mapb(uint8_t* address, uint8_t buffer){
    buffer <<= 1;
    if (*address)
      buffer |= 1;
    address++;
  return buffer;
}

void enableDisplay(uint8_t dimness){
  if (dimness>0 && dimness<255){
    analogWrite(displayData.enable, dimness);
    return;
  }
  digitalWrite(displayData.enable, LOW);
}

void disableDisplay(){
  digitalWrite(displayData.enable, HIGH);
}
