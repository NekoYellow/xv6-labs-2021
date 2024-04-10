#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

#define MAX_ARG_LEN 512

void
copy_argv(char **ori_argv, int ori_argc, char *new_argv, char **argv)
{
  int k = 0;
  for (int i = 0; i < ori_argc; i++) {
    argv[k] = malloc(strlen(ori_argv[i]) + 1);
    memcpy(argv[k++], ori_argv[i], strlen(ori_argv[i]) + 1);
  }
  argv[k] = malloc(strlen(new_argv) + 1);
  memcpy(argv[k++], new_argv, strlen(new_argv) + 1);
}


void
print(char **s, int n)
{
  for (int i = 0; i < n; i++) {
    printf("%s\n", s[i]);
  }
}

int 
main(int argc, char *argv[])
{
  if (argc <= 1) {
    fprintf(2, "Usage: xargx command [arg ...]\n");
    exit(1);
  }

  char param[MAX_ARG_LEN];
  int i = 0;
  char ch;
  int ignore = 0;
  while (read(0, &ch, 1) > 0) {
    if (ch == '\n') {
      if (ignore) {
        i = 0;
        ignore = 0;
        continue;
      }
      param[i] = 0;
      i = 0;

      int pid = fork();
      if (pid == 0) {
        //child
        int cmd_argc = argc;
        
        char *cmd_argv[MAXARG];

        copy_argv(argv + 1, argc - 1, param, cmd_argv);
        cmd_argv[cmd_argc] = 0;
        
        exec(cmd_argv[0], cmd_argv);

        exit(0);
      } else {
        wait((int *)0);
      }
      
    } else {
      
      if (!ignore && i >= MAX_ARG_LEN - 1) {
        printf("xargs: too long arguments...\n");
        ignore = 1;
      }

      if (!ignore) {
        param[i++] = ch;
      }
    }
  }

  exit(0);
}


// int MAXARGLEN = 128;

// int readline(int nargc, char *nargv[MAXARG])
// {
//   int i = 0;
//   while(read(0, nargv[nargc]+i, 1)){
//     if(i == MAXARGLEN-1){
//       fprintf(2, "Argument is too long\n");
//       exit(1);
//     }
//     if(nargv[nargc][i] == '\n'){
//       if(!i)
//         return nargc;
//       nargv[nargc++][i] = '\0';
//       break;
//     }
//     else if(nargv[nargc][i] == ' '){
//       nargv[nargc++][i] = '\0';
//       if (nargc == MAXARG)
//         break;
//       i = 0;
//     }
//     else
//       i++;
//   }
//   return nargc;
// }

// int main(int argc, char const *argv[])
// {
//   int nargc;
//   char *nargv[MAXARG];

//   if(argc < 2){
//     fprintf(2, "Usage: xarg cmd (arg ...)\n");
//     exit(1);
//   }

//   for(nargc = 1; nargc <= MAXARG; nargc++){
//     if (nargc < argc){
//       nargv[nargc-1] = malloc(strlen(argv[nargc])+1);
//       strcpy(nargv[nargc-1], argv[nargc]);
//     } else{
//       nargv[nargc-1] = malloc(MAXARGLEN);
//     }
//   }
//   nargc--;

//   while(1){
//     if(readline(nargc, nargv) == nargc)
//       break;
//     if(fork() == 0)
//       exec(nargv[0], nargv);
//   }
//   exit(0);
// }