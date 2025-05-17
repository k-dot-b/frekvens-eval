

#include <Arduino.h>
#include "shift.h"

#define N 16

extern const int LATCH_PIN;
extern const int OE_PIN;

void generate();
void sorting();
void shuffle(uint8_t array[N]);
void mergeSort(int, int);
void bitMapGenerate();
void bitMapGenerInt(int array[N]);
