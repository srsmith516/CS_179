#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


//Returns the name of a path
char*
fmtname(char *path)
{
  char *p;
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  return p;
}

void _find(char *path, char *name){
  char buf[512], *p;		//Buffer for Part1, temporarily stores the working
  char buf2[512], *p2;		//Buffer for Part2
  int fd;			//File Descriptor for Part1
  int fd2;			//File Descriptor for Part2
  struct dirent de;		//Directory struct for Part1
  struct dirent de2;		//Directory struct for Part2
  struct stat st;		//stat structure
  	
  if((fd = open(path, 0)) < 0){	//Open file specified by the path & catch error
    fprintf(2, "Cannot open %s\n", path);
    return;
  }
  if(fstat(fd, &st) < 0){	//Stat the file & catch error	
    fprintf(2, "Cannot stat %s\n", path);
    close(fd);
    return;
  }
  if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){	//Check that the path is not too long	
      printf("Path too long\n");
      return;
  }
    //Copy the path to buffer and add a '/'
    strcpy(buf, path);					
    p = buf+strlen(buf);			
    *p++ = '/';
	
    //Part 1 : Step through the directory and print out paths that match the given
    while(read(fd, &de, sizeof(de)) == sizeof(de)){	
      if(de.inum == 0)								
        continue;
      memmove(p, de.name, DIRSIZ); //move the name of the file in directory into buf					
      p[DIRSIZ] = 0;								
      if(stat(buf, &st) < 0){	//stat the file de points to						
        printf("Cannot stat %s\n", buf);
        continue;
      }
     if(strcmp(fmtname(buf),name)==0){	//if the names are the same, print
                printf("%s\n", buf);
       }
    }

    //Close the directory, re-open the file, and copy the path to buffer again
    //Similar to just before part 1
    close(fd);
    fd2 = open(path,0);     
    strcpy(buf2, path);
    p2 = buf2+strlen(buf2);
    *p2++ = '/';
   
   //Part 2 : Step through the directory and recurse if we find a sub-directory 
   while(read(fd2, &de2, sizeof(de2)) == sizeof(de2)){
      if(de2.inum == 0)
        continue;
      memmove(p2, de2.name, DIRSIZ);
      p2[DIRSIZ] = 0;
      if(stat(buf2, &st) < 0){
        printf("Cannot stat %s\n", buf2);
        continue;
      }
     //Recurse into a file if it is of type directory, but not if its name is "." or ".."
     if((st.type == T_DIR)&&(strcmp(fmtname(buf2),".")!=0)&&(strcmp(fmtname(buf2),"..")!=0)){
        _find(buf2, name);
      }
    }

    close(fd);
	return;
}

int main(int argc, char *argv[]){
 
	if(argc <=2){	//Check for correct argumets
		printf("Please give a directory and filename\n");
		exit();
	}
	_find(argv[1],argv[2]);	//Call find
	exit();
}
