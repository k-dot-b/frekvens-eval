#include <Arduino.h>
#include "shift.h"

#define M 6

extern const int LATCH_PIN;
extern const int OE_PIN;

//initializers
void initialize();
void graphPosition();
void graph_visualisation();

//algorythm
void dijkstra();
void match(int, int);




//currently not in use

