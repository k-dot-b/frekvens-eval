
#include "sorting.h"


//lookup table
static const uint16_t columnHeight[] = {0x0001, 0x0003, 0x0007, 0x1f, 0x3f, 0x7f, 0xff, 0x1ff, 0x3ff, 
0x7ff, 0xfff, 0x1fff, 0x3fff, 0x7fff, 0xffff};

static const int N = 16;
//static const long RAND_MAX = 2047;


static uint8_t numbers[N];


void sorting()
{
  generate();
  // Merge sort
  mergeSort(0, N - 1);



  return 0;
}

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
        }

        while (i <= mid)
            tmp[k++] = numbers[i++];

        while (j <= top)
            tmp[k++] = numbers[j++];

        for (i = bottom, k = 0; i <= top; ++i, ++k)
            numbers[i] = tmp[k];
    }
}

void generate()
{

    for(int i=0; i<N; i++){
        numbers[i] = i;
    }

    shuffle(numbers);


}

void shuffle(uint8_t array[N])
{
    
    for (int i = 0; i < N - 1; i++) {
          uint8_t j = lowByte(i) + random(RAND_MAX) / (RAND_MAX / (N - i) + 1);
          uint8_t t = array[j];
          array[j] = array[i];
          array[i] = t;
    }
}

void bitMapGenerate(){
  
}



