#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/select.h>
#include <sys/file.h>

#define LOCK -1
#define UNLOCK 1
#define getStatus(a) (a > 0) ? "UNLOCK" : "LOCK"
#define CHILDNM 3

/**
 * semaphore 
 */
union semunc { int val; struct semid_ds *buf; unsigned short *array; } arg;
int semSetting()
{
  int id;
  unsigned short vals[2] = { 0, 0 };

  if((id = semget(IPC_PRIVATE, CHILDNM, 0600)) == -1)
    { perror("semget()"); return -1; }

  arg.array = vals;
  if(semctl(id, 0, SETALL, arg) == -1)
    { perror("SETALL semctl()"); return -1; }

  return id;
}

int semClose(int id)
{
  if(semctl(id, 0, IPC_RMID, arg) == -1)
    { perror("IPC_RMID semctl()"); return -1; }

  return 0;
}

#define setSemopStatus(a, b) (a == b) ? UNLOCK : LOCK
int semLockEx(int semid, int id)
{
  int i;
  for(i = 0; i < CHILDNM; i++)
    {
      struct sembuf sops[1];
      sops[0].sem_num = i;
      sops[0].sem_op = setSemopStatus(id, i);
      sops[0].sem_flg = 0;
      if(semop(semid, sops, 1) == -1)
	{ perror("semLockEx semop()"); return -1; }
    }
  return 0;
}

int semUnlockEx(int semid)
{
  int i;
  for(i = 0; i < CHILDNM; i++)
    {
      struct sembuf sops[1];
      sops[0].sem_num = i;
      sops[0].sem_op = UNLOCK;
      sops[0].sem_flg = 0;
      if(semop(semid, sops, 1) == -1)
	{ perror("semUnlockEx semop()"); return -1; }
    }
  return 0;
}
/* semaphore end */

int distChildProcMesg(int semid, char *mesg)
{
  int fifodes;
  fifodes = open("fifo", O_WRONLY|O_NONBLOCK|O_CREAT);
  semLockEx(semid, 0);
  write(fifodes, mesg, strlen(mesg) +1);
  semUnlockEx(semid);
}

/**
 * fork
 */
void child(int semid, int semnm)
{
  int fifodes;
  int i;

  fifodes = open("fifo", O_RDONLY|O_NONBLOCK|O_CREAT);

  printf("process%d start\n", semnm);
  if(semnm == 0)
    {
      distChildProcMesg(semid, "child:0 messages");
    }

  else if(semnm == 1)
    {
      char buf[255];
      memset(buf, '\0', 255);
      read(fifodes, buf, 255);
      printf("%s\n", buf);
    }

  else if(semnm == 2)
    {
      char buf[255];
      memset(buf, '\0', 255);
      read(fifodes, buf, 255);
      printf("%s\n", buf);
    }
  
  printf("process%d end\n", semnm);
  exit(EXIT_SUCCESS);
}

void parent(int cnt)
{
  int i;
  for(i = 0; i < cnt; i++)
    wait(NULL);
}
/* fork end */


int main(int argc, char** argv)
{
  pid_t pid;
  int semid, i;

  if((semid = semSetting()) == -1)
    { fprintf(stderr, "semSetting()"); exit(EXIT_FAILURE); }

  for(i = 0; i < CHILDNM; i++)
    {
      if((pid = fork()) == -1)
	{ return -1; }

      else if(pid == 0)
	{ child(semid, i); }

      else
	{ parent(i); }
    }

  semClose(semid);

  return 0;
}
