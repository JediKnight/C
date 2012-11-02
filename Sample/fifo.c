#include <stdio.h>

#define ARRAYNUM 255

int initArray(int *array)
{
  int i;
  for(i = 0; i < ARRAYNUM; array[i] = -1, i++);
  return 0;
}

int seekIndex(int *array)
{
  int i;
  for(i = 0; i < ARRAYNUM; i++)
    {
      if(array[i] == -1)
	return i;
    }

  return -1;
}

int shiftIndex(int *array, int index, int shiftNum)
{
  int i;
  for(i = index; i < index + shiftNum; i++)
    {
      array[i] = array[i + shiftNum];
      array[i + shiftNum] = -1;
    }
      
  return 0;
}

int dispValue(int *array)
{
  int i;
  for(i = 0; i < ARRAYNUM; i++)
    printf("%d ", array[i]);

  printf("\n");
  return 0;
}

int main()
{
  int fifo[255], index, i, j;

  initArray(fifo);

  puts("初期値");
  dispValue(fifo);
  printf("\n");

  puts("1 ~ 10まで格納");
  index = seekIndex(fifo);
  for(i = index, j = 1; i < index + 10; i++, j++)
    { fifo[i] = j; }
  puts("1 ~ 10を格納した結果");
  dispValue(fifo);
  printf("\n");

  puts("11 ~ 20まで格納");
  index = seekIndex(fifo);
  for(i = index, j = 11; i < index + 10; i++, j++)
    { fifo[i] = j; }
  puts("11 ~ 20を格納した結果");
  dispValue(fifo);
  printf("\n");

  puts("index 0 ~ 9を取り出す");
  for(i = 0; i < 10; i++)
    {
      printf("%d ", fifo[i]);
    }

  shiftIndex(fifo, 0, 10);

  printf("\n");

  puts("index 0 ~ 9を取り出した結果");
  dispValue(fifo);

  puts("100 ~ 199まで格納");
  index = seekIndex(fifo);
  for(i = index, j = 100; i < index + 100; i++, j++)
    {
      fifo[i] = j;
    }

  puts("100 ~ 200を格納した結果");
  dispValue(fifo);

  return 0;
}
