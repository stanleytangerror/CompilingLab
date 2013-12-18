#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "tree.h"
#include "semantic.h"

FieldList* varlist[MAX_VARIABLE] = {NULL};
Type * typelist[MAX_VARIABLE] = {NULL};
Func * funclist[MAX_VARIABLE] = {NULL};

Type vartype;
Type * typeptr = NULL;
Func * funcptr = NULL;
char currentfuncname[MAXID];

unsigned int hash_pjw(char *name){
  unsigned int val = 0, i;
  for (; *name ; ++ name){
    val = (val << 2) + *name;
    if (i = val & ~0x3fff)
      val = (val ^ (i>>12)) & 0x3fff;
  }
  return val;
}

void InitialHashTable(){
  int i=0;
  for (i=0 ; i<MAX_VARIABLE; i++){
    varlist[i] = NULL ;	
  }
}

void InitialFieldList(FieldList * variable){
  variable->type = NULL;
  variable->tail = NULL;
}

int addType(Type* type){
  char *name = type->u.structure.name;
  unsigned int probe = hash_pjw(name);
  while (typelist[probe] != NULL){
    char *Hashname = typelist[probe]->u.structure.name;		
    if ( strcmp(name , Hashname) != 0 ) probe++;
    else return -1; 	
  }
  typelist[probe] = type;
}

int addVar(FieldList* variable){
  char *name = variable->name;
  unsigned int probe = hash_pjw(name);
  while (varlist[probe] != NULL){
    char *Hashname = varlist[probe]->name;		
    if ( strcmp(name , Hashname) != 0 ) probe++;
    else return -1; 	
  }
  varlist[probe] = variable;
}

int addFunc(Func* func){
  char *name = func->name;
  unsigned int probe = hash_pjw(name);
  while (funclist[probe] != NULL){
  	char *funcname = funclist[probe]->name;
	if (strcmp(funcname , name)!= 0) probe++;
	else return -1;
  }	
  funclist[probe] = func;
}

int findVar(char *name){
  unsigned int probe = hash_pjw(name);
  while (varlist[probe] != NULL){
    char *Hashname = varlist[probe]->name;		
    if ( strcmp(name , Hashname) == 0 ) return probe;
    else probe++;
  }
  return -1;	
}

int subtreeDef(node * p, Type * upperlevel, Func * currentfunc) {
  bool isStruct = false;
  //FieldList * fl = (FieldList *) malloc(sizeof(FieldList));
  vartype.kind = basic;
  vartype.u.basic = 0;
  //fl->type = type;
  assert(p->label == NODE_NONTERMINATE);

  p = p->child;
  //Specifier
  switch (p->child->label) {
    case NODE_TYPE:
      vartype.kind = basic;
      vartype.u.basic = p->child->nvalue.value_type;
      if (vartype.u.basic == 1) printf("float====\n");
      //addDec(p->sibling, Type type, NULL);
      break;
    case NODE_NONTERMINATE:
      assert(p->child->ntype.type_nonterm == StructSpecifier);
      subtreeStructSpecifier(p->child, NULL , currentfunc);
      isStruct = true;
      //vartype.kind = structure;
      //strncpy(vartype.u.structure.name, p->child->sibling->child->nvalue.value_id, MAXID);
      break;
    default:
      printf("error in Def\n");
      break;
  }

  p = p->sibling;
  //Declist
  printf("start add DecList\n");
  subtreeDecList(p, upperlevel, isStruct ,currentfunc);
  printf("end add DecList\n");
  //SEMI
}

int subtreeExtDef(node * p, Type * upperlevel, Func * currentfunc) {
  bool isStruct = false;
  vartype.kind = basic;
  vartype.u.basic = 0;
  //fl->type = type;
  assert(p->label == NODE_NONTERMINATE);

  p = p->child;
  //Specifier
  switch (p->child->label) {
    case NODE_TYPE:
      vartype.kind = basic;
      vartype.u.basic = p->child->nvalue.value_type;
      //addDec(p->sibling, Type type, NULL);
      break;
    case NODE_NONTERMINATE:
      assert(p->child->ntype.type_nonterm == StructSpecifier);
      subtreeStructSpecifier(p->child, upperlevel, NULL);
      isStruct = true;
      //memcpy(&vartype,  = structure;
      //strncpy(vartype.u.structure.name, p->child->sibling->child->nvalue.value_id, MAXID);
      break;
    default:
      printf("error in Def\n");
      break;
  }

  p = p->sibling;
  //Declist
  if (p->ntype.type_nonterm == FunDec){
	printf("start add FuncList\n");
	subtreeFunctionSpecifier(p , upperlevel , currentfunc);
	printf("end add FuncList\n");
	semantic(p->sibling , upperlevel , NULL);
  }else{
  printf("start add DecList\n");
  subtreeDecList(p, upperlevel, isStruct , currentfunc);
  printf("end add DecList\n");
  }//SEMI
  return 0;
}

