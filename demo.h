/**
* Quick and dirty test patterns for Frekvens
*/
#ifndef _DEMO_H_INCLUDED
#define _DEMO_H_INCLUDED

#define VERBOSE_DEMO
#undef  VERBOSE_DEMO

#include <Arduino.h>

#include "params.h"
#include "shift.h"

#define DEFINED_ROUTINES 3

#define CLUSTER_DATA  255
#define CLUSTER_MAX 32
#define PIXEL_MAX   256

#define STEP_DELAY_1 240
#define STEP_DELAY_2 30

// EXTERNAL VARIABLES
  /**
  * Direct frame data
  * Pixels are represented by bits. The array can be transmitted directly to the LED drivers.
  */
  extern uint8_t g_frame[ROWC][COLC];

  /**
  * Full frame bitmap
  * Defines a byte for each pixel for grayscale data (future use).
  */
  extern uint8_t g_bitmap[ROWC][ROWC];

  //Pin definitions
  extern const int LATCH_PIN;
  extern const int OE_PIN;

/**
* DEMO
* Calling this function produces a predefined image sequence.
* Number of routines must be set in DEFINED_ROUTINES macro
*
* routine:    ID of requested routine.
*/
void demo(uint8_t routine);

/**
* TEST FUNCTION
* Writes the given data to the selected byte, and fills the rest of the array with zeroes
* 
* frame:      byte array, 16x2
* segment:    The selected byte of the frame array
* data:       Data to be written into the segment
*/
void fgen_cluster_picker(uint8_t (*frame)[COLC], uint8_t rows, uint8_t cols, int segment, uint8_t data);

/**
* TEST FUNCTION
* Writes 1 to the selected byte, and fills the rest of the array with zeroes
* 
* bitmap:     byte array, 16x16
* pixel:      The selected pixel in the bitmap array
*/
void fgen_pixel_picker(uint8_t (*bitmap)[ROWC], uint8_t rows, int pixel);

#endif //_DEMO_H_INCLUDED