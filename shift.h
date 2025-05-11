#include <Arduino.h>
#include <SPI.h>

#include "params.h"
#include "utils.h"

#ifndef ROWC
#define ROWC 16
#endif
#ifndef COLC
#define COLC 2
#endif
#define SRSPEED 31250
#define SRORDER LSBFIRST
#define SRMODE SPI_MODE1

//declarations
void refresh(uint8_t frame[ROWC][COLC], uint8_t latch_pin_n, uint8_t oe_pin_n);
