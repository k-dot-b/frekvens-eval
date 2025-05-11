#include <Arduino.h>

#ifndef _SPI_H_INCLUDED
#include <SPI.h>
#endif

#include "params.h"
#include "utils.h"

//SPISettings parameters
#define SRSPEED 31250       //speedMaximum
#define SRORDER LSBFIRST    //dataOrder
#define SRMODE SPI_MODE1    //dataMode

//declarations
void refresh(uint8_t frame[ROWC][COLC], uint8_t latch_pin_n, uint8_t oe_pin_n);
