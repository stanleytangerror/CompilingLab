#ifndef __SEMANTIC_H__
#define __SEMANTIC_H__

#define MAX_VARIABLE 10
#define MAX_STRUCTURE 10
#define MAX_FUNCTION 10

typedef struct Type_ * Type;
typedef struct FieldList_ * FieldList;

struct Type_ {
  enum {basic, array, structure} kind;
  union {
    int basic;
    struct {
      Type elem;
      int size;
    } array;
    FieldList structure;
  } u;
};

struct FieldList_ {
  char * name;
  Type type;
  FieldList tail;
};


#endif
