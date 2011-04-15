%{
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#define YYSTYPE char**

int _numargs = 200;
char *_args[256];
int cmds[32];
int _argcount = 0;
int cmdcount = 0;
int _wordop = 0;
int operror = 0;
extern int pipe_count;
int redir_in;
int redir_out;
void add_arg(void);
void new_word(void);
void new_op(void);
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
  _argcount = _wordop = cmdcount = operror = 0; _args[0] = NULL;
  redir_in = redir_out = 0;

{WORD}               {
                        new_word();
                        add_arg();
                     }
[>]                  {
                        new_op();
                        add_arg();
                        if(redir_out){
                           fprintf(stderr, "Syntax error: detected two or more '>'\n");
                           operror = 1;
                        } else redir_out = 1;
                     }
[<]                  {
                        new_op();
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
                        new_op();
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


\n                    { return _args; }

[ \t]+

.

%%

char **getline(void) { return (char **)yylex(); }

void add_arg(void){
   if(_argcount < _numargs - 1) {
      _args[_argcount++] = (char*)strdup(yytext);
      _args[_argcount] = NULL;
   }
}

void new_op(void){
   if(_wordop){
      _wordop = 0;
      _args[_argcount++] = NULL;
      cmds[cmdcount++] = _argcount;
      cmds[cmdcount] = -1;
   } else {
      fprintf(stderr, "Syntax error: adjacent operators detected.\n");
      operror = 1;
   }
}

void new_word(void){
   if(!_wordop){
      _wordop = 1;
      _args[_argcount++] = NULL;
      cmds[cmdcount++] = _argcount;
      cmds[cmdcount] = -1;
   }
}

void new_semicolon(void){
   if(_wordop){
      _wordop = 1;
      _args[_argcount++] = NULL;
      cmds[cmdcount++] = _argcount;
      cmds[cmdcount] = -1;
   } else {
      fprintf(stderr, "Syntax error: did not expect semi-colon after operator.\n");
      operror = 1;
   }
}

