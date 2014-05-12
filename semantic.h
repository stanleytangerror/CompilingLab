#ifndef __SEMANTIC_H__
#define __SEMANTIC_H__

#include "tree.h"

#define MAX_VARIABLE 16384
#define NAME_LEN 32

typedef struct Type_ Type;
typedef struct FieldList_ FieldList;
typedef struct Func_ Func;
typedef struct StructType StructType;

struct Type_ {
  enum {basic, array, structure} kind;
  union {
    int basic;
    struct {
      Type* elem;
      int size;
    } array;
    struct {
      char name[NAME_LEN];
      FieldList* structure;
    } structure;
  } u;
};

struct Func_ {
	int funclineno;
	Type * returntype;
	char name[NAME_LEN];
	FieldList * param;
};

struct FieldList_ {
  char name[NAME_LEN];
  Type *type;
  FieldList *tail;
};

extern FieldList* varlist[MAX_VARIABLE];
extern Type * typelist[MAX_VARIABLE];
extern Func * funclist[MAX_VARIABLE];
extern Func * funcdeclist[MAX_VARIABLE];

void InitialHashTable();


int addType(Type* type);
int findType(char *name);
int addVar(FieldList* variable);
int addFunc(Func* func);
int addFuncdec(Func* func);
int findFunc(char *name);
int findFuncDec(char *name);

int findVar(char *name);
bool cmpType(Type * p, Type * q) ;
bool cmpFunc(Func * f1, Func * f2);

int subtreeDef(node * p, Type * upperlevel, Func * currentfunc);
int subtreeExtDef(node * p, Type * upperlevel, Func * currentfunc) ;
int subtreeFunctionSpecifier(node *p, Type * upperlevel , Func * currentfunc);
int subtreeStructSpecifier(node * p, Type * upperlevel, Func * currentfunc) ;

int subtreeDecList(node * p, Type * upperlevel, bool isStruct, Func * currentfunc);
int subtreeStmt(node * p, Type * upperlevel, Func * currentfunc);
bool subtreeArgs(node * p , char * funcname);

Type * subtreeExp(node * P);

void semantic(node * p, Type * upperlevel , Func * currentfunc);

void getvarlist() ;
void gettypelist();
void getfunclist() ;
void getfuncdeclist() ;
void checkfunc();


#endif
