// frekvens_driver.h
// Display driver for FREKVENS LED array

#include <Arduino.h>
#include <SPI.h>

//NOP instruction
//inline assembly
#define NOP __asm__ __volatile__ ("nop\n\t")

//Common dimension (rows)
//Display is (DIMC) pixels tall
#define DIMC 16
//Column count for the frame buffer
//Display is (COLB x 8) pixels wide
#define COLB 2

//SPISettings parameters
#define SRSPEED 125000      //speedMaximum
#define SRORDER LSBFIRST    //dataOrder
#define SRMODE SPI_MODE0    //dataMode

/**
* GLOBAL VARIABLE
* Frame bitmap array
*/
extern uint8_t g_bitmap[DIMC][DIMC];

/*
* //DEPRECATED FUNCTION!
* Transmits the frame to the LED drivers via SPI
*
* frame:    byte array, 16x2
* rows:     array row count
* cols:     array column count
* latch:    Latch pin number
* enable:   Output Enable pin number
*/
void refresh(uint8_t (*frame)[COLB], uint8_t rows, uint8_t cols, uint8_t latch, uint8_t enable);

/*
* //DEPRECATED FUNCTION!
* Converts a bitmap into a direct frame for transmission
*
* bitmap:   byte array, 16x16
* frame:    byte array, 16x2
* rows:     array row count (bitmap and frame)
* cols:     array column count (frame)
*/
bool map(uint8_t (*bitmap)[DIMC], uint8_t (*frame)[COLB], uint8_t rows, uint8_t cols);

/**
* Compiles the frame from the bitmap and transmits it to the LED drivers via SPI
*
* *bitmap:  Byte array that contains the image.
* dim:      Dimension of bitmap (square matrix).
* mask:     Bitmask for grayscale processing. Write 8 to prevent masking.
* latch:    Latch pin number.
* enable:   Output Enable pin number.
*/
void mrefresh(uint8_t (*bitmap)[DIMC], uint8_t dim, uint8_t mask, uint8_t latch, uint8_t enable);

/*
* //DEPRECATED FUNCTION!
* Compiles the frame from the global bitmap and transmits it to the LED drivers via SPI
*
* mask:     Bitmask for grayscale processing. Write 8 to prevent masking.
* latch:    Latch pin number.
* enable:   Output Enable pin number.
*/
void gmrefresh(uint8_t mask, uint8_t latch, uint8_t enable);

/*
* //DEPRECATED FUNCTION!
* Compiles a single byte from eitght consecutive values by address
*/
static inline uint8_t mapb(uint8_t* address, uint8_t buffer);

/**
* Dim the display by applying PWM to the Output Enable pin.
* 
* oe_pin:   Output Enable pin number.
* dim:      Dimness value (1 - 254). Write 0 to disable PWM.
*/
void enableDisplay(int oe_pin, uint8_t dim);

/**
* Disable the display via the Output Enable pin
*
* oe_pin:   Output Enable pin number.
*/
void disableDisplay(int oe_pin);
