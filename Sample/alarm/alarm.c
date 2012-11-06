#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <ctype.h>

void sigHander(int signo)
{
  fprintf(stdout, "END\n");
  exit(EXIT_SUCCESS);
}

int setSignal(int sigtype)
{
  if(signal(sigtype, sigHander) == SIG_ERR)
    {
      perror("signal");
      return -1;
    }
  return 0;
}

void childProc(const int minute)
{
  alarm(minute * 60);
  fprintf(stdout, "START:%d minute\n", minute);
  for(;;)
    sleep(1);
  exit(EXIT_SUCCESS);
}

void parentProc(void)
{
  wait(NULL);
  exit(EXIT_SUCCESS);
}

int forkProc(int m, void (*child)(int), void (*parent)())
{
  pid_t pid;
  switch(pid = fork())
    {
    case -1:
      perror("fork");
      return -1;

    case 0:
      child(m);
      break;

    default:
      parent();
      break;
    }
  return 0;
}

int main(int argc, char **argv)
{
  int i;

  if(argc < 2)
    return -1;

  for(i = 0; i < strlen(argv[1]); i++)
    {
      if(isdigit(argv[1][i]) == 0)
	{
	  fprintf(stdout, "not number : %s\n", argv[1]);
	  return -1;
	}
    }

  if(setSignal(SIGALRM) == -1 ||
     setSignal(SIGABRT) == -1)
    {
      fprintf(stderr, "setSignal");
      return -1;
    }

  if(forkProc(atoi(argv[1]), childProc, parentProc) == -1)
    {
      fprintf(stderr, "forkProc");
      return -1;
    }

  return 0;
}
