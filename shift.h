#include <Arduino.h>

#ifndef _SPI_H_INCLUDED
#include <SPI.h>
#endif //_SPI_H_INCLUDED

#include "params.h"
#include "utils.h"

//frame columns
#define FRAME_COL 2

//SPISettings parameters
#define SRSPEED 125000      //speedMaximum
#define SRORDER LSBFIRST    //dataOrder
#define SRMODE SPI_MODE0    //dataMode

/*
* Transmits the frame to the LED drivers via SPI
*
* frame:    byte array, 16x2
* rows:     array row count
* cols:     array column count
* latch:    Latch pin number
* enable:   Output Enable pin number
*/
void refresh(uint8_t (*frame)[COLC], uint8_t rows, uint8_t cols, uint8_t latch, uint8_t enable);

/*
* Converts a bitmap into a direct frame for transmission
*
* bitmap:   byte array, 16x16
* frame:    byte array, 16x2
* rows:     array row count (bitmap and frame)
* cols:     array column count (frame)
*/
bool map(uint8_t (*bitmap)[ROWC], uint8_t (*frame)[COLC], uint8_t rows, uint8_t cols);

/*
* Transmits the frame to the LED drivers via SPI
*
* *bitmap:  byte array (square)
* dimc:     array common dimension
* latch:    Latch pin number
* enable:   Output Enable pin number
*/
void refreshf(uint8_t (*bitmap)[DIMC], uint8_t dimc, uint8_t latch, uint8_t enable);

/*
* Compiles a single byte from eitght consecutive values by address
*
* bitmap:   byte array, 16x16
* frame:    byte array, 16x2
* rows:     array row count (bitmap and frame)
* cols:     array column count (frame)
*/
static inline uint8_t mapb(uint8_t* address, uint8_t buffer);
