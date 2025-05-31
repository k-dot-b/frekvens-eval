// frekvens_driver.cpp
// Display driver for FREKVENS LED array

#include "frekvens_driver.h"

//SPISettings parameters
#define FREKVENS_SRSPEED 2000000      //speedMaximum

//GLOBAL VARIABLES

/**
* GLOBAL FLAG
* Activity indicator for synchronising display related operations
*/
volatile bool frekvens_vsync_ready = false;

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
} displayPins;  //Display control pins

uint8_t i_bitmap_buffer[FREKVENS_DIMC][FREKVENS_DIMC];

//Static function declarations
static inline void configureInterruptTimer();
static inline void disableInterruptTimer();
static inline void enableInterruptTimer();

//-------------------------------------------------------------
// Frekvens Driver function definitions
uint8_t debug_read_buffer(uint8_t row, uint8_t col){
  if (row>FREKVENS_DIMC || col>FREKVENS_DIMC)
    return 0;
  
  return i_bitmap_buffer[row][col];
}

bool FrekvensAttachDisplay(int latch_pin, int enable_pin){
  if (latch_pin==enable_pin)
    return false;

  SPI.begin();
  displayPins.latch = latch_pin;
  displayPins.enable = enable_pin;

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

  configureInterruptTimer();

  return true;
}

void FrekvensLoadBuffer(uint8_t (*bitmap)[FREKVENS_DIMC], uint8_t dimension){
  if (!(bitmap&&*bitmap) && dimension!=FREKVENS_DIMC)
		return;

  memcpy(i_bitmap_buffer, bitmap, FREKVENS_DIMC*FREKVENS_DIMC);
}

void FrekvensLoadPixel(uint8_t row, uint8_t col, uint8_t data){
  if (row>FREKVENS_DIMC || col>FREKVENS_DIMC)
    return;

  i_bitmap_buffer[row][col] = data;
}

void FrekvensRefreshDisplay(){
  uint8_t buffer[FREKVENS_DIMC*FREKVENS_COLB];
  uint8_t cnt = 0;

  for (uint8_t i=0;i<FREKVENS_DIMC;i++){
    for (uint8_t j=0;j<8;j++){    //read bits 0-7 in every row
      buffer[cnt] <<= 1;
      if ((i_bitmap_buffer[i][j] & FrekvensBCM.bitmask[FrekvensBCM.bitmask_index]))
        buffer[cnt] |= 1;
      }
    cnt++;
  }
  for (uint8_t i=0;i<FREKVENS_DIMC;i++){
    for (uint8_t j=8;j<16;j++){   //read bits 8-15 in every row
      buffer[cnt] <<= 1;
      if ((i_bitmap_buffer[i][j] & FrekvensBCM.bitmask[FrekvensBCM.bitmask_index]))
        buffer[cnt] |= 1;
      }
    cnt++;
  }
  SPI.beginTransaction(SPISettings(FREKVENS_SRSPEED, MSBFIRST, SPI_MODE0));
  SPI.transfer(buffer, (FREKVENS_DIMC*FREKVENS_COLB));
  SPI.endTransaction();

  digitalWrite(displayPins.latch, HIGH);
  _NOP();
  digitalWrite(displayPins.latch, LOW);
}

