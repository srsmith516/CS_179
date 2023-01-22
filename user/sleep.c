#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i;	//Time the OS will sleep
  
  if(argc <= 1){	//Catch if no arguments
    printf("Please give an argument for the number of cycles to sleep\n");
    exit();
  }
  
  i = atoi(argv[1]);	//The OS will sleep for the amount of time passed to sleep
  sleep(i);		//syscall sleep
  exit();
}
