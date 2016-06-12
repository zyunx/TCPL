/* ldc.c - Reverse Polish calculator */
#include <stdio.h>
#include <stdlib.h>			/* for atof() */

#define		MAXOP	100		/* max size of operand of operator */
#define		NUMBER	'0'		/* signal that a number was found */

int getop(char []);
void push(double);
double pop(void);
void clear();
double peek();
void exchg();

/* reverse Polish calculator */
main()
{
	int type;
	double op2, op3;
	char s[MAXOP];

	while ((type = getop(s)) != EOF) {
		switch (type) {
		case NUMBER:
			push(atof(s));
			break;
		case '+':
			push(pop() + pop());
			break;
		case '*':
			push(pop() * pop());
			break;
		case '-':
			op2 = pop();
			push(pop() - op2);
			break;
		case '/':
			op2 = pop();
			if (op2 != 0.0)
				push(pop() / op2);
			else
				printf("error: zero divisor\n");
			break;
		case '%':
			op2 = pop();
			if (op2 != 0.0)
				push((int)pop() % (int)op2);
			else
				printf("error: zero divisor\n");
			break;
		case 'p':		/* print top stack number */
			printf("\t%.8g\n", peek());
			break;
		case 'x':
			exchg();
			break;	
		case 'c':
			clear();
			break;		
		case '\n':
			//printf("\t%.8g\n", pop());
			break;
		default:
			printf("error: unknown command %s\n", s);
			break;
		}
	}

	return 0;
}

/* stack.c - stack operation */
#define		MAXVAL	100	/* maximum depth of val stack */

int sp = 0;			/* next free stack position */
double val[MAXVAL];		/* value stack */

/* push: push 'f' onto value stack */
void push(double f)
{
	if (sp < MAXVAL)
		val[sp++] = f;
	else
		printf("error: stack full, can't push %g\n", f);
}

/* pop: pop and return top value from stack */
double pop(void)
{
	if (sp > 0)
		return val[--sp];
	else {
		printf("error: stack empty\n");
		return 0.0;
	}
}

/* clear: clear stack */
void clear()
{
	sp = 0;
}
/* peek: peek top stack number */
double peek()
{
	if (sp > 0)
		return val[sp-1];
	else {
		printf("error: stack empty\n");
		return 0.0;
	}
}
/* exchg: exchange the top 1st, 2nd numbers */
void exchg()
{
	double t;
	if (sp >= 2) {
		t = val[sp - 1];
		val[sp - 1] = val[sp - 2];
		val[sp - 2] = t;
	} else
		printf("error: # of stack numbers is less than 2\n");
}

/* getop.c - get next operator */
#include <ctype.h>

int getch(void);
void ungetch(int);

/* getop: get next operator or numeric operand */
int getop(char s[])
{
	int i, c;

	while ((s[0] = c = getch()) == ' ' || c == '\t')
		;

	s[1] = '\0';
	if (!isdigit(c) && c != '.' && c != '-')
		return c;		/* not a number */

	if (c == '-') {
		s[1] = c = getch();
		i = 1;
	} else
		i = 0;

	if (isdigit(c))			/* collect integer part */
		while (isdigit(s[++i] = c = getch()))
			;
	if (c == '.')			/* colloct fractional part */
		while (isdigit(s[++i] = c = getch()))
			;
	s[i] = '\0';

	/* '-' is not unary operator if the following character is not digit */
	if (strcmp(s, "-") == 0) {
		ungetch(c);
		return '-';
	}

	if (c != EOF)
		ungetch(c);
	return NUMBER;
}
/* getch.c - get and unget a char */
#define		BUFSIZE		100

char buf[BUFSIZE];	/* buffer for ungetch */
int bufp = 0;		/* next free position in buf */

int getch(void)		/* get a (possibly pushed back) character */
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)	/* push character back on input */
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}
