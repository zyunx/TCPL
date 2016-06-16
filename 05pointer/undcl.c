/* undcl.c - make declaration */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN	100

enum { NAME, PARENS, BRACKETS };

void dcl(void);
void dirdcl(void);

int gettoken(void);
int tokentype;			/* type of last token */
char token[MAXTOKEN];		/* last token string */
char name[MAXTOKEN];		/* identifier name */
char datatype[MAXTOKEN];	/* data type = "char, int, etc. */
char out[1000];			/* output string */

main()				/* undcl, make a declaration */
{
	int type, lasttype;
	char temp[MAXTOKEN];

	while ((lasttype = gettoken()) != EOF) {
		strcpy(out, token);
		while ((type = gettoken()) != '\n') {
			if (type == PARENS || type == BRACKETS) {
				/* check operator's precedence.
				surround it in parenthesis, if higher */
				if (lasttype == '*') {
					sprintf(temp, "(%s)", out);
					strcpy(out, temp);
				}
				strcat(out, token);
	
			} else if (type == '*') {
				sprintf(temp, "*%s", out);
				strcpy(out, temp);
			} else if (type == NAME) {
				sprintf(temp, "%s %s", token, out);
				strcpy(out, temp);
			} else
				printf("invalid input at %s\n", token);

			lasttype = type;
		}
		
		printf("%s\n", out);
	}
	return 0;
}

/* gettoken: return next token */
int gettoken(void)
{
	int c, getch(void);
	void ungetch(int c);
	char *p = token;

	while ((c = getch()) == ' ' || c == '\t')
		;
	if (c == '(') {
		if ((c = getch()) == ')') {
			strcpy(token, "()");
			return tokentype = PARENS;
		} else {
			ungetch(c);
			return tokentype = '(';
		}
	} else if (c == '[') {
		for (*p++ = c; (*p++ = getch()) != ']'; )
			;
		*p = '\0';
		return tokentype = BRACKETS;
	} else if (isalpha(c)) {
		for (*p++ = c; isalnum(c = getch()); )
			*p++ = c;
		*p = '\0';
		ungetch(c);
		return tokentype = NAME;
	} else
		return tokentype = c;
}

