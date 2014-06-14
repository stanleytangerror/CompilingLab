#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "tree.h"
#include "semantic.h"

FieldList* varlist[MAX_VARIABLE] = {NULL};
Type * typelist[MAX_VARIABLE] = {NULL};
Func * funclist[MAX_VARIABLE] = {NULL};
Func * funcdeclist[MAX_VARIABLE] = {NULL};

Type vartype;
Type exptypecontent;
//Type * exptype = NULL;
Type * lefttype = NULL;
Type * typeptr = NULL;
Func * funcptr = NULL;
char currentfuncname[MAXID];
node * idnode = NULL;
char firstid[MAXID];

int anonymouscount = 0;

bool leftmost = true;
bool valid = true;
bool decfunc = false;
bool decconsistent = false;
bool assignop = false;
bool freturn = false;
bool fstmt = true;
bool fexp = true;
bool fdecexp = false;

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

void initialDefaultFunc(){
	char wname[NAME_LEN]="write";
	char rname[NAME_LEN]="read";
	Func * func = (Func *) malloc (sizeof(Func));
    	func->returntype  =(Type *) malloc (sizeof(Type));
    	strncpy(func->name , wname , NAME_LEN);
    	func->param = NULL; 
	addFunc(func);
	Func * func1 = (Func *) malloc (sizeof(Func));
    	func1->returntype  =(Type *) malloc (sizeof(Type));
    	strncpy(func1->name , rname , NAME_LEN);
    	func1->param = NULL; 
	addFunc(func1);
}

