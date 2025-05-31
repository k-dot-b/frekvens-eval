// frekvens_driver.h
// Display driver for FREKVENS LED array

#ifndef FREKVENS_DRIVER_H_INCLUDED
#define FREKVENS_DRIVER_H_INCLUDED

#include <Arduino.h>
#ifndef _SPI_H_INCLUDED
#include <SPI.h>
#endif

//Common dimension of display related arrays (rows and columns)
//Display is square with (FREKVENS_DIMC) pixels on all sides
#define FREKVENS_DIMC 16
//Column count of the frame buffer array
//Display is (FREKVENS_COLB x 8) pixels wide
#define FREKVENS_COLB 2

//Grayscale image bit depth
#define FREKVENS_GRAYSCALE_BIT_DEPTH 4

struct displayBCM {
  uint8_t iter_max = 0;
  volatile uint8_t iter_index = 0;
  uint8_t bitmask_max = 0;
  volatile uint8_t bitmask_index = 8;
  const uint8_t bitmask[9] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0xff};
};


extern uint8_t g_bitmap[FREKVENS_DIMC][FREKVENS_DIMC];

extern volatile bool frekvens_vsync_ready;
extern displayBCM FrekvensBCM;

uint8_t debug_read_buffer(uint8_t row, uint8_t col);

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
void FrekvensLoadBuffer(uint8_t (*bitmap)[FREKVENS_DIMC], uint8_t dimension);

/**
* Load data to the specified pixel of the display buffer.
* 
* row:    Bitmap X coordinate.
* col:    Bitmap Y coordinate.
* data:   The data to be loaded.
*/
void FrekvensLoadPixel(uint8_t row, uint8_t col, uint8_t data);

/**
* Refresh the display with the buffered bitmap.
* Masking must be set via global variable 'FrekvensBCM.bitmask_index'
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
void mrefresh(uint8_t (*bitmap)[FREKVENS_DIMC], uint8_t dimension, uint8_t mask, uint8_t latch, uint8_t enable);

/**
* New version of mrefresh() using internal sources for display parameters.
* Compiles the frame from the bitmap and transmits it to the LED drivers via SPI.
* "map-and-refresh"
*
* *bitmap:    Byte array that contains the image.
* dimension:  Dimension of passed array (must be square!).
* mask:       Bitmask for grayscale processing. Write 8 to prevent masking.
*/
void mrefresh2(uint8_t (*bitmap)[FREKVENS_DIMC], uint8_t dimension, uint8_t mask);

/**
* Preload BCM parameters.
* WARNING! BCM is interrupt dependent!
* Timer interrupt must be configured separately.
*/
void FrekvensEnableDisplayGrayscale();

/**
* Enable the display with global PWM dimming via the Output Enable pin.
* 
* dimness:    Dimness value (0-255). Higher number means darker display.
*/
void FrekvensEnableDisplayDimming(uint8_t dimness);

/**
* Enable the display
*/
void FrekvensEnableDisplayStatic();

/**
* Disable the display
*/
void FrekvensDisableDisplay();

#endif //FREKVENS_DRIVER_H_INCLUDED
