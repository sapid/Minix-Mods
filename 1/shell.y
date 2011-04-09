/* Author: Will Crawford <wacrawfo@ucsc.edu> */
%{

%}

%error-verbose
%token TOK_WORD TOK_EXIT TOK_NEWLINE TOK_EOF
%start cmd

%%

cmd		: word ';'
		| word TOK_NEWLINE
		;

pipe	: word '|' word
		;

word	:	TOK_WORD
		|	word TOK_WORD
		;

