#ifndef __TREE_H__
#define __TREE_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

#define MAXID 16

#define NODE_NONTERMINATE 0x00
#define NODE_TERMINATE 0x01
#define NODE_INT 0x02
#define NODE_FLOAT 0x03
#define NODE_RELOP 0x04
#define NODE_TYPE 0x05
#define NODE_ID 0x06

enum NonTerminate {
 Program, ExtDefList, ExtDef, ExtDecList,
 Specifier, StructSpecifier, OptTag, Tag,
 VarDec, FunDec, VarList, ParamDec,
 CompSt, StmtList, Stmt, Args,
 DefList, Def, DecList, Dec, Exp, Empty
 };

enum Terminate {
 eSEMI, eCOMMA, eASSIGNOP, 
 ePLUS, eMINUS, eSTAR, eDIV,
 eAND, eOR, eDOT, eNOT,
 eLP, eRP, eLB, eRB, eLC, eRC,
 eSTRUCT, eRETURN, eIF, eELSE, eWHILE
};

enum SemanticTerminate {
  eINT, eFLOAT, eRELOP, eTYPE, eID
};

enum RelopValue {
  eLT, eLE, eGT, eGE, eEQ, eNE
};

enum TypeValue {
  eINTTYPE, eFLOATTYPE
};

union nodetype{
  enum NonTerminate type_nonterm;
  enum Terminate type_term;
  enum SemanticTerminate type_semterm;
};

union nodevalue{
  int value_int;
  float value_float;
  char value_id[MAXID];
  enum RelopValue value_relop;
  enum TypeValue value_type;
};

struct node {
  struct node* parent;
  struct node* child;
  struct node* sibling;
  char label;
  union nodetype ntype;
  union nodevalue nvalue;
  int lineno;
};
typedef struct node node;

extern node * forest;
extern bool compile;

node * newnode();

bool insert(node * parent, node * current);

bool merge(node * parent, int num, ...);

void traversal( node * subtree, int level, void (*func)(node *, int) );

node * shiftTerminate(enum Terminate terminate, int lineno  );

node * shiftInt(int value, int lineno  );

node * shiftFloat(float value, int lineno  );

node * shiftType(char * text, int lineno  );

node * shiftRelop(char * text, int lineno  );

node * shiftId( char * value, int lineno  );

node * reduce(enum NonTerminate nonterm, int lineno, int num, ...);

void printinfo(node * p, int indent);

  void printnode(node * p, int indent);

#endif
