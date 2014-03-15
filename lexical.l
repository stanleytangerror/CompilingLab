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
wrongid		{digit}+{letter}+[{digit}{letter}]*
id		{letter}({letter}|{digit})*
wrongoct	0[0-7]*[89]+{digit}*
wronghex	0x[0-9a-fA-F]*[g-zG-Z]+[{digit}{letter}]*
oct		[+-]?0[0-7]+
hex		[+-]?0x[0-9a-fA-F]+
int		[+-]?(([1-9]{digit}*)|0)

%%

{break}		{chars ++; cols = 0;}
{delim}		{chars += yyleng; cols += yyleng; }
";"		{printf("SEMI\tLn %d\n", (int)yylineno);}
","		{printf("COMMA\tLn %d\n", (int)yylineno);}
"="		{printf("ASSIGNOP\tLn %d\n", (int)yylineno);}
">"|"<"|">="|"<="|"=="|"!="	{printf("RELOP\tLn %d\n", (int)yylineno);}
"+"		{printf("PLUS\tLn %d\n", (int)yylineno);}
"-"		{printf("MINUS\tLn %d\n", (int)yylineno);}
"*"		{printf("STAR\tLn %d\n", (int)yylineno);}
"/"		{printf("DIV\tLn %d\n", (int)yylineno);}
"&&"		{printf("AND\tLn %d\n", (int)yylineno);}
"||"		{printf("OR\tLn %d\n", (int)yylineno);}
"."		{printf("DOT\tLn %d\n", (int)yylineno);}
"!"		{printf("NOT\tLn %d\n", (int)yylineno);}
"int"|"float"	{printf("TYPE\tLn %d\n", (int)yylineno);}
"("		{printf("LP\tLn %d\n", (int)yylineno);}
")"		{printf("RP\tLn %d\n", (int)yylineno);}
"["		{printf("LB\tLn %d\n", (int)yylineno);}
"]"		{printf("RB\tLn %d\n", (int)yylineno);}
"{"		{printf("LC\tLn %d\n", (int)yylineno);}
"}"		{printf("RC\tLn %d\n", (int)yylineno);}
"struct"	{printf("STRUCT\tLn %d\n", (int)yylineno);}
"return"	{printf("RETURN\tLn %d\n", (int)yylineno);}
"if"		{printf("IF\tLn %d\n", (int)yylineno);}
"else"		{printf("ELSE\tLn %d\n", (int)yylineno);}
"while"		{printf("WHILE\tLn %d\n", (int)yylineno);}
{wrongid}	{printf("Error type A at line %d: Mysterious character \'%s\'\n", yylineno, yytext); }
{id}		{printf("Id: %s\tLn %d\n", yytext, yylineno);}
{wrongoct}|{wronghex}	{printf("Error type A at line %d: Mysterious character \'%s\'\n", yylineno, yytext); }
{oct}		{printf("OCT: %s\tLn %d\n", yytext, yylineno);}
{hex}		{printf("HEX: %s\tLn %d\n", yytext, yylineno);}
{int}		{printf("INT: %s\tLn %d\n", yytext, yylineno);}
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
