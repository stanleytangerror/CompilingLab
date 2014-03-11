%option yylineno
%{
	int cols = 0;
	int chars = 0;
	int words = 0;
	int lines = 0;
%}

break		\n
delim		[ \t\n]
ws		{delim}+
letter		[_a-zA-Z]
digit		[0-9]
id		{letter}({letter}|{digit})*
oct		[+-]?0[0-7]+
hex		[+-]?0x[0-9a-fA-F]+
int		([+-]?([1-9]\d*)|0)
number		{digit}+(\.{digit}+)?(E[+-]?{digit}+)?

%%

{break}		{chars ++; cols = 0;}
{delim}		{chars += yyleng; cols += yyleng; }
";"		{printf("SEMI\tLn %d, Col %d\n", (int)yylineno, (int)cols); chars ++; cols ++; }
","		{printf("COMMA\tLn %d, Col %d\n", (int)yylineno, (int)cols); chars ++; cols ++; }
"="		{printf("ASSIGNOP\tLn %d, Col %d\n", (int)yylineno, (int)cols); chars ++; cols ++; }
">"|"<"|">="|"<="|"=="|"!="	{printf("RELOP\tLn %d, Col %d\n", (int)yylineno, (int)cols); chars ++; cols ++; }
"+"		{printf("PLUS\tLn %d, Col %d\n", (int)yylineno, (int)cols); chars ++; cols ++; }
"-"		{printf("MINUS\tLn %d, Col %d\n", (int)yylineno, (int)cols); chars ++; cols ++; }
"*"		{printf("STAR\tLn %d, Col %d\n", (int)yylineno, (int)cols); chars ++; cols ++; }
"/"		{printf("DIV\tLn %d, Col %d\n", (int)yylineno, (int)cols); chars ++; cols ++; }
"&&"		{printf("AND\tLn %d, Col %d\n", (int)yylineno, (int)cols); chars ++; cols ++; }
"||"		{printf("OR\tLn %d, Col %d\n", (int)yylineno, (int)cols); chars ++; cols ++; }
"."		{printf("DOT\tLn %d, Col %d\n", (int)yylineno, (int)cols); chars ++; cols ++; }
"!"		{printf("NOT\tLn %d, Col %d\n", (int)yylineno, (int)cols); chars ++; cols ++; }
"int"|"float"	{printf("TYPE\tLn %d, Col %d\n", (int)yylineno, (int)cols); chars ++; cols ++; }
"("		{printf("LP\tLn %d, Col %d\n", (int)yylineno, (int)cols); chars ++; cols ++; }
")"		{printf("RP\tLn %d, Col %d\n", (int)yylineno, (int)cols); chars ++; cols ++; }
"["		{printf("LB\tLn %d, Col %d\n", (int)yylineno, (int)cols); chars ++; cols ++; }
"]"		{printf("RB\tLn %d, Col %d\n", (int)yylineno, (int)cols); chars ++; cols ++; }
"{"		{printf("LC\tLn %d, Col %d\n", (int)yylineno, (int)cols); chars ++; cols ++; }
"}"		{printf("RC\tLn %d, Col %d\n", (int)yylineno, (int)cols); chars ++; cols ++; }
"struct"	{printf("STRUCT\tLn %d, Col %d\n", (int)yylineno, (int)cols); chars ++; cols ++; }
"return"	{printf("RETURN\tLn %d, Col %d\n", (int)yylineno, (int)cols); chars ++; cols ++; }
"if"		{printf("IF\tLn %d, Col %d\n", (int)yylineno, (int)cols); chars ++; cols ++; }
"else"		{printf("ELSE\tLn %d, Col %d\n", (int)yylineno, (int)cols); chars ++; cols ++; }
"while"		{printf("WHILE\tLn %d, Col %d\n", (int)yylineno, (int)cols); chars ++; cols ++; }
{id}		{printf("Id: %s\tLn %d, Col \n", yytext, yylineno); chars += yyleng; }
{oct}		{printf("OCT: %s\tLn %d, Col \n", yytext, yylineno); chars += yyleng; }
{hex}		{printf("HEX: %s\tLn %d, Col \n", yytext, yylineno); chars += yyleng; }
{int}		{printf("INT: %s\tLn %d, Col \n", yytext, yylineno); chars += yyleng; }
{number}	{printf("NUM: %s\tLn %d, Col \n", yytext, yylineno); chars += yyleng; }
.		{printf("Error type A at line %d: Mysterious character \'%s\'\n", yylineno, yytext); } 

%%

int yywrap() {
	return 1;
}

int main(int argc, char** argv) {
	if (argc > 1) {
		if ( !(yyin = fopen(argv[1], "r")) ) {
			perror(argv[1]);
			return 1;
		}
	}
	yylex();
	printf("Chars: %8d\n", chars);
	printf("Words: %8d\n", words);
	printf("Lines: %8d\n", lines);

	return 0;
}
