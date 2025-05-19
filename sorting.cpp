
#include "sorting.h"


//lookup table
/*
static const uint16_t columnHeight[] = {0x0001, 0x0003, 0x0007, 0x1f, 0x3f, 0x7f, 0xff, 0x1ff, 0x3ff, 
0x7ff, 0xfff, 0x1fff, 0x3fff, 0x7fff, 0xffff};
*/

//static const long RAND_MAX = 2047;


static uint8_t numbers[N];
static const int SORT_STEP_DELAY = 200;


void sorting()
{
  generate();
  delay(2000);

  quickSort(0, N-1);

  reset();

  return 0;
}

//mergesort implementation
/*
void mergeSort(int bottom, int top)
{
    if (bottom < top)
    {
        int mid = (bottom + top) / 2;
        mergeSort(bottom, mid);
        mergeSort(mid + 1, top);

        int i = bottom, j = mid + 1, k = 0;
        int tmp[N];

        while (i <= mid && j <= top)
        {
            if (numbers[i] <= numbers[j])
                tmp[k++] = numbers[i++];
            else
                tmp[k++] = numbers[j++];
            //bitMapGenerate();
            bitMapGenerInt(tmp);
            mrefresh(g_bitmap, DIMC, 8, LATCH_PIN, OE_PIN);
            delay(SORT_STEP_DELAY);
        }

        while (i <= mid)
            tmp[k++] = numbers[i++];
            bitMapGenerate();
            //bitMapGenerInt(tmp);
            mrefresh(g_bitmap, DIMC, 8, LATCH_PIN, OE_PIN);
            delay(SORT_STEP_DELAY);

        while (j <= top)
            tmp[k++] = numbers[j++];
            bitMapGenerate();
            //bitMapGenerInt(tmp);
            mrefresh(g_bitmap, DIMC, 8, LATCH_PIN, OE_PIN);
            delay(SORT_STEP_DELAY);

        for (i = bottom, k = 0; i <= top; ++i, ++k)
            numbers[i] = tmp[k];
            bitMapGenerate();
            //bitMapGenerInt(tmp);
            mrefresh(g_bitmap, DIMC, 8, LATCH_PIN, OE_PIN);
            delay(SORT_STEP_DELAY);
    }
}
*/
//quicksort implementation

int temp;
void quickSort(int bottom, int top)
{
    if (bottom < top)
    {
        int p = partition(bottom, top);
        quickSort(bottom, p - 1);
        quickSort(p + 1, top);
    }
}

int partition(int bottom, int top)
{
    int key = numbers[top];
    int i = bottom - 1;
    
    for (int j = bottom; j < top; j++)
    {
        if (numbers[j] <= key)
        {
            i++;
            temp = numbers[i];
            numbers[i] = numbers[j];
            numbers[j] = temp;
            //swap(numbers[i], numbers[j]);

            bitMapGenerate();
            mrefresh(g_bitmap, DIMC, 8, LATCH_PIN, OE_PIN);
            delay(SORT_STEP_DELAY);
            
        }
    }
    //swap(numbers[i + 1], numbers[top]);
    temp = numbers[i + 1];
    numbers[i + 1] = numbers[top];
    numbers[top] = temp;

    bitMapGenerate();
    mrefresh(g_bitmap, DIMC, 8, LATCH_PIN, OE_PIN);
    delay(SORT_STEP_DELAY);
    return i + 1;
}

//--------------------------


void generate()
{

    for(int i=0; i<N; i++){
        numbers[i] = i;
        bitMapGenerate();
          mrefresh(g_bitmap, DIMC, 8, LATCH_PIN, OE_PIN);
          delay(100);
    }

    shuffle(numbers);


}

void shuffle(uint8_t array[N])
{
    
    for (int i = 0; i < N; i++) {  //removed n-1
          uint8_t j = lowByte(i) + random(RAND_MAX) / (RAND_MAX / (N - i) + 1);
          uint8_t t = array[j];
          array[j] = array[i];
          array[i] = t;
            bitMapGenerate();
            mrefresh(g_bitmap, DIMC, 8, LATCH_PIN, OE_PIN);
            delay(100);
    }
}

void reset(){
  for(int i=0; i<N; i++){
        numbers[i] = 15;
        bitMapGenerate();
        mrefresh(g_bitmap, DIMC, 8, LATCH_PIN, OE_PIN);
        delay(100);
}
}

void bitMapGenerate(){

  for(int i=0; i<N; i++){   //column
    for(int j=0; j<N; j++){ //row 
        if(j < 15-numbers[i]){
            g_bitmap[j][i] = 0;
        } else g_bitmap[j][i] = 1;
    }
  }
}

void bitMapGenerInt(int array[N]){

  for(int i=0; i<N; i++){   //column
    for(int j=0; j<N; j++){ //row 
        if(j < 15-array[i]){
            g_bitmap[i][j] = 0;
        } else g_bitmap[i][j] = 1;
    }
  }
}



