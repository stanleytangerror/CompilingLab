%{
	#include <stdio.h>
  #include "lex.yy.c"
%}
%locations

/* declared types*/
%union{
	struct node * type_node;
}

/* declared tokens*/
%token <type_node> INT
%token <type_node> FLOAT
%token <type_node> ID OCT HEX NUM
%token <type_node> SEMI COMMA ASSIGNOP RELOP
%token <type_node> PLUS MINUS STAR DIV
%token <type_node> AND OR DOT NOT TYPE
%token <type_node> LP RP LB RB LC RC
%token <type_node> STRUCT RETURN IF ELSE WHILE

/* declared non-terminals */
%type <type_node> Program ExtDefList ExtDef ExtDecList
%type <type_node> Specifier StructSpecifier OptTag Tag
%type <type_node> VarDec FunDec VarList ParamDec
%type <type_node> CompSt StmtList Stmt Args
%type <type_node> DefList Def DecList Dec Exp

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
Program		: ExtDefList  { forest = reduce(Program, @$.first_line, 1 , $1);  }
		;
ExtDefList	: ExtDef ExtDefList  { $$ = reduce(ExtDefList, @$.first_line, 2 , $1, $2);  }
		| /*empty*/ { $$ = reduce(Empty, @$.first_line, 0);  }
		;
ExtDef		: Specifier ExtDecList SEMI  { $$ = reduce(ExtDef, @$.first_line, 3 , $1, $2, $3);  }
		| Specifier SEMI  { $$ = reduce(ExtDef, @$.first_line, 2 , $1, $2);  }
		| Specifier FunDec CompSt  { $$ = reduce(ExtDef, @$.first_line, 3 , $1, $2, $3);  }
		;
ExtDecList 	: VarDec  { $$ = reduce(ExtDecList, @$.first_line, 1 , $1);  }
		| VarDec COMMA ExtDecList  { $$ = reduce(ExtDecList, @$.first_line, 3 , $1, $2, $3);  }
		;
Specifier	: TYPE  { $$ = reduce(Specifier, @$.first_line, 1 , $1);  }
		| StructSpecifier  { $$ = reduce(Specifier, @$.first_line, 1 , $1);  }
		;
StructSpecifier : STRUCT OptTag LC DefList RC  { $$ = reduce(StructSpecifier, @$.first_line, 5 , $1, $2, $3, $4, $5);  }
		| STRUCT Tag  { $$ = reduce(StructSpecifier, @$.first_line, 2 , $1, $2);  }
		;
OptTag		: ID  { $$ = reduce(OptTag, @$.first_line, 1 , $1);  }
		| /*empty*/ { $$ = reduce(Empty, @$.first_line, 0);  }
		;
Tag		: ID  { $$ = reduce(Tag, @$.first_line, 1 , $1);  }
		;
VarDec		: ID  { $$ = reduce(VarDec, @$.first_line, 1 , $1);  }
		| VarDec LB INT RB  { $$ = reduce(VarDec, @$.first_line, 4 , $1, $2, $3, $4);  }
		;
FunDec		: ID LP VarList RP  { $$ = reduce(FunDec, @$.first_line, 4 , $1, $2, $3, $4);  }
		| ID LP RP  { $$ = reduce(FunDec, @$.first_line, 3 , $1, $2, $3);  }
		;
VarList		: ParamDec COMMA VarList  { $$ = reduce(VarList, @$.first_line, 3 , $1, $2, $3);  }
		| ParamDec  { $$ = reduce(VarList, @$.first_line, 1 , $1);  }
		;
ParamDec	: Specifier VarDec  { $$ = reduce(ParamDec, @$.first_line, 2 , $1, $2);  }
		;
CompSt 		: LC DefList StmtList RC  { $$ = reduce(CompSt, @$.first_line, 4 , $1, $2, $3, $4);  }
		;
StmtList	: Stmt StmtList  { $$ = reduce(StmtList, @$.first_line, 2 , $1, $2);  }
		| /*empty*/ { $$ = reduce(Empty, @$.first_line, 0);  }
		;
