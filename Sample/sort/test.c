#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include "sort.h"

int AddNum(int *v, int n)
{
  int nm, i;
  srand((unsigned)time(NULL));

  for(i = 0; i < n; *(v + i) = rand() % n + 1, i++);

  return 0;
}

int DispNm(int *v, int n)
{
  int i;

  for(i = 0; i < n; i++)
    printf("%3d ", *(v + i));

  printf("\n");
  
  return 0;
}

int main(int argc, char **argv)
{
  int arrnm;
  int *val;

  if(argc < 2)
    {
      fprintf(stderr, "引数がたりん\n");
      return -1;
    }

  if(!isdigit(argv[1][0]))
    {
      fprintf(stderr, "引数が数字じゃない\n");
      return -1;
    }

  if(atoi(argv[1]) < 1)
    {
      fprintf(stderr, "引数が0か0より小さい\n");
      return -1;
    }

  val = (int *)malloc(sizeof(int) * atoi(argv[1]));
  arrnm = atoi(argv[1]);

  printf("バブルソート\n");
  AddNum(val, arrnm);
  printf("before:\n");
  DispNm(val, arrnm);

  printf("\n");

  BubSort(val, arrnm);
  printf("after:\n");
  DispNm(val, arrnm);

  printf("クイックソート\n");
  AddNum(val, arrnm);
  printf("before:\n");
  DispNm(val, arrnm);

  printf("\n");

  /* QuickSort(val, arrnm); */
  /* printf("after:\n"); */
  /* DispNm(val, arrnm); */

  free(val);

  return 0;
}
