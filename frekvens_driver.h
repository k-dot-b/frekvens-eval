/*
 * Copyright 2025 Benedek Kiss
 * Licensed under the EUPL
 *
 * frekvens_driver.h
 */

// Display driver for FREKVENS LED array

#ifndef FREKVENS_DRIVER_H_INCLUDED
#define FREKVENS_DRIVER_H_INCLUDED

#include <Arduino.h>
#ifndef _SPI_H_INCLUDED
#include <SPI.h>
#endif

#define FREKVENS_STATUS_SUCCESS 0
#define FREKVENS_STATUS_FAILURE 1

//Common dimension of display related arrays (rows and columns)
//Display is square with (FREKVENS_DIMC) pixels on all sides
#define FREKVENS_DIMC 16
//Column count of the frame buffer array
//Display is (FREKVENS_COLB x 8) pixels wide
#define FREKVENS_COLB 2

#define FREKVENS_GRAYSCALE_OFF    1
#define FREKVENS_GRAYSCALE_2BITS  2
#define FREKVENS_GRAYSCALE_3BITS  3
#define FREKVENS_GRAYSCALE_4BITS  4
#define FREKVENS_GRAYSCALE_5BITS  5
#define FREKVENS_GRAYSCALE_6BITS  6
#define FREKVENS_GRAYSCALE_7BITS  7
#define FREKVENS_GRAYSCALE_8BITS  8


extern volatile bool frekvens_vsync_ready;


/**
* Initialize the display.
*
* Define the physical connections to the display driver ICs.
* Calculate parameters for the binary code modulation algorithm.
*
* latch_pin:    Latch pin number.
* enable_pin:   Output Enable pin number.
* bit_depth:    Bit depth of the grayscale image.
*
* return        FREKVENS_STATUS_SUCCESS or FREKVENS_STATUS_FAILURE
*/
uint8_t FrekvensAttachDisplay(int latch_pin, int enable_pin, int bit_depth);

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
* Read data from the specified pixel of the display buffer.
*
* row:    Bitmap X coordinate.
* col:    Bitmap Y coordinate.
*
* return  Pixel data.
*/
uint8_t FrekvensReadPixel(uint8_t row, uint8_t col);

/**
* Refresh the display with the buffered bitmap.
* Masking must be set via global variable 'FrekvensBCM.bitmask_index'
*/
void FrekvensRefreshDisplay();

/**
* DEPRECATED FUNCTION!
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
* DEPRECATED FUNCTION!
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
* Enable the display in BCM mode.
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
