#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
  char buf;	        //Stores standard input
  char *args[MAXARG];	//Stores new args to pass to exec
  int argsSize = 0;	//Stores size of args
  int n;		//Loop variable
  int pos = 0;		//Stores position of character we want to change
  char newarg[1024];	//Stores new argument

  if(argc < 2){		//Check that a command is passed to xargs
	printf("Xargs: xargs cmd [args]\n");
	exit();
  }

  //Pass argv[] arguments to args[]
  if( strcmp(argv[1],"-n") == 0){	//If newline optimization, get fourth argument
	n = 3;
  }
  else{
	n = 1;				//Otherwise get the second
  } 
  while(n < argc){			//Pass arguments
        args[argsSize] = argv[n];
        argsSize++;
        n++;
  }
  n = 0;
  while((n = read(0, &buf, 1) > 0)) {	//Read stdin to buf
    if (n < 0){				//Check we are reading
	printf("read error\n");	
    }
    if(buf == '\n') {			//Stop adding characters to newarg on newline
	newarg[pos] = '\0';		//end argument at newline position & pass to args
        args[argsSize] = newarg;	
	if(fork()==0){			//fork & call exec
	   exec(args[0],args);
	}
	else{				//wait if the child hasn't returned yet
	   wait();
        }
        pos = 0;			//reset the position for the next newline
    }
    else{				//Add characters to newarg from stdin			       
       newarg[pos] = buf;
       pos++;
    }		
  }
  exit();
}
