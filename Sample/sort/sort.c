#include "sort.h"

int BubSort(int *v, int n)
{
  int i, j, temp;
  
  for (i = 0; i < n - 1; i++)
    {
      for (j = n - 1; j > i; j--)
	{
	  if (v[j - 1] > v[j])
	    {
	      temp = v[j];
	      v[j] = v[j - 1];
	      v[j - 1] = temp;
	    }
	}
    }

  return 0;
}

int QuickSort(int *v, int n)
{
  int i, j, min, max, piv = (int)(n / 2), temp;

  min = max = 0;

  for(i = 0; i < n; i++)
    if(min > v[i]) min = v[i];

  for(i = 0; i < n; i++)
    if(max < v[i]) max = v[i];

    while(1)
      {
	while(v[i] < piv) i++;
	while(piv < v[j]) j--;
	if(i >= j) break;

	temp = v[i];
	v[i] = v[j];
	v[j] = temp;

    if (left < i - 1) QuickSort(x, left, i - 1);
    if (j + 1 <  right) QuickSort(x, j + 1, right);
  }

  return 0;
}
