
/*  A Bison parser, made from dataread.y
 by  GNU Bison version 1.25.90
  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse dl_parse
#define yylex dl_lex
#define yyerror dl_error
#define yylval dl_lval
#define yychar dl_char
#define yydebug dl_debug
#define yynerrs dl_nerrs
#define	NAME	257
#define	INTEGER	258
#define	STRING	259
#define	TIME	260
#define	DATE	261
#define	BAD	262

#line 1 "dataread.y"

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "datalink.h"

extern int line_num;

#define ALARM 0
#define APP 1
#define PHONE 2
#define TODO 3
#define ANNIV 4
#define SYSTEM 5
#define WRISTAPP 6
#define MELODY 7
#define TIMEZONE 8
#define DATA 9
#define INTVAL 10
#define NUMLISTS 9

static ListPtr *lists = NULL;
static ItemPtr ip = NULL;
static char buf[1024];
static int hour, minute, second, month, day, year;
static int prio, audible, chime, beep, pos;
static int val, tfmt, dfmt;
static char msg[64];
static char phnum[64];
static WatchInfoPtr wi;
static int watch_type;


#line 37 "dataread.y"
typedef union {
	char string[1024];
	int integer;
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		36
#define	YYFLAG		-32768
#define	YYNTBASE	12

#define YYTRANSLATE(x) ((unsigned)(x) <= 262 ? yytranslate[x] : 15)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    10,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,    11,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     9,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     5,    10,    12,    22,    28,    32,    36,    40,
    44,    54,    56
};

static const short yyrhs[] = {    13,
     0,    12,    13,     0,     3,     9,    14,    10,     0,    10,
     0,     4,    11,     7,    11,     6,    11,     5,    11,     4,
     0,     7,    11,     6,    11,     5,     0,     4,    11,     5,
     0,     5,    11,     5,     0,     7,    11,     5,     0,     4,
    11,     4,     0,     4,    11,     5,    11,     4,    11,     4,
    11,     4,     0,     5,     0,     4,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    52,    53,    56,   390,   393,   404,   412,   419,   426,   433,
   440,   450,   455
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","NAME","INTEGER",
"STRING","TIME","DATE","BAD","'='","'\\n'","','","data","item","value", NULL
};
#endif

static const short yyr1[] = {     0,
    12,    12,    13,    13,    14,    14,    14,    14,    14,    14,
    14,    14,    14
};

static const short yyr2[] = {     0,
     1,     2,     4,     1,     9,     5,     3,     3,     3,     3,
     9,     1,     1
};

static const short yydefact[] = {     0,
     0,     4,     0,     1,     0,     2,    13,    12,     0,     0,
     0,     0,     0,     3,    10,     7,     0,     8,     9,     0,
     0,     0,     0,     0,     0,     6,     0,     0,     0,     0,
     0,     0,    11,     5,     0,     0
};

static const short yydefgoto[] = {     3,
     4,    10
};

static const short yypact[] = {    -2,
    -7,-32768,     0,-32768,     2,-32768,     4,     5,     6,     3,
     7,    13,    -1,-32768,-32768,     8,     9,-32768,-32768,    10,
    18,    17,    19,    14,    15,-32768,    23,    24,    20,    21,
    26,    29,-32768,-32768,    28,-32768
};

static const short yypgoto[] = {-32768,
    31,-32768
};


#define	YYLAST		34


static const short yytable[] = {    35,
     1,     5,     1,    19,    20,     7,     8,     2,     9,     2,
    15,    16,    14,    17,    11,    12,    13,    18,    21,    22,
    23,    24,    25,    26,    27,    28,    29,    36,    30,    33,
    31,    32,    34,     6
};

static const short yycheck[] = {     0,
     3,     9,     3,     5,     6,     4,     5,    10,     7,    10,
     4,     5,    10,     7,    11,    11,    11,     5,    11,    11,
    11,     4,     6,     5,    11,    11,     4,     0,     5,     4,
    11,    11,     4,     3
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/share/misc/bison.simple"
/* This file comes from bison-1.25.90.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 216 "/usr/share/misc/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int yyparse (void *);
#else
int yyparse (void);
#endif
#endif

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 3:
#line 57 "dataread.y"
{
	int fd;
	char file[1024];
	unsigned char mask;
	int min;
	struct stat sbuf;
	int l;
	int i;

	if (!lists) {
		wi = dl_init_watch(watch_type);

		if ((lists = (ListPtr *)calloc(NUMLISTS, sizeof(ListPtr))) == NULL) {
			dl_error("Could not allocate List list.");
			return(-1);
		}

		for (i = 0; i < NUMLISTS; i++) {

			if (!(lists[i] = dl_new_list())) {
				dl_error("Could not allocate item list.");
				return(-1);
			}

		}

	}

	switch (yyvsp[-1].integer) {
	case ALARM:

		if (strcmp(yyvsp[-3].string, "alarm")) {
			sprintf(buf, "Bad entry, %s = alarm.", yyvsp[-3].string);
			dl_error(buf);
			return(-1);
		}

		if (!(ip = dl_new_item(wi, DL_ALARM_TYPE))) {
			dl_error("Could not allocate alarm item.");
			return(-1);
		}

		ip->data.alarm.alarm_num = pos;
		ip->data.alarm.hours = hour;
		ip->data.alarm.minutes = minute;
		ip->data.alarm.month = month;
		ip->data.alarm.day = day;
		ip->data.alarm.audible = audible;
		l = strlen(msg);

		if (l > wi->max_alarm_str)
			l = wi->max_alarm_str;

		if ((ip->data.alarm.label = (char *)malloc(l + 1)) == NULL) {
			dl_error("Could not allocate alarm label.");
			return(-1);
		}

		strncpy(ip->data.alarm.label, msg, l);
		ip->data.alarm.label[l] = '\0';
		dl_add_to_list(lists[ALARM], ip);
		break;
	case APP:

		if (strcmp(yyvsp[-3].string, "appointment")) {
			sprintf(buf, "Bad entry, %s = appointment.", yyvsp[-3].string);
			dl_error(buf);
			return(-1);
		}

		if (!(ip = dl_new_item(wi, DL_APP_TYPE))) {
			dl_error("Could not allocate appointment item.");
			return(-1);
		}

		ip->data.app.month = month;
		ip->data.app.day = day;
		minute = hour*60+minute;
		ip->data.app.time = 0;

		for (min = 960, mask = 0x40; mask; mask >>= 1, min /= 2) {

			if (minute >= min) {
				ip->data.app.time |= mask;
				minute -= min;
			}

		}

		l = strlen(msg);

		if (l > wi->max_str)
			l = wi->max_str;

		if ((ip->data.app.label = (char *)malloc(l + 1)) == NULL) {
			dl_error("Could not allocate appointment label.");
			return(-1);
		}

		strncpy(ip->data.app.label, msg, l);
		ip->data.app.label[l] = '\0';
		dl_add_to_list(lists[APP], ip);
		break;
	case PHONE:

		if (strcmp(yyvsp[-3].string, "phone")) {
			sprintf(buf, "Bad entry, %s = phone.", yyvsp[-3].string);
			dl_error(buf);
			return(-1);
		}

		if (!(ip = dl_new_item(wi, DL_PHONE_TYPE))) {
			dl_error("Could not allocate phone item.");
			return(-1);
		}

		l = strlen(phnum);

		if (l > wi->max_phone_str)
			l = wi->max_phone_str;

		if ((ip->data.phone.number = (char *)malloc(l + 1)) == NULL) {
			dl_error("Could not allocate phone number.");
			return(-1);
		}

		strncpy(ip->data.phone.number, phnum, l);
		ip->data.phone.number[l] = '\0';

		l = strlen(msg);

		if (l > wi->max_str)
			l = wi->max_str;

		if ((ip->data.phone.label = (char *)malloc(l + 1)) == NULL) {
			dl_error("Could not allocate phone label.");
			return(-1);
		}

		strncpy(ip->data.phone.label, msg, l);
		ip->data.phone.label[l] = '\0';
		dl_add_to_list(lists[PHONE], ip);
		break;
	case TODO:

		if (strcmp(yyvsp[-3].string, "todo")) {
			sprintf(buf, "Bad entry, %s = todo.", yyvsp[-3].string);
			dl_error(buf);
			return(-1);
		}

		if (!(ip = dl_new_item(wi, DL_TODO_TYPE))) {
			dl_error("Could not allocate todo item.");
			return(-1);
		}

		ip->data.todo.priority = prio;
		l = strlen(msg);

		if (l > wi->max_str)
			l = wi->max_str;

		if ((ip->data.todo.label = (char *)malloc(l + 1)) == NULL) {
			dl_error("Could not allocate todo label.");
			return(-1);
		}

		strncpy(ip->data.todo.label, msg, l);
		ip->data.todo.label[l] = '\0';
		dl_add_to_list(lists[TODO], ip);
		break;
	case ANNIV:

		if (strcmp(yyvsp[-3].string, "anniversary")) {
			sprintf(buf, "Bad entry, %s = anniversary.", yyvsp[-3].string);
			dl_error(buf);
			return(-1);
		}

		if (!(ip = dl_new_item(wi, DL_ANNIV_TYPE))) {
			dl_error("Could not allocate anniversary item.");
			return(-1);
		}

		ip->data.anniv.month = month;
		ip->data.anniv.day = day;

		l = strlen(msg);

		if (l > wi->max_str)
			l = wi->max_str;

		if ((ip->data.anniv.label = (char *)malloc(l + 1)) == NULL) {
			dl_error("Could not allocate anniversary label.");
			return(-1);
		}

		strncpy(ip->data.anniv.label, msg, l);
		ip->data.anniv.label[l] = '\0';
		dl_add_to_list(lists[ANNIV], ip);
		break;
	case SYSTEM:

		if (strcmp(yyvsp[-3].string, "system")) {
			sprintf(buf, "Bad entry, %s = system.", yyvsp[-3].string);
			dl_error(buf);
			return(-1);
		}

		if (!(ip = dl_new_item(wi, DL_SYSTEM_TYPE))) {
			dl_error("Could not allocate system item.");
			return(-1);
		}

		ip->data.system.chime = chime;
		ip->data.system.beep = beep;
		dl_add_to_list(lists[SYSTEM], ip);
		break;
	case DATA:

		if (*msg == '~')
			sprintf(file, "%s%s", getenv("HOME"), &msg[1]);
		else
			strcpy(file, msg);

		if ((fd = open(file, O_RDONLY)) < 0) {
			sprintf(buf, "Could not open %s for reading %s.", file, yyvsp[-3].string);
			dl_error(buf);
			return(-1);
		}

		if (fstat(fd, &sbuf) < 0) {
			sprintf(buf, "Could not stat %s for %s.", file, yyvsp[-3].string);
			dl_error(buf);
			return(-1);
		}

		if (strcmp(yyvsp[-3].string, "wristapp") == 0) {

			if (!(ip = dl_new_item(wi, DL_WRISTAPP_TYPE))) {
				dl_error("Could not allocate system item.");
				return(-1);
			}

		}
		else if (strcmp(yyvsp[-3].string, "melody") == 0) {

			if (!(ip = dl_new_item(wi, DL_MELODY_TYPE))) {
				dl_error("Could not allocate system item.");
				return(-1);
			}

		}
		else {
			sprintf(buf, "Bad entry, %s = string.", yyvsp[-3].string);
			dl_error(buf);
			return(-1);
		}

		ip->data.wristapp.len = sbuf.st_size;

		if ((ip->data.wristapp.data = (char *)malloc(sbuf.st_size)) == NULL) {
			sprintf(buf, "Could not allocate data for %s.", yyvsp[-3].string);
			dl_error(buf);
			return(-1);
		}

		if (read(fd, ip->data.wristapp.data, sbuf.st_size) != sbuf.st_size) {
			sprintf(buf, "Could not read data for %s from %s.", yyvsp[-3].string, file);
			dl_error(buf);
			return(-1);
		}

		if (strcmp(yyvsp[-3].string, "wristapp") == 0)
			dl_add_to_list(lists[WRISTAPP], ip);
		else
			dl_add_to_list(lists[MELODY], ip);

		break;
	case INTVAL:

		if (strcmp(yyvsp[-3].string, "time_adjust") == 0) {
			wi->time_adjust = val;
		}
		else if (strcmp(yyvsp[-3].string, "pre_notification_time") == 0) {
			wi->pre_notification_time = val;
		}
		else {
			sprintf(buf, "Bad entry, %s = intval.", yyvsp[-3].string);
			dl_error(buf);
			return(-1);
		}

		break;
	case TIMEZONE:

		if (strcmp(yyvsp[-3].string, "timezone")) {
			sprintf(buf, "Bad entry, %s = timezone.", yyvsp[-3].string);
			dl_error(buf);
			return(-1);
		}

		if (!(ip = dl_new_item(wi, DL_TIME_TYPE))) {
			dl_error("Could not allocate timezone item.");
			return(-1);
		}

		l = strlen(msg);

		if (l > wi->max_tzlen)
			l = wi->max_tzlen;

		if ((ip->data.time.label = (char *)malloc(l + 1)) == NULL) {
			dl_error("Could not allocate timezone label.");
			return(-1);
		}

		strncpy(ip->data.time.label, msg, l);
		ip->data.time.label[l] = '\0';
		ip->data.time.offset = minute;
		ip->data.time.tz_num = pos;
		ip->data.time.month = 1;
		ip->data.time.day = 1;
		ip->data.time.hour_fmt = tfmt;
		ip->data.time.date_fmt = dfmt;
		dl_add_to_list(lists[TIMEZONE], ip);
		break;
	default:
		dl_error("Internal Error, unknown type.");
		return(-1);
	}

;
    break;}
case 5:
#line 395 "dataread.y"
{
				pos = yyvsp[-8].integer;
				sscanf(yyvsp[-6].string, "%d/%d/%d", &month, &day, &year);
				sscanf(yyvsp[-4].string, "%d:%d:%d", &hour, &minute, &second);
				strcpy(msg, yyvsp[-2].string);
				audible = yyvsp[0].integer;
				yyval.integer = ALARM;
			;
    break;}
case 6:
#line 406 "dataread.y"
{
				sscanf(yyvsp[-4].string, "%d/%d/%d", &month, &day, &year);
				sscanf(yyvsp[-2].string, "%d:%d:%d", &hour, &minute, &second);
				strcpy(msg, yyvsp[0].string);
				yyval.integer = APP;
			;
    break;}
case 7:
#line 414 "dataread.y"
{
				prio =  yyvsp[-2].integer;
				strcpy(msg, yyvsp[0].string);
				yyval.integer = TODO;
			;
    break;}
case 8:
#line 421 "dataread.y"
{
				strcpy(phnum, yyvsp[-2].string);
				strcpy(msg, yyvsp[0].string);
				yyval.integer = PHONE;
			;
    break;}
case 9:
#line 428 "dataread.y"
{
				sscanf(yyvsp[-2].string, "%d/%d/%d", &month, &day, &year);
				strcpy(msg, yyvsp[0].string);
				yyval.integer = ANNIV;
			;
    break;}
case 10:
#line 435 "dataread.y"
{
				chime =  yyvsp[-2].integer;
				beep =  yyvsp[0].integer;
				yyval.integer = SYSTEM;
			;
    break;}
case 11:
#line 442 "dataread.y"
{
				pos = yyvsp[-8].integer;
				strcpy(msg, yyvsp[-6].string);
				minute = yyvsp[-4].integer;
				tfmt = yyvsp[-2].integer;
				dfmt = yyvsp[0].integer;
				yyval.integer = TIMEZONE
			;
    break;}
case 12:
#line 451 "dataread.y"
{
				strcpy(msg, yyvsp[0].string);
				yyval.integer = DATA;
			;
    break;}
case 13:
#line 456 "dataread.y"
{
				val = yyvsp[0].integer;
				yyval.integer = INTVAL;
			;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 542 "/usr/share/misc/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
#line 462 "dataread.y"

dl_error(s)
char *s;
{
	char buf[1024];

	sprintf(buf, "%s on line %d.",s,line_num);
	(*dl_error_proc)(buf);
}

extern FILE *dl_in;

WatchInfoPtr
dl_read_save(datafile, type, times, alarms, apps, todos, phones, annivs,
	system, wristapp, melody)
char *datafile;
int type;
ListPtr *times;
ListPtr *alarms;
ListPtr *apps;
ListPtr *todos;
ListPtr *phones;
ListPtr *annivs;
ListPtr *system;
ListPtr *wristapp;
ListPtr *melody;
{
	char file[1024];
	char buf[1024];
	int r;

	if (*datafile == '~')
		sprintf(file, "%s%s", getenv("HOME"), &datafile[1]);
	else
		strcpy(file, datafile);

	watch_type = type;

	if ((dl_in = fopen(file,"r")) == NULL) {
		sprintf(buf,"Could not open %s for reading.", file);
		(*dl_error_proc)(buf);
		return(NULL);
	}

	r = dl_parse();
	fclose(dl_in);

	if (r)
		return(NULL);

	*alarms = lists[ALARM];
	*apps = lists[APP];
	*phones = lists[PHONE];
	*todos = lists[TODO];
	*annivs = lists[ANNIV];
	*system = lists[SYSTEM];
	*wristapp = lists[WRISTAPP];
	*melody = lists[MELODY];
	*times = lists[TIMEZONE];
	free(lists);
	return(wi);
}
