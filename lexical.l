%option yylineno
%{
	#include "syntax.tab.h"	
	int cols = 0;
	int chars = 0;
	int words = 0;
	int lines = 0;
	int yycolumn = 1;
	#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;\
	   yylloc.first_column = yycolumn ; yylloc.last_column = yycolumn + yyleng -1;\
	   yycolumn += yyleng;
%}

break		\n
delim		[ \t\n]
ws		{delim}+
letter		[_a-zA-Z]
digit		[0-9]
id		{letter}({letter}|{digit})*
oct		0[0-7]+
hex		0x[0-9a-fA-F]+
int		([1-9]{digit}*)|0
float 		{int}\.{digit}*[1-9]
number		{digit}+(\.{digit}+)?(E[+-]?{digit}+)?

%%

{break}		{yycolumn = 1;}
{delim}		{}
";"		{return SEMI;}
","		{return COMMA; }
"="		{return ASSIGNOP;  }
">"|"<"|">="|"<="|"=="|"!="	{return RELOP;  }
"+"		{return PLUS; }
"-"		{return MINUS;  }
"*"		{return STAR;  }
"/"		{return DIV;  }
"&&"		{return AND;  }
"||"		{return OR;  }
"."		{return DOT;  }
"!"		{return NOT;  }
"int"|"float"	{return TYPE;  }
"("		{return LP;  }
")"		{return RP;  }
"["		{return LB;  }
"]"		{return RB;  }
"{"		{return LC;  }
"}"		{return RC;  }
"struct"	{return STRUCT;  }
"return "	{return RETURN;  }
"if"		{return IF;  }
"else"		{return ELSE;  }
"while"		{return WHILE;  }
{id}		{return ID;}
{oct}		{return OCT;}
{hex}		{return HEX;}
{int}		{
			yylval.type_int = atoi(yytext);
			return  INT;		
		}
{float}		{
			yylval.type_float = atof(yytext);
			return FLOAT;
		}
{number}	{return NUM;}
.		{printf("Error type A at line %d: Mysterious character \'%s\'\n", yylineno, yytext); } 

%%

int yywrap() {
	return 1;
}
