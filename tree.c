#include "tree.h"

node * forest;
bool compile;

const char * stringNonTerminate [] = { 
 "Program", "ExtDefList", "ExtDef", "ExtDecList",
 "Specifier", "StructSpecifier", "OptTag", "Tag",
 "VarDec", "FunDec", "VarList", "ParamDec",
 "CompSt", "StmtList", "Stmt", "Args",
 "DefList", "Def", "DecList", "Dec", "Exp", "Empty"
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
  "int", "float"
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
      traversal(subtree->sibling, level, func);
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

node * shiftType(char * text, int lineno  ) {
  node * p = newnode();
  p->label = NODE_TYPE;
  p->ntype.type_semterm = eTYPE;
  if (strncmp(text, "int", MAXID) == 0) {
    p->nvalue.value_type = eINT;
  } else if (strncmp(text, "float", MAXID) == 0 ) {
    p->nvalue.value_type = eFLOAT;
  }
  p->lineno = lineno;
  return p;
}

node * shiftRelop(char * text, int lineno  ) {
  node * p = newnode();
  p->label = NODE_RELOP;
  p->ntype.type_semterm = eRELOP;
  if (strncmp(text, "<", MAXID) == 0) {
    p->nvalue.value_type = eLT;
  } else if (strncmp(text, "<=", MAXID) == 0 ) {
    p->nvalue.value_type = eLE;
  } else if (strncmp(text, ">", MAXID) == 0 ) {
    p->nvalue.value_type = eGT;
  } else if (strncmp(text, ">=", MAXID) == 0 ) {
    p->nvalue.value_type = eGE;
  } else if (strncmp(text, "==", MAXID) == 0 ) {
    p->nvalue.value_type = eEQ;
  } else if (strncmp(text, "!=", MAXID) == 0 ) {
    p->nvalue.value_type = eNE;
  }
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

//  printf("\nreduce ----- start --\n");
//  traversal(upper, 0, printnode);
//  printf("reduce ----- end --\n\n");
  return upper;
}

void printnode(node * p, int indent) {
  if (p == NULL) {
    return;
  }

  if (p->label != NODE_NONTERMINATE || p->ntype.type_nonterm != Empty) {
    while (indent > 0) {
      printf("  ");
      indent --;
    }
  }
  switch (p->label) {
    case NODE_NONTERMINATE:
      if (p->ntype.type_nonterm != Empty) {
        printf("%s (%d)\n", stringNonTerminate[p->ntype.type_nonterm], p->lineno);
      }
      break;
    case NODE_INT:
      printf("INT: %d\n", p->nvalue.value_int);
      break;
    case NODE_FLOAT:
      printf("FLOAT: %f\n", p->nvalue.value_float);
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

  node * sub0 = reduce(2, 2, 
  shiftInt(4, 3),
  shiftInt(5, 3)
    );
  
  node * sub1 = reduce(3, 2, 
  shiftInt(6, 3),
  shiftInt(7, 3),
  shiftInt(8, 3)
    );

  forest = reduce( 1, 1, 2, sub0, sub1);
*/
/*
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
*/
/*  traversal(forest, 0, printnode);
  
  return 0;
}*/