int subtreeFunctionSpecifier(node *p, Type * upperlevel , Func * currentfunc){
	if (p->label == NODE_NONTERMINATE && p->ntype.type_nonterm == FunDec){
		Func * func = (Func *) malloc (sizeof(Func));
		func->returntype  =(Type *) malloc (sizeof(Type));
		memcpy((void *)func->returntype , (void *)&vartype , sizeof(Type));
		strncpy(func->name , p->child->nvalue.value_id , MAXID);
		strncpy(currentfuncname , p->child->nvalue.value_id , MAXID);
		func->param = NULL;
		addFunc(func);
		funcptr = func;
		semantic(p->child->sibling->sibling , upperlevel , func);
	}
}

int subtreeStructSpecifier(node * p, Type * upperlevel, Func * currentfunc) {
  if (p != NULL && p->child->sibling->sibling->sibling != NULL) {
    node * q = p;
    //StructSpecifier define
    p = p->child;
    assert(p->label == NODE_TERMINATE && p->ntype.type_term == eSTRUCT);
    if (p->sibling->label != NODE_NONTERMINATE) {
      // anonimous struct
      printf("anonimous structure not support currently in beta version\n");      
    } else {
      Type * type = (Type *) malloc(sizeof(Type));
      type->kind = structure;
      strncpy(type->u.structure.name, p->sibling->child->nvalue.value_id, MAXID);
      addType(type);
      typeptr = type;
      semantic(q->child->sibling->sibling->sibling, type , currentfunc);
      typeptr = type; 
    }
  }
}

int subtreeDecList(node * p, Type * upperlevel, bool isStruct , Func * currentfunc) {
  if (p != NULL) {
    printf("p-nodevalue = %s\n" , stringNonTerminate[p->ntype.type_nonterm]);
    node * subtree = p;
    if (p->label == NODE_NONTERMINATE && p->ntype.type_nonterm == VarDec
        && p->child->label == NODE_ID) {
      FieldList * fl = (FieldList *) malloc(sizeof(FieldList));
	printf("isSturct = %d\n" ,isStruct );      
	if (!isStruct) {
        //basic type
        fl->type = (Type *) malloc(sizeof(Type));
        memcpy((void *)fl->type, (void *)&vartype, sizeof(Type));
        strncpy(fl->name, p->child->nvalue.value_id, MAXID);
        fl->tail = NULL;
      } else {
        //basic structrue
        fl->type = typeptr;
        strncpy(fl->name, p->child->nvalue.value_id, MAXID);
        fl->tail = NULL;
      }

      //array
      while (p->parent->label == NODE_NONTERMINATE && p->parent->ntype.type_nonterm == VarDec) {
        p = p->parent;
        Type * type = (Type *) malloc(sizeof(Type)), * temp = fl->type;
        type->kind = array;
        type->u.array.size = p->child->sibling->sibling->nvalue.value_int;

        if (fl->type->kind == basic) {
          type->u.array.elem = fl->type;
          fl->type = type;
        } else {
          while (temp->u.array.elem->kind == array) {
            temp = temp->u.array.elem;
          }
          type->u.array.elem = temp->u.array.elem;
          temp->u.array.elem = type;
        }
      }

      //add structure using space
      if (upperlevel != NULL) {
        Type * upper = upperlevel;
        FieldList * ahead = NULL;
        if (upper->u.structure.structure == NULL) {
          upper->u.structure.structure = fl;
        } else {
          ahead = upper->u.structure.structure;
          while (true) {
            if (ahead->tail == NULL) {
              ahead->tail = fl;
              break;
            }
            ahead = ahead->tail;
          }
        }
      }
      if (currentfunc != NULL){
	    printf("funcname : = %s\n" , currentfunc->name);
	    Func * f = currentfunc;
	    FieldList * ahead = NULL;
	    if (f->param == NULL) {
			printf("p0005\n");			
			f->param = fl;
			printf("p0006\n");
		}
	    else {
		ahead = f->param; 
	       while (ahead->tail != NULL) ahead = ahead->tail;
	       ahead->tail = fl;
      	    }
       }
      addVar(fl);
    }
    subtreeDecList(subtree->child, upperlevel, isStruct , currentfunc);
    subtreeDecList(subtree->sibling, upperlevel, isStruct , currentfunc);
  }
}

