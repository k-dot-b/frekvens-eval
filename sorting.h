

#include <Arduino.h>
#include "shift.h"

#define N 16

extern const int LATCH_PIN;
extern const int OE_PIN;

//initializers
void generate();
void sorting();
void shuffle(uint8_t array[N]);
void reset();

//current sorting program
void quickSort(int, int);
int partition(int, int);


void bitMapGenerate();
void bitMapGenerInt(int array[N]);

//currently not in use
//void mergeSort(int, int);
