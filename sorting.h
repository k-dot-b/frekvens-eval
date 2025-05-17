

#include <Arduino.h>
#include "shift.h"



extern const int LATCH_PIN;
extern const int OE_PIN;

void generate();
void sorting();
void shuffle(uint8_t array[16]);
void mergeSort(int, int);
void bitMapGenerate();
