#include "tree.h"

node * forest;

const char * stringNonTerminate [] = { 
 "Program", "ExtDefList", "ExtDef", "ExtDecList",
 "Specifier", "StructSpecifier", "OptTag", "Tag",
 "VarDec", "FunDec", "VarList", "ParamDec",
 "CompSt", "StmtList", "Stmt", "Args",
 "DefList", "Def", "DecList", "Dec", "Exp"
 };

const char * stringTerminate [] = {
 "SEMI", "COMMA", "ASSIGNOP", 
 "PLUS", "MINUS", "STAR", "DIV",
 "AND", "OR", "DOT", "NOT",
 "LP", "RP", "LB", "RB", "LC", "RC",
 "STRUCT", "RETURN", "IF", "ELSE", "WHILE"
};

const char * stringSemanticTerminate [] = {
  "INT", "FLOAT", "RELOP", "TYPE", "ID"
};

const char * stringRelopValue [] = {
  "LT", "LE", "GT", "GE", "EQ", "NE"
};

const char * stringTypeValue [] = {
  "INT", "FLOAT"
};


node * newnode() {
  node * s = (node *) malloc(sizeof(node));
  s->parent = NULL;
  s->child = NULL;
  s->sibling = NULL;
  return s;
}

bool insert(node * parent, node * current) {
  if (parent == NULL) {
    printf("parent node is null\n");
    return false;
  }
  node * p = parent;
  if (p->child == NULL) {
    p->child = current;
  } else {
    p = p->child;
    while (p->sibling != NULL) {
      p = p->sibling;
    }
    p->sibling = current;
    current->parent = parent;
  }
  return true;
}

bool merge(node * parent, int num, ...) {
  node * p = NULL;
  va_list argptr;
  va_start(argptr, num);
  while (num > 0) {
    p = va_arg(argptr, node *);
    if (insert(parent, p) == false) {
      va_end(argptr);
      return false;
    }
    num --;
  }
  va_end(argptr);

  return true;
}

void traversal( node * subtree, int level, void (*func)(node *, int) ) {
  if (subtree != NULL) {
      (* func)(subtree, level);
      traversal(subtree->child, level + 1, func);
      traversal(subtree->sibling, level + 1, func);
  }
}

node * shiftTerminate(enum Terminate terminate, int lineno  ) {
  node * p = newnode();
  p->label = NODE_TERMINATE;
  p->ntype.type_term = terminate;
  p->nvalue.value_int = 0;
  p->lineno = lineno;
  return p;
}

node * shiftInt(int value, int lineno  ) {
  node * p = newnode();
  p->label = NODE_INT;
  p->ntype.type_semterm = eINT;
  p->nvalue.value_int = value;
  p->lineno = lineno;
  return p;
}

node * shiftFloat(float value, int lineno  ) {
  node * p = newnode();
  p->label = NODE_FLOAT;
  p->ntype.type_semterm = eFLOAT;
  p->nvalue.value_float = value;
  p->lineno = lineno;
  return p;
}

node * shiftType(enum TypeValue value, int lineno  ) {
  node * p = newnode();
  p->label = NODE_TYPE;
  p->ntype.type_semterm = eTYPE;
  p->nvalue.value_type = value;
  p->lineno = lineno;
  return p;
}

node * shiftRelop(enum RelopValue value, int lineno  ) {
  node * p = newnode();
  p->label = NODE_RELOP;
  p->ntype.type_semterm = eRELOP;
  p->nvalue.value_relop = value;
  p->lineno = lineno;
  return p;
}

node * shiftId( char * value, int lineno  ) {
  node * p = newnode();
  p->label = NODE_ID;
  p->ntype.type_semterm = eID;
  strncpy(p->nvalue.value_id, value, MAXID);
  p->lineno = lineno;
  return p;
}

node * reduce(enum NonTerminate nonterm, int lineno, int num, ...) {
  node * upper = newnode();
  upper->label = NODE_NONTERMINATE;
  upper->ntype.type_nonterm = nonterm;
  upper->nvalue.value_int = 0;
  upper->lineno = lineno;
  
  node * p = NULL;
  va_list argptr;
  va_start(argptr, num);
  while (num > 0) {
    p = va_arg(argptr, node *);
    if (insert(upper, p) == false) {
      va_end(argptr);
      return false;
    }
    num --;
  }
  va_end(argptr);

  return upper;
}

void printnode(node * p, int indent) {
  if (p == NULL) {
    return;
  }
  while (indent > 0) {
    printf("  ");
    indent --;
  }
  switch (p->label) {
    case NODE_NONTERMINATE:
      printf("%s (%d)\n", stringNonTerminate[p->ntype.type_nonterm], p->lineno);
      break;
    case NODE_INT:
      printf("INT: %d\n", p->nvalue.value_int);
      break;
    case NODE_FLOAT:
      printf("FLOAT: %d\n", p->nvalue.value_float);
      break;
    case NODE_TYPE:
      printf("TYPE: %s\n", stringTypeValue[p->nvalue.value_type]);
      break;
    case NODE_RELOP:
      printf("RELOP: %s\n", stringRelopValue[p->nvalue.value_relop]);
      break;
    case NODE_TERMINATE:
      printf("%s\n", stringTerminate[p->ntype.type_term]);
      break;    
    case NODE_ID:
      printf("ID: %s\n", p->nvalue.value_id);
      break;
    default:
      printf("Invalid\n");
      break;
  }
  return;
}


/*int main () {
  node * sub0 = reduce(
  shiftInt(4, 3),
  shiftInt(5, 3)
    );
  
  shiftInt(1, 1);
  shiftInt(1, 1);
  shiftInt(1, 1);
  shiftInt(1, 1);
  shiftInt(1, 1);
  shiftInt(1, 1);
  shiftInt(1, 1);
  shiftInt(1, 1);
  shiftInt(1, 1);

  node * subtree0 = newnode(2, 2);
  insert(subtree0, newnode(5, 3));
  insert(subtree0, newnode(6, 3));

  node * subtree1 = newnode(3, 2);
  insert(subtree1, newnode(7, 3));

  merge(forest->child, 2, subtree0, subtree1);

  node * subtree2 = newnode(4, 2);
  insert(subtree2, newnode(8, 3));
  insert(subtree2, newnode(9, 3));
  insert(subtree2, newnode(10, 3));

  merge(forest->child, 1, subtree2);

  traversal(forest, printnode);
  return 0;
}*/
