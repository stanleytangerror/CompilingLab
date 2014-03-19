%{
	#include<stdio.h>
  #include "lex.yy.c"
%}
%locations
/* declared types*/
%union{
	int type_int;
	float type_float;
	double type_double;
}

/* declared tokens*/
%token <type_int> INT
%token <type_float> FLOAT
%token ID OCT HEX NUM
%token SEMI COMMA ASSIGNOP RELOP
%token PLUS MINUS STAR DIV
%token AND OR DOT NOT TYPE
%token LP RP LB RB LC RC
%token STRUCT RETURN IF ELSE WHILE

/* declared non-terminals */
//%type <type_double> Exp Factor Term

%right ASSIGNOP
%right NOT
%left PLUS MINUS
%left STAR DIV
%left RELOP
%left AND OR
%left LP LB LC
%left RP RB RC
%left DOT
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%
Program		: ExtDefList
		;
ExtDefList	: ExtDef ExtDefList
		| /*empty*/
		;
ExtDef		: Specifier ExtDecList SEMI
		| Specifier SEMI
		| Specifier FunDec CompSt
		;
ExtDecList 	: VarDec
		| VarDec COMMA ExtDecList
		;
Specifier	: TYPE
		| StructSpecifier
		;
StructSpecifier : STRUCT OptTag LC DefList RC
		| STRUCT Tag
		;
OptTag		: ID
		| /*empty*/
		;
Tag		: ID
		;
VarDec		: ID
		| VarDec LB INT RB
		;
FunDec		: ID LP VarList RP
		| ID LP RP
		;
VarList		: ParamDec COMMA VarList
		| ParamDec
		;
ParamDec	: Specifier VarDec
		;
CompSt 		: LC DefList StmtList RC
		;
StmtList	: Stmt StmtList
		| /*empty*/
		;
Stmt 		: Exp SEMI
		| CompSt
		| RETURN Exp SEMI
		| IF LP Exp RP Stmt   %prec LOWER_THAN_ELSE
		| IF LP Exp RP Stmt ELSE Stmt
		| WHILE LP Exp RP Stmt
		;
DefList		: Def DefList
		| /*empty*/
		;
Def 		: Specifier DecList SEMI
		;
DecList 	: Dec
		| Dec COMMA DecList
		;
Dec 		: VarDec
		| VarDec ASSIGNOP Exp
		;
Exp 		: Exp ASSIGNOP Exp
		| Exp AND Exp
		| Exp OR Exp
		| Exp RELOP Exp
		| Exp PLUS Exp
		| Exp MINUS Exp
		| Exp STAR Exp
		| Exp DIV Exp
		| LP Exp RP
		| MINUS Exp
		| NOT Exp
		| ID LP Args RP
		| ID LP RP
		| Exp LB Exp RB
		| Exp DOT ID
		| ID
		| INT
		| FLOAT
		;
Args 		: Exp COMMA Args
		| Exp
		;
	
/*Exp	:Factor
	| Exp ADD Factor {$$ = $1 + $3;}
	| Exp SUB Factor {$$ = $1 - $3;}
	;
Factor	: Term
	| Factor MUL Term {$$ = $1 * $3;}
	| Factor DIV Term {$$ = $1 / $3;}
	;
Term	: INT {$$ = $1;}
	| FLOAT {$$ = $1;}
	;
*/
%%

yyerror(char* msg){
	fprintf(stderr, "Error type B at line %d: Syntax error\n" , yylineno);
}