void semantic(node * p, Type * upperlevel , Func * currentfunc) {
  if (p != NULL) {
    if (p->label == NODE_NONTERMINATE ) {
      printf("%s (%d)\n", stringNonTerminate[p->ntype.type_nonterm], p->lineno);
      switch (p->ntype.type_nonterm) {
        case Def:
	case ParamDec:
          printf("start add Def\n");
          subtreeDef(p, upperlevel, currentfunc);
          printf("end add Def\n");
          semantic(p->sibling, upperlevel , currentfunc);
          break;
        case ExtDef:
          printf("start add ExtDef\n");
          subtreeExtDef(p, upperlevel, currentfunc);
          printf("end add ExtDef\n");
          semantic(p->sibling, upperlevel , currentfunc);
          break;
        default:
          semantic(p->child, upperlevel , currentfunc);
          semantic(p->sibling, upperlevel , currentfunc);
          break;
      }
    } else {
      semantic(p->child, upperlevel , currentfunc);
      semantic(p->sibling, upperlevel , currentfunc);
    }
  }
}

void getvarlist() {
  int i = 0;
  Type * type = NULL;
  for (i = 0; i < MAX_VARIABLE; i ++) {
    if (varlist[i] != NULL ) {
      printf("varlist[%d]: %s\n", i, varlist[i]->name);
      switch (varlist[i]->type->kind) {
        case basic :
          printf("\tkind: basic\n");
          printf("\tu: %s\n", (varlist[i]->type->u.basic == eINTTYPE) ? "INT" : "FLOAT");
          break;
        case array :
          printf("\tkind: array\n");
          printf("\tu: ");
          type = varlist[i]->type;
          while (type->kind == array) {
            printf("%d ", type->u.array.size);
            type = type->u.array.elem;
          }
          if (type->kind == basic) {
            printf("%s \n", (type->u.basic == eINTTYPE) ? "INT" : "FLOAT");
          } else {
            printf("%s \n", type->u.structure.name);
          }
          break;
        case structure :
          printf("\tkind: structure\n");
          printf("\tu: struct %s\n", varlist[i]->type->u.structure.name);
          break;
        default :
          printf("\tvariable invalid\n");
          break;
      }
    }
  }
}


void gettypelist() {
  int i = 0;
  Type * type = NULL;
  FieldList * member = NULL;
  for (i = 0; i < MAX_VARIABLE; i ++) {
    if (typelist[i] != NULL ) {
      printf("typelist[%d]: struct %s\n", i, typelist[i]->u.structure.name);
      member = typelist[i]->u.structure.structure;
      while (member != NULL) {
        switch (member->type->kind) {
          case basic :
            printf("\tmember: %s\n", member->name);
            printf("\tu: %s\n", (member->type->u.basic == eINTTYPE) ? "INT" : "FLOAT");
            break;
          case array :
            printf("\tmember: %s\n", member->name);
            printf("\tu: ");
            type = member->type;
            while (type->kind == array) {
              printf("%d ", type->u.array.size);
              type = type->u.array.elem;
            }
            if (type->kind == basic) {
              printf("%s \n", (type->u.basic == eINTTYPE) ? "INT" : "FLOAT");
            } else {
              printf("%s \n", type->u.structure.name);
            }
            break;
          case structure :
            printf("\tmember: %s\n", member->name);
            printf("\tu: struct %s\n", member->type->u.structure.name);
            break;
          default :
            printf("\ttype invalid\n");
            break;
        }
        member = member->tail;
      }
    }
  }
}

void getfunclist() {
  int i = 0;
  FieldList * param = NULL;
  Type * type = NULL;
  for (i = 0; i < MAX_VARIABLE; i ++) {
    if (funclist[i] != NULL ) {
      printf("funclist[%d]: %s\n", i, funclist[i]->name);
      switch (funclist[i]->returntype->kind) {
        case basic :
          printf("\treturnkind: basic\n");
          printf("\tu: %s\n", (funclist[i]->returntype->u.basic == eINTTYPE) ? "INT" : "FLOAT");
          break;
        case structure :
          printf("\treturnkind: structure\n");
          printf("\tu: struct %s\n", funclist[i]->returntype->u.structure.name);
          break;
        default :
          printf("\tfuntion invalid\n");
          break;
      }
	printf("paramlist:\n");
      if (funclist[i]->param != NULL) param =funclist[i]->param;
	while (param != NULL){
	switch(param->type->kind){	
	case basic :
          printf("\tkind: basic\n");
          printf("\tu: %s\n", (param->type->u.basic == eINTTYPE) ? "INT" : "FLOAT");
          break;
        case array :
          printf("\tkind: array\n");
          printf("\tu: ");
          type = param->type;
          while (type->kind == array) {
            printf("%d ", type->u.array.size);
            type = type->u.array.elem;
          }
          if (type->kind == basic) {
            printf("%s \n", (type->u.basic == eINTTYPE) ? "INT" : "FLOAT");
          } else {
            printf("%s \n", type->u.structure.name);
          }
          break;
        case structure :
          printf("\tkind: structure\n");
          printf("\tu: struct %s\n", param->type->u.structure.name);
          break;
        default :
          printf("\tvariable invalid\n");
          break;
      	}
	param = param->tail;
  	}
}}}	
