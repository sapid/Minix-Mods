%{
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>

int _numargs = 200;
char **_args;
extern int *cmds;
int _argcount = 0;
extern int cmd_count;
extern int pipe_count;
int _wordop = 0;
int operror = 0;
int redir_in;
int redir_out;
void add_arg(void);
void new_word(void);

/* Notes for new_op:
-1 EOL
-2 ;
-10 <
-11 >
-12 |
*/
void new_op(int);
void new_semicolon(void);
%}
%option noyywrap
%option 8bit
%option noinput
%option nounput
%option verbose

WORD [a-zA-Z0-9\/\.-]+
BADOP [()]

%%
  _argcount = _wordop = cmd_count = pipe_count = operror = 0; 
  redir_in = redir_out = 0;
exit                 {  if(!_wordop)
                           exit(1); /* Kludge. */
                        new_word();
                        add_arg();
                     }
{WORD}               {
                        new_word();
                        add_arg();
                     }
[>]                  {
                        new_op(-11);
                        add_arg();
                        if(redir_out){
                           fprintf(stderr, "Syntax error: detected two or more '>'\n");
                           operror = 1;
                        } else redir_out = 1;
                     }
[<]                  {
                        new_op(-10);
                        add_arg();
                        if(pipe_count){
                           fprintf(stderr, "Syntax error: detected pipe before '<'.\n");
                           operror = 1;
                        }
                        if(redir_in){
                           fprintf(stderr, "Syntax error: detected two or more '<'\n");
                           operror = 1;
                        } else redir_in = 1;
                     }
[|]                  {
                        new_op(-12);
                        add_arg();
                        pipe_count++;
                        if(redir_out){
                           fprintf(stderr,"Syntax error: detected pipe after '>'.\n");
                           operror = 1;
                        }
                     }
[;]                  {
                        new_semicolon();
                        add_arg();
                     }
{BADOP}              {  operror = 1; 
                        fprintf(stderr, "Syntax error: parentheses not supported.");
                     }


\n                    {cmds[cmd_count + 1] = -1; return 1; }

[ \t]+               {}

.                    {}

%%

char **getline(void) { return (char **)yylex(); }

void add_arg(void){
   if(_argcount < _numargs - 1) {
      _args[_argcount++] = (char*)strdup(yytext);
      _args[_argcount] = NULL;
   }
}

void new_op(int sigil){
   if(_wordop){
      _wordop = 0;
      _args[_argcount++] = NULL;
      cmds[cmd_count++] = sigil;
      cmds[cmd_count] = -1;
   } else {
      fprintf(stderr, "Syntax error: adjacent operators detected.\n");
      operror = 1;
   }
}

void new_word(void){
   if(!_wordop){
      _wordop = 1;
      _args[_argcount++] = NULL;
      cmds[cmd_count++] = _argcount;
      cmds[cmd_count] = -1;
   }
}

void new_semicolon(void){
   if(_wordop){
      _wordop = 1;
      _args[_argcount++] = NULL;
      cmds[cmd_count++] = -2;
      cmds[cmd_count] = -1;
      redir_in = redir_out = 0;
      return;
   } else {
      fprintf(stderr, "Syntax error: did not expect semi-colon after operator.\n");
      operror = 1;
      return;
   }
}

