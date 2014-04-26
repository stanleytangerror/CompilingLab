#ifndef __SEMANTIC_H__
#define __SEMANTIC_H__

#define MAX_VARIABLE 16384
#define NAME_LEN 16

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

//void InitialFieldList(FieldList * variable);

int addVar(FieldList* variable);

int findVar(char *name);

int subtreeDef(node * p, Type * upperlevel, Func * currentfunc);

int subtreeDecList(node * p, Type * upperlevel, bool isStruct, Func * currentfunc);

Type * subtreeExp(node * P);

void semantic(node * p, Type * upperlevel , Func * currentfunc);

void gettypelist();

void gettypelist();

#endif