void mrefresh(uint8_t (*bitmap)[FREKVENS_DIMC], uint8_t dimension, uint8_t mask, uint8_t latch, uint8_t enable){
	if (!(bitmap&&*bitmap))
		return;
  if (dimension!=FREKVENS_DIMC)
    return;
	
  //Compile a frame from the bitmap
  uint8_t frame_buffer[FREKVENS_DIMC][FREKVENS_COLB];
  for (int i=0;i<dimension;i++){
    uint8_t cnt = 0;
    for (int j=0;j<FREKVENS_COLB;j++){
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
	for(int j=0;j<FREKVENS_COLB;j++){
		for(int i=0;i<FREKVENS_DIMC;i++){
    int received = SPI.transfer(frame_buffer[i][j]);
		}
	}
  SPI.endTransaction();

  digitalWrite(latch, HIGH);	//latch new values
  _NOP();
  digitalWrite(latch, LOW);
}

void mrefresh2(uint8_t (*bitmap)[FREKVENS_DIMC], uint8_t dimension, uint8_t mask){
  if (!(bitmap&&*bitmap))
		return;
  if (dimension!=FREKVENS_DIMC)
    return;
	
  //Compile a frame from the bitmap
  uint8_t frame_buffer[FREKVENS_DIMC][FREKVENS_COLB];
  for (int i=0;i<dimension;i++){
    uint8_t cnt = 0;
    for (int j=0;j<FREKVENS_COLB;j++){
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
	for(int j=0;j<FREKVENS_COLB;j++){
		for(int i=0;i<FREKVENS_DIMC;i++){
    int received = SPI.transfer(frame_buffer[i][j]);
		}
	}
  SPI.endTransaction();

  digitalWrite(displayPins.latch, HIGH);	//latch new values
  _NOP();
  digitalWrite(displayPins.latch, LOW);
}

void FrekvensEnableDisplayGrayscale(){
  FrekvensBCM.iter_index = FrekvensBCM.iter_max;
  FrekvensBCM.bitmask_index = FrekvensBCM.bitmask_max;
  digitalWrite(displayPins.enable, LOW);
  enableInterruptTimer();
}

void FrekvensEnableDisplayDimming(uint8_t dimness){
  disableInterruptTimer();
  FrekvensBCM.bitmask_index = 8;   //disable masking
  analogWrite(displayPins.enable, dimness);
}

void FrekvensEnableDisplayStatic(){
  disableInterruptTimer();
  FrekvensBCM.bitmask_index = 8;   //disable masking
  digitalWrite(displayPins.enable, LOW);
}

void FrekvensDisableDisplay(){
  disableInterruptTimer();
  digitalWrite(displayPins.enable, HIGH);
}

//-------------------------------------------------------------
// HARDWARE SPECIFIC FUNCTIONS

static inline void configureInterruptTimer(){
  //-----------------------------------------------------------
  #if defined (__AVR_ATmega328P__)
  /*
  * Arduino Uno timer1
  * 
  * frequency | CSx2-1-0  | OCR
  * 1600 Hz   | 0-1-0     | 1249
  */
  cli();  //Clear interrupts
  TCCR1A = 0; //Timer-Counter Control Register 1A
  TCCR1B = 0;
  TCNT1 = 0;  //initialize counter value to 0
  TCCR1B |= (1<<WGM12);
  TCCR1B |= (0<<CS12) | (1<<CS11) | (0<<CS10);
  OCR1A = 1249; // 1600Hz subframe refresh frequency
  OCR1B = 1999; // 1000Hz / 1ms intervals, millis() substitute
  //Disable interrupts by default:
  TIMSK1 = (0<<ICIE1) | (0<<OCIE1B) | (0<<OCIE1A) | (0<<TOIE1);
  sei();  //Enable interrupts
  //End timer configuration
  #endif //AVR_ATmega328P
  //-----------------------------------------------------------
}

static inline void disableInterruptTimer(){
  #if defined (__AVR_ATmega328P__)
  cli();
  TIMSK1  &= ~(1<<OCIE1A);
  TIFR1   |= (1<<OCF1A);
  sei();
  #endif //__AVR_ATmega328P__
}

static inline void enableInterruptTimer(){
  #if defined (__AVR_ATmega328P__)
  cli();
  TIMSK1  |= (1<<OCIE1A);
  TIFR1   |= (1<<OCF1A);
  sei();
  #endif //__AVR_ATmega328P__
}

//-------------------------------------------------------------
// INTERRUPT SERVICE ROUTINES

#if defined (__AVR_ATmega328P__)
ISR(TIMER1_COMPA_vect){

  FrekvensRefreshDisplay();

  //BCM algorithm
  if ((FrekvensBCM.iter_index & FrekvensBCM.bitmask[FrekvensBCM.bitmask_index])){
      FrekvensBCM.iter_index--;
  }
  else {
    if (FrekvensBCM.iter_index){
      FrekvensBCM.iter_index--;
      FrekvensBCM.bitmask_index--;
    }
    else {
      FrekvensBCM.iter_index = FrekvensBCM.iter_max;        //reload counter
      FrekvensBCM.bitmask_index = FrekvensBCM.bitmask_max;  //reload bitmask
      frekvens_vsync_ready = true;                          //signal frame completion
    }
  }
}
#endif //__AVR_ATmega328P__
