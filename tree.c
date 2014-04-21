#include "tree.h"
#include "semantic.h"

node * forest;

int vartemp;
Type vartype;
FieldList * variable;

bool compile;
bool fDef = false , fStmt = false , fFunDec = false ,
     fParamDec = false, fStructSpecifier = false;

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
    current->parent = parent;
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

  //if (p->label != NODE_NONTERMINATE || p->ntype.type_nonterm != Empty) {
    while (indent > 0) {
      printf("  ");
      indent --;
    }
  //}
  switch (p->label) {
    case NODE_NONTERMINATE:
      //if (p->ntype.type_nonterm != Empty) {
        printf("%s (%d)\n", stringNonTerminate[p->ntype.type_nonterm], p->lineno);
      //}
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
/*
void addstructurevariable(node * p, int indent) {
  switch (p->label) {
    case NODE_NONTERMINATE:
      switch (p->ntype.type_nonterm) {
        case Def:
          p = p->child;
          
          break;
      }
      break;
  }
}

FieldList * addDef(node * p, FieldList * upperlevel) {

}

FieldList * addstructure(node * p) {
  FieldList * fl = (FieldList *) malloc(sizeof(FieldList));
  fl->type = (Type *) malloc(sizeof(Type));
  fl->tail = NULL;
  fl->type->kind = structure;
  //fl->type->u.structure = NULL;
  p = p->child->sibling;
  memcpy(fl->name, p->child->nvalue.value_id, MAXID);
}

void structuresubtree(node * p) {
  fStructSpecifier = true;
  
  FieldList * fl = (FieldList *) malloc(sizeof(FieldList));
  fl->type = (Type *) malloc(sizeof(Type));
  fl->tail = NULL;
  fl->type->kind = structure;
  //fl->type->u.structure = NULL;
  p = p->child->sibling;
  memcpy(fl->name, p->child->nvalue.value_id, MAXID);

  fStructSpecifier = false;
}
*/
/*void addvariable(node * p , int indent) {
  if (p == NULL) {
    return;
  }
  if (fStructSpecifier) {
    return;
  }
  switch (p->label) {
    case NODE_NONTERMINATE:
      if (p->ntype.type_nonterm == StructSpecifier) {
//        structuresubtree(p);
        break;
      }

      if (p->ntype.type_nonterm == Def) {
        printf("%s (%d)\n", stringNonTerminate[p->ntype.type_nonterm], p->lineno);
        //variable = (FieldList *) malloc(sizeof(FieldList));
        fDef = true;
      }
      //if (p->ntype)
      if (p->ntype.type_nonterm == Stmt) {
        printf("%s (%d)\n", stringNonTerminate[p->ntype.type_nonterm], p->lineno);
        fStmt = true;
      }
      if (p->ntype.type_nonterm == FunDec) {
        printf("%s (%d)\n", stringNonTerminate[p->ntype.type_nonterm], p->lineno);
        fFunDec = true;
      }
      if (p->ntype.type_nonterm == ParamDec && fFunDec){
        fParamDec = true;
      }
      if (p->ntype.type_nonterm == VarDec){
        if (p->child->label == NODE_ID) {
          vartype.kind = basic;
          vartype.u.basic = vartemp; 			
        } else {
          vartype.kind = array;
        }
      }
      break;
    case NODE_INT:
      printf("INT: %d\n", p->nvalue.value_int);
      variable->type->kind = basic;
      variable->type->u.basic = 1;
      break;
    case NODE_FLOAT:
      printf("FLOAT: %f\n", p->nvalue.value_float);
      break;
    case NODE_TYPE:
      printf("TYPE: %s\n", stringTypeValue[p->nvalue.value_type]);
      if (fDef){
        vartemp = p->nvalue.value_type;	
      }
      if (fParamDec){
        vartemp = p->nvalue.value_type;	
      }
      break;
    case NODE_RELOP:
      printf("RELOP: %s\n", stringRelopValue[p->nvalue.value_relop]);
      break;
    case NODE_TERMINATE:
      printf("%s\n", stringTerminate[p->ntype.type_term]);
      if (p->ntype.type_term == eSEMI){
        fDef = false;
        printf("Def finish \n");	
      }
      break;    
    case NODE_ID:
      printf("ID: %s\n", p->nvalue.value_id);
      if (fDef){	
        variable = (FieldList *) malloc(sizeof(FieldList));
        InitialFieldList(variable);
        variable->type = &vartype;
        strcpy(variable->name , p->nvalue.value_id);
        //printf("HashVarName: %s\n", variable->name);
        int probe = addVar(variable);
        if (probe > 0){
          printf("probe : = %d\n" , probe);
          printf("hash[].name : = %s\n" , varlist[probe]->name);
          printf("hash[].kind : = %d\n" , varlist[probe]->type->kind);
          printf("hash[].type : = %d\n" , varlist[probe]->type->u.basic);
        }
        else{
          printf("Error type 3 at line %d: Redefined variable \"%s\"\n " ,
              p->lineno , p->nvalue.value_id);
        }
      }
      if (fStmt){
        if (findVar(p->nvalue.value_id) < 0){
          printf("Error type 1 at line %d: Undefined variable \"%s\"\n " ,
              p->lineno , p->nvalue.value_id);	
        }	
      }
      if (fFunDec){
        if (findVar(p->nvalue.value_id) < 0){
          printf("Error type 4 at line %d: Redefined function \"%s\"\n " ,
              p->lineno , p->nvalue.value_id);
          fFunDec = false;
        }
        else{
          variable = (FieldList *) malloc(sizeof(FieldList));
          strcpy(variable->name , p->nvalue.value_id);
        }	
      }
      if (fParamDec){
        FieldList * p = variable;
        bool f=true;	   
        //while (p!=NULL){
        //  if (strcmp(p->name , ))	
        //}	
      }
      break;
    default:
      printf("Invalid\n");
      break;
  }
  return;
}
*/
