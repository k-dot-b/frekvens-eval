/**
* Quick and dirty test patterns for Frekvens
*/
#ifndef _DEMO_H_INCLUDED
#define _DEMO_H_INCLUDED

#include <Arduino.h>

#include "shift.h"

//-------------------------------------------------
// DEMO PARAMETERS
//Number of defined cases in demo function
#define DEFINED_ROUTINES 5

//Option for serial output
  //#define VERBOSE_DEMO
//Define this for interrupt-based demo sequence
  //#define DEMO_INTERRUPT
//The starting point of the multiDemo sequence
  #define FIRST_ROUTINE 4
//-------------------------------------------------

#define CLUSTER_DATA  255
#define CLUSTER_MAX 32
#define PIXEL_MAX   256

#define STEP_DELAY_1 240
#define STEP_DELAY_2 30

// EXTERNAL VARIABLES

  /**
  * Full frame bitmap
  * Defines a byte for each pixel for grayscale data (future use).
  */
  extern uint8_t g_bitmap[DIMC][DIMC];

  //Pin definitions
  extern const int LATCH_PIN;
  extern const int OE_PIN;

/**
* Calling this function produces a predefined image sequence.
*
* routine:    ID of requested routine. Write 0 to skip.
*/
void demo(uint8_t routine);

/**
* Iterate through multiple demo routines.
* Number of routines must be set correctly in DEFINED_ROUTINES macro
*/
void multiDemo();

/**
* Call this from a timer interrupt
*/
void demoInterrupt();

/**
* TEST FUNCTION
* Writes the given data to the selected byte, and fills the rest of the array with zeroes
* 
* frame:      byte array, 16x2
* segment:    The selected byte of the frame array
* data:       Data to be written into the segment
*/
void fgen_cluster_picker(uint8_t (*frame)[COLB], uint8_t rows, uint8_t cols, int segment, uint8_t data);

/**
* TEST FUNCTION
* Writes 1 to the selected byte, and fills the rest of the array with zeroes
* 
* bitmap:     byte array, 16x16
* pixel:      The selected pixel in the bitmap array
*/
void fgen_pixel_picker(uint8_t (*bitmap)[DIMC], uint8_t rows, int pixel);

#endif //_DEMO_H_INCLUDED