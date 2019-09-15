#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];

void
cat(int fd)
{
  int n, l, i;
  n = read(fd, buf, sizeof(buf));
  char * prev;
  char curr[100];
  prev = 0;
    for(i=0; i<n; i++){
      l=0;
      while(buf[i] != '\n' && i<n){
        curr[l] = buf[i];
        l++;
        i++;
      }
      curr[l] = '\0';
      if(!strcmp(prev,curr)){
        // prev and curr are same
        // do not print curr
      }else{
        // prev and curr are diff
        // print curr
        // update prev
        printf(1,"%s\n",curr);
        prev = strcpy(prev,curr);
      }
    }
    
    
  if(n < 0){
    printf(1, "cat: read error\n");
    exit();
  }
}

int
main(int argc, char *argv[])
{
   int fd,i;

  if(argc <= 1){
    cat(0);
    exit();
  }
 
  
  for(i = 1; i < argc; i++){
    if((fd = open(argv[i], 0)) < 0){
      printf(1, "cat: cannot open %s\n", argv[i]);
      exit();
    }
    cat(fd);
    close(fd);
  }
  
  exit();
}
