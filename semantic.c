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
Type *lefttype = NULL;
Type * typeptr = NULL;
Func * funcptr = NULL;
char currentfuncname[MAXID];
node * idnode = NULL;
char firstid[MAXID];

bool leftmost = true;
bool valid = true;

int args[100];

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

int findType(char *name){
  unsigned int probe = hash_pjw(name);
  while (typelist[probe] != NULL){
    char *Hashname = typelist[probe]->u.structure.name;		
    if ( strcmp(name , Hashname) == 0 ) return probe;
    else probe++;
  }
  return -1;	
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

int findFunc(char *name){
   unsigned int probe = hash_pjw(name);
   while (funclist[probe] != NULL){
   	char * funcname = funclist[probe]->name;
	if (strcmp(funcname , name) == 0) return probe;
	else probe++;
   }
   return -1;
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
		if ( addFunc(func) < 0) printf("Error type 4 at line %d: Redefined function \"%s\"\n", 
			p->child->lineno, p->child->nvalue.value_id);
		funcptr = func;
		semantic(p->child->sibling->sibling , upperlevel , func);
	}
}

int subtreeStructSpecifier(node * p, Type * upperlevel, Func * currentfunc) {
  if (p != NULL && p->child->sibling->sibling != NULL) {
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
  } else {
    int probe = findType(p->child->sibling->child->nvalue.value_id);
	if (probe < 0) {
		printf("Error type 17 at line %d: Undefine structure\n", p->lineno);
	} else {
		typeptr = typelist[probe];
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
	idnode = p->child;
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
      if (addVar(fl) < 0) printf("Error type 3 at line %d: Redefined variable \"%s\"\n" , idnode->lineno , idnode->nvalue.value_id );
    }
    subtreeDecList(subtree->child, upperlevel, isStruct , currentfunc);
    subtreeDecList(subtree->sibling, upperlevel, isStruct , currentfunc);
  }
}

int subtreeStmt(node * p, Type * upperlevel, Func * currentfunc){
    leftmost = true;
    valid = true;
    p = p->child;
    if (p->label == NODE_TERMINATE && p->ntype.type_term == eRETURN){
		printf("functionname : = %s\n" , currentfuncname);
		int probe = findFunc(currentfuncname);
		Type * retype = funclist[probe]->returntype;
		node *q=p;
		q = q->sibling->child;
		if (q->label == NODE_ID){
			probe = findVar(q->nvalue.value_id);
			if (probe < 0) printf("Error type 1 at line %d: Undefined variable \"%s\"\n" , q->lineno , q->nvalue.value_id);
			else{
				if (retype->kind != varlist[probe]->type->kind || retype->u.basic != varlist[probe]->type->u.basic)
				printf("Error type 8 at line %d: The return type mismatched\n" , q->lineno);
			}
		}
		if (q->label == NODE_INT || q->label == NODE_FLOAT){
			int checkbasic = 0;
			if (q->label == NODE_INT) checkbasic = 0;
			else checkbasic =1;
			if (retype->kind != basic || retype->u.basic != checkbasic)
			printf("Error type 8 at line %d: The return type mismatched\n" , q->lineno);
		}
			
	}
    if (p->label == NODE_NONTERMINATE && p->ntype.type_nonterm == Exp){
	printf("Exp-----\n");
	subtreeExp(p , NULL);
	printf("endExp---\n");	
	}
}

