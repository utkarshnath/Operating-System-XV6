#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];

void
printString1(const char * p){
  printf(1,"printstring called ");
  if(p==0){
    printf(1,"P==0");
    return;
  }
  while(*p){
    printf(1,"%c",*p);
    p++;
  }
  printf(1,"\n");
}


void
unic(int fd,int typec,int typed,int typei)
{
  int n, l, i,same;
  n = read(fd, buf, sizeof(buf));
  char prev[100];
  char curr[100];
  int firstWord;
  firstWord = 1;
  same = 0;
    for(i=0; i<n; i++){
      l=0;
      while(buf[i] != '\n' && i<n){
        curr[l] = buf[i];
        l++;
        i++;
      }
      curr[l] = '\0';
      if(firstWord){
        strcpy(prev,curr);
        firstWord = 0;
        same++;
        continue;
      }
      if(!strcmp(prev,curr)){
        // prev and curr are same
        // do not print curr
        same++;
      }else{
        // prev and curr are diff
        // print curr
        // update prev
        if(typec){
          printf(1,"%d %s\n",same,prev);
        }else{
          printf(1,"%s\n",prev);
        }
        
        same = 1;
        strcpy(prev,curr);
      }
    }
    if(typec){
          printf(1,"%d %s\n",same,prev);
    }else{
          printf(1,"%s\n",prev);
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

   for (int i = 0; i < argc; ++i)
   {
     printf(1,"%s\n",argv[i]);
   }

  if(argc <= 1){
    unic(0,0,0,0);
    exit();
  }
 


  int typec,typed,typei;
  typec = typed = typei = 0;
  for(i = 1; i < argc; i++){
    if(!strcmp(argv[i],"-c")){
        typec = 1;
    }
    else if(!strcmp(argv[i],"-d")){
      typed = 1;
    }
    else if(!strcmp(argv[i],"-i")){
      typei = 1;
    }
    else{
      if((fd = open(argv[i], 0)) < 0){
        printf(1, "cat: cannot open %s\n", argv[i]);
        exit();
      }
      unic(fd,typec,typed,typei);
      close(fd);
    }
  }
  
  exit();
}
