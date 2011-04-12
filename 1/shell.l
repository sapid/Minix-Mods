/* Author: Will Crawford <wacrawfo@ucsc.edu> */
%{

#include <stdio.h>
#include <strings.h>

%}

WORD              [a-zA-Z0-9\/\.-]+

%%

"exit"            { return yylval_token(TOK_EXIT); }
{WORD}            { return yylval_token(TOK_WORD); }
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
