int fork(void (*clild)(), void (*parent)())
{
  pid_t pid;
  switch(pid = fork())
    {
    case -1:
      perror("fork");
      return -1;

    case 0:
      child();

    default:
      parent();
    }

  return 0;
}
