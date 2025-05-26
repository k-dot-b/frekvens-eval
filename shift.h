// frekvens_driver.h
// Display driver for FREKVENS LED array

#ifndef FREKVENS_DRIVER_H_INCLUDED
#define FREKVENS_DRIVER_H_INCLUDED

#include <Arduino.h>
#ifndef _SPI_H_INCLUDED
#include <SPI.h>
#endif

//Common dimension of display related arrays (rows and columns)
//Display is square with (DIMC) pixels on all sides
#define DIMC 16
//Column count of the frame buffer array
//Display is (COLB x 8) pixels wide
#define COLB 2


extern uint8_t g_bitmap[DIMC][DIMC];
extern uint8_t frekvens_bitmask_index;

extern bool flag_bitmap_available;
extern bool flag_frame_available;

/**
* Defines the physical connections to the display driver ICs
*
* latch_pin:    Latch pin number.
* enable_pin:   Output Enable pin number.
*/
bool FrekvensAttachDisplay(int latch_pin, int enable_pin);

/**
* Load a complete bitmap into the display buffer
* 
* *bitmap:    Byte array that contains the image.
* dimension:  Dimension of bitmap (square matrix).
*/
void FrekvensLoadBuffer(uint8_t (*bitmap)[DIMC], uint8_t dimension);

/**
* Load data to the specified pixel of the display buffer.
* 
* data:   The data to be loaded.
* row:    Bitmap X coordinate.
* col:    Bitmap Y coordinate.
*/
void FrekvensLoadPixel(uint8_t data, uint8_t row, uint8_t col);

/**
* Refresh the display with the buffered bitmap.
* Masking must be set via global variable 'frekvens_bitmask_index'
*/
void FrekvensRefreshDisplay();

/**
* Compiles the frame from the bitmap and transmits it to the LED drivers via SPI
*
* *bitmap:    Byte array that contains the image.
* dimension:  Dimension of bitmap (square matrix).
* mask:       Bitmask for grayscale processing. Write 8 to prevent masking.
* latch:      Latch pin number.
* enable:     Output Enable pin number.
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
*/
void mrefresh2(uint8_t (*bitmap)[DIMC], uint8_t dimension, uint8_t mask);

/**
* Enable the display with global PWM dimming via the Output Enable pin.
* 
* dimness:    Dimness value (1 - 254). A value of '0' '255' or 'false' disables PWM dimming.
*/
void FrekvensEnableDisplayDimming(uint8_t dimness);

/**
* Enable the display
*/
void FrekvensEnableDisplay();

/**
* Disable the display
*/
void FrekvensDisableDisplay();

#endif //FREKVENS_DRIVER_H_INCLUDED