int addFuncdec(Func* func){
  char *name = func->name;
  unsigned int probe = hash_pjw(name);
  while (funcdeclist[probe] != NULL){
    char *funcname = funcdeclist[probe]->name;
    if (strcmp(funcname , name)!= 0) probe++;
    else return -1;
  }	
  funcdeclist[probe] = func;
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

int findFuncDec(char *name){
  unsigned int probe = hash_pjw(name);
  while (funcdeclist[probe] != NULL){
    char * funcname = funcdeclist[probe]->name;
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

bool cmpType(Type * p, Type * q) {
  if (p == NULL || q == NULL || p->kind != q->kind) {
    return false;
  }
  switch (p->kind) {
    case basic:
      return (p->u.basic == q->u.basic);
    case array:
      p = p->u.array.elem;
      q = q->u.array.elem;
      return cmpType(p, q);
    case structure:
      if (strncmp(p->u.structure.name, q->u.structure.name, MAXID) == 0) {
        return true;
      } else {
        return false;
      }
    default:
      printf("kind invalid\n");
      return false;
  }
}

bool cmpFunc(Func * f1, Func * f2){
  bool F = true;
  FieldList * p1= NULL;
  FieldList * p2 = NULL;
  if (strcmp(f1->name , f2->name) != 0) return false;
  if ( !cmpType( f1->returntype , f2->returntype) ) return false;
  p1 = f1->param;
  p2 = f2->param;	
  while (true){
    if (p1 != NULL && p2 != NULL){
      if (strcmp(p1->name , p2->name) != 0) return false;
      if (! cmpType(p1->type , p2->type)) return false;
      else {
        p1 = p1->tail;
        p2 = p2->tail;
      }		
    }
    if (p1 != NULL && p2 == NULL) return false;
    if (p1 == NULL && p2 != NULL) return false;
    if (p1 == NULL && p2 == NULL) return true;
  }
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
  subtreeDecList(p, upperlevel, isStruct ,currentfunc);
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
    if (p->sibling->label == NODE_NONTERMINATE){
      subtreeFunctionSpecifier(p , upperlevel , currentfunc);
      semantic(p->sibling , upperlevel , NULL);
    }
    if (p->sibling->label == NODE_TERMINATE){
      decfunc = true ;
      subtreeFunctionSpecifier(p , upperlevel , currentfunc);
      if (decconsistent){
        int probe = findFuncDec(funcptr->name);
        assert( probe>=0);
        if (!cmpFunc( funcptr , funcdeclist[probe])){
          printf("Error type 19 at line %d: Inconsistent declaration of function \"%s\"\n",
              funcptr->funclineno , funcptr->name);
        }	
        decconsistent = false;	
      }
      decfunc = false;
      semantic(p->sibling , upperlevel , NULL);	
    }
  } 
  else{
    subtreeDecList(p, upperlevel, isStruct , currentfunc);
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
    func->funclineno = p->child->lineno;
    int probe = findFunc(func->name);
    if (!decfunc && addFunc(func) < 0) printf("Error type 4 at line %d: Redefined function \"%s\"\n", 
        p->child->lineno, p->child->nvalue.value_id);
    if (decfunc) {
      if ( addFuncdec(func) < 0 ){
        decconsistent = true;
      } 
    }
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
    assert (p->sibling->label == NODE_NONTERMINATE);
    if (p->sibling->ntype.type_nonterm == Empty) {
      // anonymous struct
      Type * type = (Type *) malloc(sizeof(Type));
      type->kind = structure;
      memcpy(type->u.structure.name, "AnonymousStructure", MAXID);
      sprintf((type->u.structure.name + 18), "%2d", anonymouscount ++);
      type->u.structure.structure = NULL;
      if (findType(type->u.structure.name) >= 0 || findVar(type->u.structure.name) >= 0) {
        assert(false);
        printf("Error type 16 at line %d: Duplicate name \"%s\"\n", p->lineno, type->u.structure.name);
        return -1;
      }
      addType(type);
      typeptr = type;
      semantic(q->child->sibling->sibling->sibling, type , currentfunc);
      typeptr = type; 
    } else {
      Type * type = (Type *) malloc(sizeof(Type));
      type->kind = structure;
      strncpy(type->u.structure.name, p->sibling->child->nvalue.value_id, MAXID);
      type->u.structure.structure = NULL;
      if (findType(type->u.structure.name) >= 0 || findVar(type->u.structure.name) >= 0) {
        printf("Error type 16 at line %d: Duplicate name \"%s\"\n", p->lineno, type->u.structure.name);
        return -1;
      }
      addType(type);
      typeptr = type;
      semantic(q->child->sibling->sibling->sibling, type , currentfunc);
      typeptr = type; 
    }
  } else {
    int probe = findType(p->child->sibling->child->nvalue.value_id);
    if (probe < 0) {
      printf("Error type 17 at line %d: Undefine structure\n", p->lineno);
      typeptr = NULL;
    } else {
      typeptr = typelist[probe];
    }
  }
}

int subtreeDecList(node * p, Type * upperlevel, bool isStruct , Func * currentfunc) {
  if (p != NULL) {
    node * subtree = p;
    bool redefinefield = false;
    if (p->label == NODE_NONTERMINATE && p->ntype.type_nonterm == VarDec
        && p->child->label == NODE_ID) {
      FieldList * fl = (FieldList *) malloc(sizeof(FieldList));
      idnode = p->child;
      if (!isStruct) {
        //basic type
        fl->type = (Type *) malloc(sizeof(Type));
        memcpy((void *)fl->type, (void *)&vartype, sizeof(Type));
        strncpy(fl->name, p->child->nvalue.value_id, MAXID);
        fl->tail = NULL;
        lefttype = fl->type;
      } else {
        //basic structrue
        if (typeptr == NULL) {
          return -1;
        }
        fl->type = typeptr;
        strncpy(fl->name, p->child->nvalue.value_id, MAXID);
        fl->tail = NULL;
        lefttype = fl->type;
      }

      //array
      while (p->parent->label == NODE_NONTERMINATE && p->parent->ntype.type_nonterm == VarDec) {
        p = p->parent;
        Type * type = (Type *) malloc(sizeof(Type)), * temp = fl->type;
        type->kind = array;
        type->u.array.size = p->child->sibling->sibling->nvalue.value_int;

        if (fl->type->kind == basic || fl->type->kind == structure) {
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
      lefttype = fl->type;

      //add structure using space
      if (upperlevel != NULL) {
        Type * upper = upperlevel;
        FieldList * ahead = NULL;
        if (upper->u.structure.structure == NULL) {
          upper->u.structure.structure = fl;
        } else {
          ahead = upper->u.structure.structure;
          while (true) {
            if (strncmp(fl->name, ahead->name, MAXID) == 0) {
              printf("Error type 15 at line %d: Redefine field \"%s\"\n", p->lineno, fl->name);
              redefinefield = true;
              break;
            }
            if (ahead->tail == NULL) {
              ahead->tail = fl;
              break;
            }
            ahead = ahead->tail;
          }
        }
      }

      //add function parameters
      if (currentfunc != NULL){
        Func * f = currentfunc;
        FieldList * ahead = NULL;
        if (f->param == NULL) {
          f->param = fl;
        }
        else {
          ahead = f->param; 
          while (ahead->tail != NULL) ahead = ahead->tail;
          ahead->tail = fl;
        }
      }
      if (!decfunc && addVar(fl) < 0 && !redefinefield)
        printf("Error type 3 at line %d: Redefined variable \"%s\"\n" , idnode->lineno , idnode->nvalue.value_id );
      // initial when define
      if (p->sibling != NULL && p->sibling->ntype.type_term == eASSIGNOP) {
        if (upperlevel != NULL && upperlevel->kind == structure) {
          // in structure field
          printf("Error type 15 at line %d: Initialize in structure field\n", p->lineno);
        } else {
          if (!isStruct) {
            leftmost = false;
            fdecexp = true;
            subtreeExp(p->sibling->sibling);
            fdecexp = false;
          } else {
          }
        }
      }
    }
    subtreeDecList(subtree->child, upperlevel, isStruct , currentfunc);
    subtreeDecList(subtree->sibling, upperlevel, isStruct , currentfunc);
  }
}

int subtreeStmt(node * p, Type * upperlevel, Func * currentfunc){
  leftmost = true;
  valid = true;
  assignop = false;
  freturn = false;
  fstmt = false;
  fexp = false;
  p = p->child;
  bool branch = false;
  if (p != NULL){
    if (p->sibling != NULL && p->sibling->label == NODE_TERMINATE && p->sibling->ntype.type_term == eLP){
      fstmt = true;
      subtreeExp(p->sibling->sibling->child);
    }
    if (p->label == NODE_TERMINATE && p->ntype.type_term == eRETURN){
      int probe = findFunc(currentfuncname);
      Type * retype = funclist[probe]->returntype;
      freturn = true;
      leftmost = false;
      lefttype = retype;
      subtreeExp(p->sibling);
      freturn = false;  
    }
    if (p->label == NODE_NONTERMINATE && p->ntype.type_nonterm == Exp){
      fexp = true;
      subtreeExp(p);
    } 
  }
}

bool subtreeArgs(node * p , char * funcname){
  int probe = findFunc(funcname);
  bool checkargs = true;
  FieldList * param = funclist[probe]->param;
  Type argtype;
  node * temp = p;
  // get all args
  while (temp != NULL && param != NULL && checkargs) {
    /*
       if (temp->child->child->label == NODE_INT) {
       argtype.kind = basic;
       argtype.u.basic = eINTTYPE;
       } else if (temp->child->child->label == NODE_FLOAT) {
       argtype.kind = basic;
       argtype.u.basic = eFLOATTYPE;
       }*/
    if ( !cmpType(subtreeExp(temp->child), param->type) ) {
      checkargs = false;
    } else {
      temp = (temp->child->sibling != NULL) ? temp->child->sibling->sibling : NULL;
      param = param->tail;
    }
  }
  if (temp != NULL || param != NULL) {
    checkargs = false;
  }
  /*if ( !checkargs ) {
    bool f = true;
    if (strcmp(funcname , "write") == 0 ) f = false;
    if (strcmp(funcname , "read") == 0) f = false;
 	if (f){
    printf("Error type 9 at line %d: The method \"%s\" is not applicable for the arguments\n" ,
        p->lineno , funcname);
	}
    return false;
  } else {
    return true;
  }*/
  /*
     int paramlist[100];
     int count=1;
     while (param != NULL ){
     if (param->type->kind == basic)
     paramlist[count] = param->type->u.basic;
     else paramlist[count] = 2;
     count++;
     param = param->tail;
     }
     int i=0;
     count--;	
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
     if (paramlist[1] == 1) printf("float");
     if (paramlist[1] == 2) printf("structure");
     int i = 2;
     for (i=2 ; i<= count ; i++) {
     if (paramlist[i] == 0) printf(",int");
     if (paramlist[i] == 1) printf(",float");
     if (paramlist[i] == 2) printf(",structure");
     }
     } 
     printf(")\" is not applicable for the arguments.\n");
     return false;
     }
     return true;	
     */
}
/*
   void travelArgs(node * p){
   if (p != NULL){
   if (p->label == NODE_NONTERMINATE && p->ntype.type_nonterm == Exp){
   args[0]++;
   int count = args[0];
   subtreeExp(p);
   if (lefttype->kind == 0) {
   if (lefttype->u.basic == 0) args[count] = 0;
   else args[count] = 1;
   }
   if (lefttype->kind == 1) {
   args[count] = 0;		
//while (lefttype->u.array->kind != basic) lefttype = lefttype->u.array.elem
}
if (lefttype->kind == 2) args[count] = 2;
}
travelArgs(p->child);
travelArgs(p->sibling);
}
}
*/
Type * subtreeExp(node * p){
  if (p != NULL && valid){
    if (p->label == NODE_NONTERMINATE && p->ntype.type_nonterm == Exp &&p->child->label == NODE_TERMINATE && p->child->ntype.type_term == eMINUS) {
      return subtreeExp(p->child->sibling);
    }
    Type * exptype = NULL;
    bool branch = false;
    //identify AND
    if (p->label == NODE_TERMINATE && (p->ntype.type_term == eAND || p->ntype.type_term == eOR || p->ntype.type_term == eNOT) ){
      branch = true;
      if (lefttype->kind != 0 || lefttype->u.basic !=0 ){
        valid = false;
        printf("Error type 7 at line %d: Operands type mismatched\n", p->lineno);
      }
    }
    //identify ASSIGNOP
    if (p->label == NODE_TERMINATE && p->ntype.type_term == eASSIGNOP){
      assignop = true;
      node * q = p->parent->child;
      bool check = false;
      if (q->label == NODE_NONTERMINATE && q->ntype.type_nonterm == Exp){
        if (q->child->label == NODE_ID && q->child->sibling == NULL)
          check = true;
        if (q->child->label == NODE_NONTERMINATE && q->child->ntype.type_nonterm == Exp && q->child->sibling != NULL ){
          if (q->child->sibling->label == NODE_TERMINATE && (q->child->sibling->ntype.type_term == eLB || q->child->sibling->ntype.type_term == eDOT))
            check =true;
        }
      }
      if (check){
        subtreeExp(p->child);
        subtreeExp(p->sibling);
      }
      else {
        valid = false;
        printf("Error type 6 at line %d: The left-hand side of an assignment must be a variable\n" , p->lineno);
      }
    }
    //identify function
    if (p->child != NULL && p->child->sibling != NULL && p->child->label == NODE_ID && p->child->sibling->ntype.type_term == eLP) {
      branch = true;	
      node * q = p->child;
      int probe = findFunc(q->nvalue.value_id);
      char *funcname = q->nvalue.value_id;
      if ( probe < 0 ) {
        valid = false;
        if (findVar(funcname) > 0 )
          printf("Error type 11 at line %d: \"%s\" must be a function\n"  ,q->lineno , funcname);		
        //else
        //  printf("Error type 2 at line %d: Undefined function \"%s\"\n" , q->lineno , q->nvalue.value_id);
      } else {
        q = q->sibling->sibling;
        // if (q->label == NODE_NONTERMINATE && q->ntype.type_nonterm == Args) 
        if ((q->label == NODE_TERMINATE && q->ntype.type_term == eRP) || (q->label == NODE_NONTERMINATE && q->ntype.type_nonterm == Args)) {
          // memset(args , 0 , sizeof(int));
          // travelArgs(q);
          int i=0;
          if ( !subtreeArgs(q,funcname) ) {
            valid = false;
            subtreeExp(q->sibling);
          }			
        }
        exptype = funclist[probe]->returntype;
        if (valid && leftmost){
          leftmost = false;
          lefttype = funclist[probe]->returntype;
          subtreeExp(p->child);
          subtreeExp(p->sibling);			
        } else if (valid) {
          if ( (lefttype->kind != funclist[probe]->returntype->kind) 
              || (lefttype->u.basic != funclist[probe]->returntype->u.basic) ) {
            //printf("Error type 5 at line %d: Type mismatched\n" , p->child->lineno);
            valid = false;				
          } else {
            subtreeExp(p->child);
            subtreeExp(p->sibling);
          }		
        }	
      }
    }
    //identify ID
    if (p->child != NULL && p->child->label == NODE_ID && p->child->sibling == NULL){
      branch = true;
      node * q = p->child, * tempnode = p;
      Type * temptype, * childtype;
      FieldList * mem;
      int probe = findVar(q->nvalue.value_id);
      if ( probe < 0 ){
        valid = false;	
        printf("Error type 1 at line %d: Undefined variable \"%s\"\n" , p->child->lineno , p->child->nvalue.value_id);		
      } else {
        // get whole semantic unit
        temptype = varlist[probe]->type;
        // array or structure
        while (valid && tempnode->sibling != NULL) {
          if (tempnode->sibling != NULL && tempnode->sibling->ntype.type_term == eLB) {
            // array
            if (temptype->kind == array) {
              if (tempnode->sibling->sibling->child->ntype.type_term == eFLOAT) {
                printf("Error type 12 at line %d: Operands type mistaken\n", tempnode->lineno);
                valid = false;
                break;
              } else {
                assert(tempnode->sibling->sibling->ntype.type_nonterm == Exp);
                childtype = subtreeExp(tempnode->sibling->sibling);
                leftmost = true;
                if (childtype != NULL && (childtype->kind != basic || childtype->u.basic == eFLOAT) ) {
                  printf("Error type 12 at line %d: Operands type mistaken\n", tempnode->lineno);
                  valid = false;
                  break;
                }
              }
              temptype = temptype->u.array.elem;
              tempnode = tempnode->parent;
            } else {
              valid = false;
              printf("Error type 10 at line %d: \"%s\" must be an array\n" , p->child->lineno , p->child->nvalue.value_id);		
              break;
            }
          } else if (tempnode->sibling != NULL && tempnode->sibling->ntype.type_term == eDOT) {
            // structure
            if (temptype->kind == structure) {
              mem = temptype->u.structure.structure;
              while (mem != NULL) {
                if (strncmp(mem->name, tempnode->sibling->sibling->nvalue.value_id, MAXID) == 0) {
                  temptype = mem->type;
                  break;
                }
                mem = mem->tail;
              }
              if ( mem == NULL) {
                valid = false;
                printf("Error type 14 at line %d: Unexisted field \"%s\"\n" , 
                    p->child->lineno, tempnode->sibling->sibling->nvalue.value_id);
                break;
              }
              tempnode = tempnode->parent;
            } else {
              valid = false;
              printf("Error type 13 at line %d: Illegal use of \".\"\n" , p->child->lineno);
              break;
            }
          } else
            break;
        }
        exptype = temptype;
        // type compare
        if (leftmost){
          leftmost = false;
          lefttype = temptype;
          subtreeExp(p->child);
          subtreeExp(p->sibling);
        } else {
          if ( !cmpType(lefttype, temptype) ) {
            valid = false;
            if (freturn) printf("Error type 8 at line %d: The return type mismatched\n" , p->lineno);
            if (assignop || fdecexp) ;//printf("Error type 5 at line %d: Type mismatched\n" , p->lineno);
            else printf("Error type 7 at line %d: Operands type mismatched\n", p->lineno);	
          }
          if (valid) {
            subtreeExp(p->child);
            subtreeExp(p->sibling);
          }
        }
      }
    }
    //identify INTorFLOAT
    if (p->child != NULL && (p->child->label == NODE_INT || p->child->label == NODE_FLOAT) ){
      exptype = &exptypecontent;
      exptype->kind = basic;
      exptype->u.basic = (p->child->label == NODE_INT) ? eINTTYPE : eFLOATTYPE;
      if (leftmost){
        leftmost = false;
        vartype.kind = basic;
        if (p->child->label == NODE_INT) vartype.u.basic = 0;
        else vartype.u.basic = 1;
        lefttype = &vartype;
        subtreeExp(p->child);
        subtreeExp(p->sibling);
        //valid = false;			
        //printf("Error type 6 at line %d: The left-hand side of an assignment must be a variable\n" , p->child->lineno);	
      }
      else {
        int checkbasic = 0;
        if (p->child->label == NODE_INT) checkbasic = 0;
        else checkbasic = 1;
        if (lefttype->kind != basic || lefttype->u.basic != checkbasic){
          valid = false;
          if (freturn) printf("Error type 8 at line %d: The return type mismatched\n" , p->child->lineno);
          if (assignop || fdecexp) ;//printf("Error type 5 at line %d: Type mismatched\n" , p->child->lineno);
          else printf("Error type 7 at line %d: Operands type mismatched\n" , p->child->lineno);
        }
        else {
          subtreeExp(p->child);
          subtreeExp(p->sibling);
        }
      }
    }
    if (!branch) {
      subtreeExp(p->child);
      subtreeExp(p->sibling);
    }
    return exptype;
  } else {
    return NULL;
  }
}

void semantic(node * p, Type * upperlevel , Func * currentfunc) {
  if (p != NULL) {
    if (p->label == NODE_NONTERMINATE ) {
      //printf("%s (%d)\n", stringNonTerminate[p->ntype.type_nonterm], p->lineno);
      switch (p->ntype.type_nonterm) {
        case Def:
        case ParamDec:
          subtreeDef(p, upperlevel, currentfunc);
          semantic(p->sibling, upperlevel , currentfunc);
          break;
        case ExtDef:
          subtreeExtDef(p, upperlevel, currentfunc);
          semantic(p->sibling, upperlevel , currentfunc);
          break;
        case Stmt:
          subtreeStmt(p , upperlevel , currentfunc);
          if(fstmt) {
            semantic(p->child->sibling->sibling->sibling->sibling, upperlevel ,currentfunc);
            semantic(p->sibling , upperlevel , currentfunc);			
          }
          else {
            if (fexp) semantic(p->sibling , upperlevel , currentfunc);
            else { semantic(p->child , upperlevel , currentfunc);
              semantic(p->sibling , upperlevel ,currentfunc);			
            }
          }

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

      if (funclist[i]->param != NULL) param =funclist[i]->param;
      while (param != NULL){
        switch(param->type->kind){	
          case basic :
            printf("\tparam: %s\n", param->name);
            printf("\tkind: basic\n");
            printf("\tu: %s\n", (param->type->u.basic == eINTTYPE) ? "INT" : "FLOAT");
            break;
          case array :
            printf("\tparam: %s\n", param->name);
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
            printf("\tparam: %s\n", param->name);
            printf("\tkind: structure\n");
            printf("\tu: struct %s\n", param->type->u.structure.name);
            break;
          default :
            printf("\tvariable invalid\n");
            break;
        }
        param = param->tail;
      }
    }
  }
}	

void getfuncdeclist() {
  int i = 0;
  FieldList * param = NULL;
  Type * type = NULL;
  for (i = 0; i < MAX_VARIABLE; i ++) {
    if (funcdeclist[i] != NULL ) {
      printf("funcdeclist[%d]: %s\n", i, funcdeclist[i]->name);
      switch (funcdeclist[i]->returntype->kind) {
        case basic :
          printf("\treturnkind: basic\n");
          printf("\tu: %s\n", (funcdeclist[i]->returntype->u.basic == eINTTYPE) ? "INT" : "FLOAT");
          break;
        case structure :
          printf("\treturnkind: structure\n");
          printf("\tu: struct %s\n", funcdeclist[i]->returntype->u.structure.name);
          break;
        default :
          printf("\tfuntion invalid\n");
          break;
      }

      if (funcdeclist[i]->param != NULL) param =funcdeclist[i]->param;
      while (param != NULL){
        switch(param->type->kind){	
          case basic :
            printf("\tparam: %s\n", param->name);
            printf("\tkind: basic\n");
            printf("\tu: %s\n", (param->type->u.basic == eINTTYPE) ? "INT" : "FLOAT");
            break;
          case array :
            printf("\tparam: %s\n", param->name);
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
            printf("\tparam: %s\n", param->name);
            printf("\tkind: structure\n");
            printf("\tu: struct %s\n", param->type->u.structure.name);
            break;
          default :
            printf("\tvariable invalid\n");
            break;
        }
        param = param->tail;
      }
    }
  }
}


void checkfunc(){
  int i = 0 , probe = 0;
  char* name;
  FieldList * paramdec = NULL;
  FieldList * paramdef = NULL;
  Type * type = NULL;
  bool F = true;
  for (i = 0 ; i<MAX_VARIABLE ; i++){
    if ( funcdeclist[i] != NULL){
      name = funcdeclist[i]->name;
      if ( findFunc(name) < 0 ) {
        F = false;
        printf("Error type 18 at line %d: Undefined function \"%s\"\n" ,
            funcdeclist[i]->funclineno , name);
      }
      else {
        probe = findFunc(name);
        if (! cmpFunc(funcdeclist[i] , funclist[probe]) )
          printf("Error type 19 at line %d: Inconsistent declaration of function \"%s\"\n", funclist[i]->funclineno , name);
      }
    }

  }
}

