#include <stdio.h>
#include <myth.h>
int main(int argc,char **argv)
{
  myth_init();
  printf("Hello, MassiveThreads!\n");
  myth_fini();
  return 0;
}

