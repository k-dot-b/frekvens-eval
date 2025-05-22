// frekvens_driver.h
// Display driver for FREKVENS LED array

#ifndef FREKVENS_DRIVER_H_INCLUDED
#define FREKVENS_DRIVER_H_INCLUDED

#include <Arduino.h>
#ifndef _SPI_H_INCLUDED
#include <SPI.h>
#endif

//Common dimension (rows)
//Display is (DIMC) pixels tall
#define DIMC 16
//Column count for the frame buffer
//Display is (COLB x 8) pixels wide
#define COLB 2

/**
* GLOBAL VARIABLE
* Frame bitmap array
*/
extern uint8_t g_bitmap[DIMC][DIMC];

/**
* Defines the physical connections to the display driver ICs
*
* latch_pin:    Latch pin number.
* enable_pin:   Output Enable pin number.
*/
bool attachDisplay(int latch_pin, int enable_pin);

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
* dimension:      Dimension of bitmap (square matrix).
* mask:     Bitmask for grayscale processing. Write 8 to prevent masking.
* latch:    Latch pin number.
* enable:   Output Enable pin number.
*/
void mrefresh(uint8_t (*bitmap)[DIMC], uint8_t dimension, uint8_t mask, uint8_t latch, uint8_t enable);

/**
* New version of mrefresh() using internal sources for display parameters.
* Compiles the frame from the bitmap and transmits it to the LED drivers via SPI.
* "map-and-refresh"
*
* *bitmap:    Byte array that contains the image.
* dimension:  Dimension of passed array (must be square!).
* mask:       Bitmask for grayscale processing. Write 8 to prevent masking.
* latch:      Latch pin number.
* enable:     Output Enable pin number.
*/
void mrefresh2(uint8_t (*bitmap)[DIMC], uint8_t dimension, uint8_t mask);

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
* Enable the display. Dimming available by applying PWM to the Output Enable pin.
* 
* dimness:      Dimness value (1 - 254). Write 0 or 'false' to disable PWM.
*/
void enableDisplay(uint8_t dimness);

/**
* Disable the display via the Output Enable pin
*/
void disableDisplay();

#endif //FREKVENS_DRIVER_H_INCLUDED