bool subtreeArgs(node * p , char * funcname){
	int probe = findFunc(funcname);
	FieldList * param = funclist[probe]->param;
	int paramlist[100];
	int count=1;
	while (param != NULL ){
		if (param->type->kind == basic)
		paramlist[count] = param->type->u.basic;
		count++;
		param = param->tail;
	}
	int i=0;
	count--;	
	for (i=1; i<=count ; i++){
		printf("%d*-*",paramlist[i]);
	}
	printf("\n");
	bool check = true;
	if (args[0] == count) {
		int i=0;		
		for (i=1 ; i<=count ; i++){
			if (args[i] != paramlist[i]) check =false;
		}
	}
	if (args[0] != count || (!check)){
		printf("Error type 9 at line %d: The method \"%s(" , p->lineno , funcname);
		if (count > 0) {
			if (paramlist[1] == 0) printf("int");
			else printf("float");
			int i = 2;
			for (i=2 ; i<= count ; i++) {
				if (paramlist[i] == 0) printf(",int");
				else printf(",float");
				}
		} 
		printf(") is not applicable for the arguments \"(");
		if (args[0]>0){
			if (args[1] == 0) printf("int");
			else printf("float");
			int i = 2;
			for (i=2 ; i<= args[0] ; i++) {
				if (args[i] == 0) printf(",int");
				else printf(",float");
				}
		}
		printf(")\"\n");
		return false;
	}
	return true;	
	
}
void travelArgs(node * p){
	if (p != NULL){
		if (p->label == NODE_NONTERMINATE && p->ntype.type_nonterm == Exp){
			args[0]++;
			int count = args[0];
			if (p->child->label == NODE_INT) args[count] = 0;
			if (p->child->label == NODE_FLOAT) args[count] = 1;
		}
	    travelArgs(p->child);
	    travelArgs(p->sibling);
	}
}
int subtreeExp(node * p){
    if (p == NULL) printf("node is null\n");
    if (p != NULL && valid){
	bool branch = false;
	//identify function
	if (p->child != NULL && p->child->sibling != NULL && p->child->label == NODE_ID && p->child->sibling->ntype.type_term == eLP){
		branch = true;	
		printf("i am in func----\n");	
		node * q = p->child;
		int probe = findFunc(q->nvalue.value_id);
		char *funcname = q->nvalue.value_id;
		if ( probe < 0 ) {
				valid = false;				
				printf("Error type 2 at line %d: Undefined function \"%s\"\n" , q->lineno , q->nvalue.value_id);
			}
		else {
			q = q->sibling->sibling;
			if (q->label == NODE_NONTERMINATE && q->ntype.type_nonterm == Args){
				memset(args , 0 , sizeof(int));
				travelArgs(q);
				int i=0;
				for (i=1; i<=args[0] ; i++) printf("%d--" , args[i]);
				printf("\n");
				
				if ( !subtreeArgs(q,funcname) ){
					subtreeExp(q->sibling);
				}			
			}
			else{
			if (leftmost){
				leftmost = false;
				lefttype = funclist[probe]->returntype;
				subtreeExp(p->child);
				subtreeExp(p->sibling);			
			}
			else{
				if ( (lefttype->kind != funclist[probe]->returntype->kind) || 
						(lefttype->u.basic != funclist[probe]->returntype->u.basic) ){
					printf("Error type 5 at line %d:Type mismatched\n" , p->child->lineno);
					valid = false;				
				}
				else {
		 		subtreeExp(p->child);
				subtreeExp(p->sibling);
			   	}		
			}}		
		}
	}
	//identify ID
	if (p->child != NULL && p->child->label == NODE_ID && p->child->sibling == NULL){
		branch = true;
		printf("i am in var----\n");
		node * q = p->child;
		int probe = findVar(q->nvalue.value_id);
		if ( probe < 0 ){
			valid = false;	
			printf("Error type 1 at line %d: Undefined variable \"%s\"\n" , q->lineno , q->nvalue.value_id);		
		}
		else{
		    if (leftmost){
			leftmost = false;
			lefttype = varlist[probe]->type;
			printf("store left hand type\n");
			printf("lefttype : = %d\n" , lefttype->u.basic);
			subtreeExp(p->child);
			subtreeExp(p->sibling);
			}
		    else{
			if ( (lefttype->kind != varlist[probe]->type->kind) || (lefttype->u.basic != varlist[probe]->type->u.basic) ){
				printf("Error type 5 at line %d:Type mismatched\n" , p->child->lineno);
				valid = false;				
				}
			else {
		 		subtreeExp(p->child);
				subtreeExp(p->sibling);
			     }
			}
		}
	}
	//identify INTorFLOAT
	if (p->child != NULL && (p->child->label == NODE_INT || p->child->label == NODE_FLOAT) ){
		if (leftmost){
			valid = false;			
			printf("Error type 6 at line %d: The left-hand side of an assignment must be a variable\n" , p->child->lineno);	
		}
		else {
			int checkbasic = 0;
			if (p->child->label == NODE_INT) checkbasic = 0;
			else checkbasic = 1;
			printf("i am in INTorFLOAT\n");
			printf("lefttype : = %d , %d\n" , lefttype->kind , lefttype->u.basic);
			if (lefttype->kind != basic || lefttype->u.basic != checkbasic){
				valid = false;
				printf("Error type 5 at line %d:Type mismatched\n" , p->child->lineno);
			}
			else {
			    subtreeExp(p->child);
			    subtreeExp(p->sibling);
			}
		}
	}
	if (!branch) {
			printf("i am in branch----\n");
			subtreeExp(p->child);
			subtreeExp(p->sibling);
		 }
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
	case Stmt:
	  printf("start StmtList\n");
	  subtreeStmt(p , upperlevel , currentfunc);
	  printf("end add StmtList\n");
	  semantic(p->sibling , upperlevel ,currentfunc);
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
