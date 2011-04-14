#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

extern char **getline(void);
int redir_in;
int redir_out;
int pipe_count;
int word_op;
int execute_now;
int error_found;

int adj_op_check(void){
   if(word_op){
      fprintf(stderr, "Syntax error: adjacent operators detected.\n");
      return 1;
   }
   return word_op++;
}

int op_check(char *arg){
   if(!(strcmp(arg, "<"))){
     if(adj_op_check())
        return 1;
     if(redir_in){
        fprintf(stderr, "Syntax error: detected two or more '<'\n");
        return 1;
     }
     redir_in++;
   }
   if(!(strcmp(arg, ">"))){
      if(adj_op_check())
         return 1;
      if(redir_in){
         fprintf(stderr, "Syntax error: detected two or more '>'\n");
         return 1;
      }
      redir_out++;
   }
   if(!(strcmp(arg, ")")) || !(strcmp(arg, "("))){
      fprintf(stderr, "Syntax error: parentheses not supported.");
      return 1;
   }
   if(!(strcmp(arg, "|"))){
      if(adj_op_check())
         return 1;
      pipe_count++;
   }
   if(!(strcmp(arg, ";")))
      execute_now = 1;
   word_op = 0;
   return 0;
}

int main(int argc, char *argv[], char *envp[]) {
  int i;
  char **args; 

  while(1) {
    word_op = pipe_count = redir_in = redir_out = execute_now = 0;
    printf("dShell$ ");
    args = getline();
    for(i = 0; args[i] != NULL; i++) {
      if(op_check(args[i]))
         break;
      printf("Argument %d: %s\n", i, args[i]);
    }
    if(error_found)
      continue;

  }
  return 0;
}
