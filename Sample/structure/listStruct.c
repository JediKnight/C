#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#undef BUFSIZ
#define BUFSIZ 255

struct _list {
  char fname[BUFSIZ];
  char lname[BUFSIZ];
  int age;
  struct _list *prev;
  struct _list *next;
};

#define clear(a)				\
  memset(a, '\0', BUFSIZ)

void getChar(char *buf)
{
  char *p;
  fgets(buf, BUFSIZ, stdin);
  if((p = strchr(buf, '\n')) != NULL)
    *p = '\0';
}

struct _list *getNewList()
{
  struct _list *p;
  p = (struct _list *)malloc(sizeof(struct _list));
  memset(p, 0x00, sizeof(struct _list));
  return p;
}

struct _list *addList(char *fn, char *ln, int ag, struct _list *prev)
{
  struct _list *p, *next;

  p = getNewList();
  next = getNewList();

  strncpy(p->fname, fn, BUFSIZ);
  strncpy(p->lname, ln, BUFSIZ);
  p->age = ag;

  p->prev = prev;
  p->next = next;
  prev->next = p;
  next->prev = p;
  return p;
}

struct _list *seekHead(struct _list *list)
{
  for(; list->prev != NULL; list = list->prev);
  return list->next;
}

struct _list *seekTail(struct _list *list)
{
  for(; list->next != NULL; list = list->next);
  return list->prev;
}

struct _list *seekIndex(struct _list *list, int index)
{
  int i;
  list = seekHead(list);
  for(i = 0; i < index && list->next != NULL; list = list->next, i++);
  return list;
}

int getListNumber(struct _list *list)
{
  int cnt;
  list = seekHead(list);
  for(cnt = 0; list->next != NULL; list = list->next, cnt++);
  return cnt;
}

void dispAll(struct _list *list)
{
  int i;
  list = seekHead(list);
  for(i = 0; list->next != NULL; list = list->next, i++)
    printf("index %d   :%s, %d\n", i, list->fname, list->age);
}

void freeList(struct _list *list)
{
  list = seekHead(list);
  for(; list->next != NULL; list = list->next)
    free(list);
}

int main()
{
  struct _list *list;
  int i;

  list = getNewList();

  for(i = 0;i < 3; i++)
    {
      char fn[BUFSIZ], ln[BUFSIZ], ag[BUFSIZ];

      printf("名前を入力> ");
      getChar(fn);

      /* printf("苗字を入力> "); */
      /* getChar(ln); */

      printf("年齢を入力> ");
      getChar(ag);

      list = addList(fn, ln, atoi(ag), list);
    }

  list = seekHead(list);
  printf("seekHead  :%s, %d\n", list->fname, list->age);

  list = seekTail(list);
  printf("seekTail  :%s, %d\n", list->fname, list->age);

  list = seekIndex(list, 1);
  printf("seekIndex :%s, %d\n", list->fname, list->age);

  dispAll(list);

  freeList(list);
  return 0;
}
