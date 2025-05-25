#include "graph_visualisation.h"


using namespace std;

static int mins[M];
static int path[M];
static bool done[M];

//graph
static int nodeCoordinates[M][2];
static int graph[M][M] = {
    { 0, 7, 9, 0, 0,14}, //0   
    { 7, 0,10,15, 0, 0}, //1   
    { 9,10, 0,11, 0, 2}, //2   
    { 0,15,11, 0, 6, 0}, //3   
    { 0, 0, 0, 6, 0, 9}, //4   
    {14, 0, 2, 0, 9, 0}  //5   
//    0  1  2  3  4  5
};





void graph_visualisation() {
  //necesserry values
  int start = 0; //starting point
  int dest = 4; //destination

  int current = start;

  mins[start] = 0; 
  int minVal = 9999; 
  int nextIndex = 0;


    graphPosition();
    dijkstra();
    match(start, dest);
}


void dijkstra() 
{
  initialize();

while (true) {
    int current = -1;
    
    for (int i = 0; i < M; i++) {
        if (!done[i] && (current == -1 || mins[i] < mins[current])) {
            current = i;
        }
    }
    

    if (current == -1 || mins[current] == 99999) break;
    done[current] = true;


    for (int j = 0; j < M; j++) {
        if (graph[current][j] != 0 && !done[j]) {
            int newDistance = mins[current] + graph[current][j];
            if (newDistance < mins[j]) {
                mins[j] = newDistance;
                path[j] = current;
            }
        }
    }
}
/*
for(int i=0; i<M; i++){
    Serial.println(path[i]);
}
*/

}

//random ahh initializer
void initialize(){
    for(int i = 0; i < M; i++)
    {
        done[i] = false;
		mins[i] = 99999;
		path[i] = -1;
    }
}

//generating coordinates for graph nodes and lighting up nodes
void graphPosition(){
    //traverse and generate coordinates for graph nodes
    for(int i=0; i<M; i++){
        for(int j=0; j<2; j++){
            nodeCoordinates[i][j] = random(16);
            //Serial.println(nodeCoordinates[i][j]);
        }
    }

    //light up the nodes
    for(int i=0; i<M; i++){
        g_bitmap[nodeCoordinates[i][0]][nodeCoordinates[i][1]] = 1;
    }
    mrefresh(g_bitmap, DIMC, 8, LATCH_PIN, OE_PIN);
}


//match path with node coordinates
void match(int start, int dest){
    int v = dest;
    while(v != start && v != -1)
    {
        g_bitmap[nodeCoordinates[v][0]][nodeCoordinates[v][1]] = 0;
        mrefresh(g_bitmap, DIMC, 8, LATCH_PIN, OE_PIN);
        delay(300);
        g_bitmap[nodeCoordinates[v][0]][nodeCoordinates[v][1]] = 1;
        mrefresh(g_bitmap, DIMC, 8, LATCH_PIN, OE_PIN);

        
        Serial.println(v);
        v = path[v];
    }
    
    if(v != -1)
    {
        g_bitmap[nodeCoordinates[v][0]][nodeCoordinates[v][1]] = 0;
        mrefresh(g_bitmap, DIMC, 8, LATCH_PIN, OE_PIN);
        delay(300);
        g_bitmap[nodeCoordinates[v][0]][nodeCoordinates[v][1]] = 1;
        mrefresh(g_bitmap, DIMC, 8, LATCH_PIN, OE_PIN);
    }


/*
    for(int i=0; i<M; i++){
        for(int j=0; j<M; j++){
            Serial.println("nega what");
            Serial.println(i);
            Serial.println(path[j]);
            if(i != path[j]){
                g_bitmap[nodeCoordinates[i][0]][nodeCoordinates[i][1]] = 0;
                mrefresh(g_bitmap, DIMC, 8, LATCH_PIN, OE_PIN);
                delay(300);
            }
        }
    }
*/
    
}


//adjust to matrix
/*
void bitMapGenerate(){
    //matrix traverse
    for(int i=0; i<16; i++){
        for(int j=0; j<16; j++){

        }
    }
}
*/