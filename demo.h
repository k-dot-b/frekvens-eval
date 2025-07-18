/*
 * Copyright 2025 Benedek Kiss
 * Licensed under the EUPL
 *
 * demo.h
 */

/**
* Quick and dirty test patterns for Frekvens
*/
#ifndef _DEMO_H_INCLUDED
#define _DEMO_H_INCLUDED

#include <Arduino.h>

#include "frekvens_driver.h"

//-------------------------------------------------
// DEMO PARAMETERS
//Number of defined cases in demo function
#define DEFINED_ROUTINES 6

//Option for serial output
  //#define VERBOSE_DEMO
//Define this for interrupt-based demo sequence
  //#define DEMO_INTERRUPT
//The starting point of the multiDemo sequence
  #define FIRST_ROUTINE 3
//-------------------------------------------------

#define CLUSTER_DATA  255
#define CLUSTER_MAX 32
#define PIXEL_MAX   256

#define STEP_DELAY_1 240
#define STEP_DELAY_2 30

struct fadeObject {
  volatile uint8_t gray;
  uint8_t fade_prescaler;
  volatile int fade_cntr;
  volatile bool fade_reverse;
};

extern fadeObject demo_fade_image;

// EXTERNAL VARIABLES

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
* 
*/
void demoGrayscale();

/**
* TEST FUNCTION
* Writes 1 to the selected byte, and fills the rest of the array with zeroes
* 
* bitmap:     byte array, 16x16
* pixel:      The selected pixel in the bitmap array
*/
void fgen_pixel_picker(uint8_t (*bitmap)[FREKVENS_DIMC], uint8_t rows, int pixel);

#endif //_DEMO_H_INCLUDED