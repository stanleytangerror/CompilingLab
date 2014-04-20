#ifndef __SEMANTIC_H__
#define __SEMANTIC_H__

#define MAX_VARIABLE 16384
#define NAME_LEN 16

typedef struct Type_ Type;
typedef struct FieldList_ FieldList;
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

struct FieldList_ {
  char name[NAME_LEN];
  Type *type;
  FieldList *tail;
};

extern FieldList* varlist[MAX_VARIABLE];
extern Type * typelist[MAX_VARIABLE];


void InitialHashTable();

//void InitialFieldList(FieldList * variable);

int addVar(FieldList* variable);

int findVar(char *name);

int subtreeDef(node * p, Type * upperlevel, FieldList * fieldspace);

int subtreeDecList(node * p, Type * upperlevel);

void semantic(node * p, Type * upperlevel);

#endif