Stmt 		: Exp SEMI  { $$ = reduce(Stmt, @$.first_line, 2 , $1, $2);  }
		| CompSt  { $$ = reduce(Stmt, @$.first_line, 1 , $1);  }
		| RETURN Exp SEMI  { $$ = reduce(Stmt, @$.first_line, 3 , $1, $2, $3);  }
		| IF LP Exp RP Stmt   %prec LOWER_THAN_ELSE  { $$ = reduce(Stmt, @$.first_line, 5 , $1, $2, $3, $4, $5);  }
		| IF LP Exp RP Stmt ELSE Stmt  { $$ = reduce(Stmt, @$.first_line, 7 , $1, $2, $3, $4, $5, $6, $7);  }
		| WHILE LP Exp RP Stmt  { $$ = reduce(Stmt, @$.first_line, 5 , $1, $2, $3, $4, $5);  }
		;
DefList		: Def DefList  { $$ = reduce(DefList, @$.first_line, 2 , $1, $2);  }
		| /*empty*/ { $$ = reduce(Empty, @$.first_line, 0);  }
		;
Def 		: Specifier DecList SEMI  { $$ = reduce(Def, @$.first_line, 3 , $1, $2, $3);  }
		;
DecList 	: Dec  { $$ = reduce(DecList, @$.first_line, 1 , $1);  }
		| Dec COMMA DecList  { $$ = reduce(DecList, @$.first_line, 3 , $1, $2, $3);  }
		;
Dec 		: VarDec  { $$ = reduce(Dec, @$.first_line, 1 , $1);  }
		| VarDec ASSIGNOP Exp  { $$ = reduce(Dec, @$.first_line, 3 , $1, $2, $3);  }
		;
Exp 		: Exp ASSIGNOP Exp  { $$ = reduce(Exp, @$.first_line, 3 , $1, $2, $3);  }
		| Exp AND Exp  { $$ = reduce(Exp, @$.first_line, 3 , $1, $2, $3);  }
		| Exp OR Exp  { $$ = reduce(Exp, @$.first_line, 3 , $1, $2, $3);  }
		| Exp RELOP Exp  { $$ = reduce(Exp, @$.first_line, 3 , $1, $2, $3);  }
		| Exp PLUS Exp  { $$ = reduce(Exp, @$.first_line, 3 , $1, $2, $3);  }
		| Exp MINUS Exp  { $$ = reduce(Exp, @$.first_line, 3 , $1, $2, $3);  }
		| Exp STAR Exp  { $$ = reduce(Exp, @$.first_line, 3 , $1, $2, $3);  }
		| Exp DIV Exp  { $$ = reduce(Exp, @$.first_line, 3 , $1, $2, $3);  }
		| LP Exp RP  { $$ = reduce(Exp, @$.first_line, 3 , $1, $2, $3);  }
		| MINUS Exp  { $$ = reduce(Exp, @$.first_line, 2 , $1, $2);  }
		| NOT Exp  { $$ = reduce(Exp, @$.first_line, 2 , $1, $2);  }
		| ID LP Args RP  { $$ = reduce(Exp, @$.first_line, 4 , $1, $2, $3, $4);  }
		| ID LP RP  { $$ = reduce(Exp, @$.first_line, 3 , $1, $2, $3);  }
		| Exp LB Exp RB  { $$ = reduce(Exp, @$.first_line, 4 , $1, $2, $3, $4);  }
		| Exp DOT ID  { $$ = reduce(Exp, @$.first_line, 3 , $1, $2, $3);  }
		| ID  { $$ = reduce(Exp, @$.first_line, 1 , $1);  }
		| INT  { $$ = reduce(Exp, @$.first_line, 1 , $1);  }
		| FLOAT  { $$ = reduce(Exp, @$.first_line, 1 , $1);  }
		;
Args 		: Exp COMMA Args  { $$ = reduce(Args, @$.first_line, 3 , $1, $2, $3);  }
		| Exp  { $$ = reduce(Args, @$.first_line, 1 , $1);  }
		;

%%

yyerror(char* msg){
	fprintf(stderr, "Error type B at line %d: Syntax error\n" , yylineno);
}
