#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <lib.h>

int getallocmech(void){
   message m;
   return(_syscall(MM, 64, &m));
}
int setallocmech(int flag){
   message m;
   m.m1_i1 = flag;
   return(_syscall(MM, 65, &m));
}
int main(int argc, char *argv[]){
   int policy;
   if( argc = 0 || !argv[1][0]){
      printf("Usage: %s <type>\n0 - first fit\n1 - next fit\n2 - worst fit\n 3 - best fit\n4 - random fit");
      exit(1);
   }
   policy = atoi(argv[1]);
   printf("setallocmech(%d)\n", policy);
   setallocmech(policy);
   printf("getallochmech() = %d\n", getallocmech());
return 0;
}
