#include<sys/ptrace.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/reg.h>

#include<syscall.h>

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#if __WORDSIZE == 64
#define REG(reg) reg.orig_rax
#else
#define REG(reg) reg.orig_eax
#endif

const int long_size = sizeof(long);

void getdata(pid_t child, long addr, char *str){
  char* laddr = str;
  int i = 0;
  union u{
    long val;
    char chars[long_size];
  } data;
  while (i <= 20){
    data.val = ptrace(PTRACE_PEEKDATA, child, addr+i*8, NULL);
    memcpy(laddr, data.chars, long_size);
    laddr += long_size;
    ++i;
  }
}
int main(int argc, char* argv[]){
  pid_t child;
  child = fork();
  if (child == 0){
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    execlp("cat", "cat", "./test", NULL);
  }
  else{
    long orig_rax;
    long params[3];
    int status;
    char *str;
    int toggle = 0;

    while (waitpid(child, &status, 0) && !WIFEXITED(status)){
      orig_rax = ptrace(PTRACE_PEEKUSER, child, 8*ORIG_RAX, NULL);
      if (orig_rax == SYS_access){
        if (toggle == 0){
          toggle = 1;
          params[0] = ptrace(PTRACE_PEEKUSER, child, 8*RDI, NULL);
          params[1] = ptrace(PTRACE_PEEKUSER, child, 8*RSI, NULL);
          params[2] = ptrace(PTRACE_PEEKUSER, child, 8*RDX, NULL);
          //printf("%ld   %ld   %ld   %ld\n", orig_rax, params[0], params[1], params[2]);
          str = (char*)calloc(100, sizeof(char));
          if (params[0] != 0) getdata(child, params[0], str);
          printf("get pathname: %s\n", str);
          free(str);
        } 
        else {
          toggle = 0;
        }
      }
      ptrace(PTRACE_SYSCALL, child, NULL, NULL);
    }
  } 
return 0;
}
