%option yylineno
%{
	#include "syntax.tab.h"	
  #include "tree.h"
//  #define YYDEBUG 1
  
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
wrongoct  0[0-7]*[89]+({digit}|{letter})*
oct		0[0-7]+
wronghex  0x[0-9a-fA-F]*[g-zG-Z]+({digit}|{letter})*
hex		0x[0-9a-fA-F]+
wrongid  {digit}+{letter}+({digit}|{letter})*
id		{letter}({letter}|{digit})*
int		([1-9]{digit}*)|0
float 		{int}\.{digit}*[1-9]
number		{digit}+(\.{digit}+)?(E[+-]?{digit}+)?

%%
{break}		{yycolumn = 1;}
{delim}		{}
";"		{yylval.type_node = shiftTerminate(eSEMI, yylineno); return SEMI;}
","		{yylval.type_node = shiftTerminate(eCOMMA, yylineno); return COMMA; }
"="		{yylval.type_node = shiftTerminate(eASSIGNOP, yylineno); return ASSIGNOP;  }
">"|"<"|">="|"<="|"=="|"!="	{yylval.type_node = shiftRelop(yytext, yylineno); return RELOP;  }
"+"		{yylval.type_node = shiftTerminate(ePLUS, yylineno); return PLUS; }
"-"		{yylval.type_node = shiftTerminate(eMINUS, yylineno); return MINUS;  }
"*"		{yylval.type_node = shiftTerminate(eSTAR, yylineno); return STAR;  }
"/"		{yylval.type_node = shiftTerminate(eDIV, yylineno); return DIV;  }
"&&"		{yylval.type_node = shiftTerminate(eAND, yylineno); return AND;  }
"||"		{yylval.type_node = shiftTerminate(eOR, yylineno); return OR;  }
"."		{yylval.type_node = shiftTerminate(eDOT, yylineno); return DOT;  }
"!"		{yylval.type_node = shiftTerminate(eNOT, yylineno); return NOT;  }
"int"|"float"	{yylval.type_node = shiftType(yytext, yylineno); return TYPE;  }
"("		{yylval.type_node = shiftTerminate(eLP, yylineno); return LP;  }
")"		{yylval.type_node = shiftTerminate(eRP, yylineno); return RP;  }
"["		{yylval.type_node = shiftTerminate(eLB, yylineno); return LB;  }
"]"		{yylval.type_node = shiftTerminate(eRB, yylineno); return RB;  }
"{"		{yylval.type_node = shiftTerminate(eLC, yylineno); return LC;  }
"}"		{yylval.type_node = shiftTerminate(eRC, yylineno); return RC;  }
"struct"	{yylval.type_node = shiftTerminate(eSTRUCT, yylineno); return STRUCT;  }
"return "	{yylval.type_node = shiftTerminate(eRETURN, yylineno); return RETURN;  }
"if"		{yylval.type_node = shiftTerminate(eIF, yylineno); return IF;  }
"else"		{yylval.type_node = shiftTerminate(eELSE, yylineno); return ELSE;  }
"while"		{yylval.type_node = shiftTerminate(eWHILE, yylineno); return WHILE;  }
{wrongoct}		{printf("Error type A at line %d: Illegal octal number \'%s\'\n", yylineno, yytext); compile = false;} 
{wronghex}		{printf("Error type A at line %d: Illegal hexadecimal number \'%s\'\n", yylineno, yytext); compile = false;} 
{hex}		{yylval.type_node = shiftInt((int)strtol(yytext, NULL, 16), yylineno); return  INT; }
{oct}		{yylval.type_node = shiftInt((int)strtol(yytext, NULL, 8), yylineno); return  INT; }
{int}		{yylval.type_node = shiftInt((int)atoi(yytext), yylineno); return  INT; }
{float}		{yylval.type_node = shiftFloat((float)atof(yytext), yylineno);	return FLOAT; }
{wrongid}		{printf("Error type A at line %d: Mysterious character \'%s\'\n", yylineno, yytext); compile = false;} 
{id}		{yylval.type_node = shiftId(yytext, yylineno);	return ID;}
.		{printf("Error type A at line %d: Mysterious character \'%s\'\n", yylineno, yytext); compile = false;} 

%%

int yywrap() {
	return 1;
}

