%{
#include "y.tab.h"
int line_num = 1;
%}
%option noyywrap
%%

\"[^"\n]* {
			if (dl_text[yyleng - 1] == '\\')
				yymore();
			else {
				strcpy(dl_lval.string,&yytext[1]);

				if (input() != '"') {
					fprintf(stderr,"EOL found inside \" on line %d\n",line_num);
					return(-1);
				}
				return(STRING);
			}
		};
[a-zA-Z][a-zA-Z0-9.-_]* {
		strcpy(dl_lval.string, yytext);
		return(NAME);
	};
[0-9]{1,2}[:][0-9]{1,2}[:][0-9]{1,2} {
		return(TIME);
	};
[0-9]{1,2}[:][0-9]{1,2} {
		sprintf(dl_lval.string, "%s:00", yytext);
		return(TIME);
	};
[0-9]{1,2}[/][0-9]{1,2}[/][0-9]{1,4} {
		return(DATE);
	};
[0-9]{1,2}[/][0-9]{1,2} {
		sprintf(dl_lval.string, "%s/00", yytext);
		return(DATE);
	};
[0-9]+ {
		dl_lval.integer = atoi(yytext);
		return(INTEGER);
	};
-[0-9]+ {
		dl_lval.integer = atoi(yytext);
		return(INTEGER);
	};
[\[\]{}(),:=] {return(yytext[0]);};
\n {line_num++; return(yytext[0]);};
\\\n	;
[ \t]*	;
#.*		;
. {
	fprintf(stderr,"Bad datalink save file (%s).\n", yytext);
	return(BAD);
};
%%
