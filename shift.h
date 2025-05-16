#include <Arduino.h>

#ifndef _SPI_H_INCLUDED
#include <SPI.h>
#endif

#include "params.h"
#include "utils.h"

//SPISettings parameters
#define SRSPEED 125000      //speedMaximum
#define SRORDER LSBFIRST    //dataOrder
#define SRMODE SPI_MODE0    //dataMode

/*
* Transmits the frame to the LED drivers via SPI
*/
void refresh(uint8_t (*frame)[COLC], uint8_t rows, uint8_t cols, uint8_t latch, uint8_t enable);

/*
* Converts a bitmap into a direct frame for transmission
*/
bool map(uint8_t (*bitmap)[ROWC], uint8_t (*frame)[COLC], uint8_t rows, uint8_t cols);
