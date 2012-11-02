#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/sem.h>
#define LOCK -1
#define UNLOCK 1
#define CHILD1 0
#define CHILD2 1

void MySemop(int p_semid, int p_semnum, int p_op)
{
  struct sembuf    sops[1];

  sops[0].sem_num = p_semnum;
  sops[0].sem_op = p_op;
  sops[0].sem_flg = 0;

  if (semop(p_semid, sops, 1) == -1) {
    perror("MySemop ");
    exit(EXIT_FAILURE);
  }

  return;
}

char *file = "./sem.txt";
FILE *fp;
void child1(int semid)
{
  int line_cnt;
  printf("子プロセス１開始\n");

  if((fp = fopen(file, "w")) == NULL)
    {
      perror("子プロセス１ ");
      exit(EXIT_FAILURE);
    }
  for(line_cnt = 1; line_cnt <= 5; ++line_cnt) 
    {
      fprintf(fp, "子プロセス１のメッセージ%d\n", line_cnt);
      sleep(1);
    }
  fclose(fp);
  
  MySemop(semid, CHILD2, UNLOCK);
  MySemop(semid, CHILD1, LOCK);
  
  printf("子プロセス１終了\n");
  exit(EXIT_SUCCESS);
}

void child2(int semid)
{
  char buff[1024];
  printf("子プロセス２開始\n");
  
  MySemop(semid, CHILD2, LOCK);
  MySemop(semid, CHILD1, UNLOCK);
  
  if((fp = fopen(file, "r")) == NULL)
    {
      perror("子プロセス２ ");
      exit(EXIT_FAILURE);
    }
  printf("子プロセス２：ファイルの内容を表示します\n");
  while(fgets(buff, 1024, fp) != NULL)
    {
      printf("%s", buff);
    }
  fclose(fp);
  
  printf("子プロセス２終了\n");
  exit(EXIT_SUCCESS);
}

int main()
{
  int semid;
  union semun { int val; struct semid_ds *buf; unsigned short *array; } ctl_arg;
  unsigned short  vals[2] = {0, 0};
  int child_cnt;

  if ((semid = semget(IPC_PRIVATE, 2, 0600)) == -1){
    perror("main : semget ");
    exit(EXIT_FAILURE);
  }

  ctl_arg.array = vals;
  if (semctl(semid, 0, SETALL, ctl_arg) == -1){
    perror("main : semctl ");
    exit(EXIT_FAILURE);
  }

  if (fork() == 0)
    {
      child1(semid);
    }

  if (fork() == 0)
    {
      child2(semid);
    }

  for (child_cnt = 0; child_cnt < 2; ++child_cnt) {
    wait(NULL);
  }

  if (semctl(semid, 0, IPC_RMID, ctl_arg) == -1){
    perror("main : semctl ");
    exit(EXIT_FAILURE);
  }

  printf("親プロセス終了\n");
  return EXIT_SUCCESS;
}
