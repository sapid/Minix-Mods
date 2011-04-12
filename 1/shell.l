/* Author: Will Crawford <wacrawfo@ucsc.edu> */
%{

#include <stdio.h>
#include <strings.h>

%}

WORD        [a-zA-Z0-9\/\.-]+

%%

{WORD}            { return yylval_token(TOK_WORD); }
"exit"            { return yylval_token(TOK_EXIT); }
"("               { return yylval_token('('); }
")"               { return yylval_token(')'); }
">"               { return yylval_token('>'); }
"<"               { return yylval_token('<'); }
"|"               { return yylval_token('|'); }
"&"               { return yylval_token('&'); }
";"               { return yylval_token(';'); }
"*"               { return yylval_token('*'); }
"\n"              { return yylval_token(TOK_NEWLINE; }
[ \t]+            { }
.                 { }

%%

char **getline() { return (char **)yylex(); }
