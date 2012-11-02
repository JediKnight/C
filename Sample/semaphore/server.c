#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define SUM_MAX 10
#define SUM_MIN 0
#define SUM_INIT 5
#define SHM_SZ (sizeof(int))

void sync_sum(int semaphore, int *shared_data, int step);

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *__buf;
} semunion;

int main(void)
{
  int semaphore;
  int *shared_data;
  int shm_id;
  int pid;
  int status;

  errno = 0;
  shm_id = shmget((key_t)1111, SHM_SZ, 0666 | IPC_CREAT);
  if(shm_id == -1){
    perror("shmget failure");
    exit(EXIT_FAILURE);
  }

  errno = 0;
  shared_data = (int *)shmat(shm_id, 0, 0);
  if(shared_data == -1){
    perror("shmat failure");
    exit(EXIT_FAILURE);
  }

  errno = 0;
  semaphore = semget((key_t)1111, 1, 0666 | IPC_CREAT);
  if(semaphore == -1){
    perror("semget failure");
    goto FINALIZE;
  }

  else
  {
    union semun semunion;
    errno = 0;
    semunion.val = 1;
    if(semctl(semaphore, 0, SETVAL, semunion) == -1){
      perror("semctl(init) failure");
      goto FINALIZE;
    }
  }

  *shared_data = SUM_INIT;

  errno = 0;
  pid = fork();
  switch(pid){
  case 0:
    sync_sum(semaphore, shared_data, -1);
    break;
  case -1:
    perror("fork failure");
    exit(EXIT_FAILURE);
    break;
  default:
    sync_sum(semaphore, shared_data, 1);
    wait(&status);
    break;
  }


 FINALIZE:
  errno = 0;
  if(shmdt(shared_data) == -1){
    perror("shmdt failure");
  }

  if(pid != 0){
    {
      struct shmid_ds {
	struct    ipc_perm shm_perm;  /* operation perms */
	int  shm_segsz;          /* size of segment (bytes) */
	time_t    shm_atime;          /* last attach time */
	time_t    shm_dtime;          /* last detach time */
	time_t    shm_ctime;          /* last change time */
	unsigned short shm_cpid; /* pid of creator */
	unsigned short shm_lpid; /* pid of last operator */
	short     shm_nattch;         /* no. of current attaches */
	unsigned short   shm_npages;  /* size of segment (pages) */
	unsigned long   *shm_pages;
	struct shm_desc *attaches;    /* descriptors for attaches */
      } buf;

      errno = 0;
      if(shmctl(shm_id, IPC_RMID, &buf) == -1){
	perror("delete shared memory failure");
      }
    }

    errno = 0;
    if(semctl(semaphore, 0, IPC_RMID, NULL) == -1){
      perror("delete semaphore failure");
      exit(EXIT_FAILURE);
    }
  }

  exit(EXIT_SUCCESS);
}

void sync_sum(int semaphore, int *shared_data, int step){
  short run_flag = 1;
  int i = 0;
  struct sembuf semb;
  semb.sem_num = 0;
  semb.sem_op = -1;
  semb.sem_flg = SEM_UNDO;

  srand(getpid());

  while(run_flag){
#ifdef SYNC
    errno = 0;
    if(semop(semaphore, &semb, 1) == -1){
      perror("lock failure");
      run_flag = 0;
    }else{
#endif
      if(*shared_data < SUM_MAX && *shared_data > SUM_MIN){
	(*shared_data) += step;
	fprintf(stdout,"%d:\t",getpid());
	for(i = 0; i < *shared_data; i++){
	  fprintf(stdout,"*");
	}
	fprintf(stdout,"\t(%d)\n",*shared_data);
	fflush(stdout);
      }else{
	run_flag = 0;
      }
#ifdef SYNC
    }
    semb.sem_op = 1;
    if(semop(semaphore, &semb, 1) == -1){
      perror("unlock failure");
      run_flag = 0;
    }
#endif
    sleep(rand() % 3);
  }
}
