typedef union {
	char string[1024];
	int integer;
} YYSTYPE;
#define	NAME	257
#define	INTEGER	258
#define	STRING	259
#define	TIME	260
#define	DATE	261
#define	BAD	262


extern YYSTYPE dl_lval;
