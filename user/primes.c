#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int h = 35;

int
main(int argc, char const *argv[])
{
  int p[2];
  int n;
  int buf[h];
  for(n = 0; n < h; n++) buf[n] = n+2;

  while(n){
    pipe(p);
    if(fork() == 0){
      close(p[1]);
      read(p[0], buf, 4*n);
      close(p[0]);
      int prime = buf[0], nn = 0;
      for(int i = 1; i < n; i++){
        if(buf[i]%prime) buf[nn++] = buf[i];
      }
      n = nn;
      printf("prime %d\n", prime);
    } else{
      close(p[0]);
      write(p[1], buf, 4*n);
      // close(p[1]);
      wait(0);
      break;
    }
  }

  exit(0);
}
