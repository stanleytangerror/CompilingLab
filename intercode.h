#ifndef __INTERCODE_H__
#define __INTERCODE_H__

#include "tree.h"
#include "semantic.h"

typedef struct Operand_ {
  enum { opVARIABLE, opCONSTANT, opADDRESS, opVARADDRESS, opTEMP, opLABEL, opRELOP } kind;
  union {
    int var_no;
    int value;
    int temp_no;
    int label_no;
    enum RelopValue relop;
  } u;
} Operand;

typedef struct Operands {
  Operand * op;
  struct Operands * prev, * next;
} Operands;

typedef struct InterCode {
  enum { icASSIGN, icBINOP,
    icLABEL, icFUNCDEF, icGETADDR, icMEMREAD, icMEMWRITE, 
    icGOTOBRANCH, icIFBRANCH, 
    icFUNCRETURN, icMEMDEC, icARG, icCALL, icPARAM, 
    icREAD, icWRITE
  } kind;
  union {
    struct { Operand * right, * left; } assign;
    struct { Operand * result, * op1, * op2; enum Terminate sign; } binop;
    struct { Operand * label; } label;
    struct { char funcname[MAXID]; } funcdef;
    struct { Operand * result, * op; int size; } getaddr;
    struct { Operand * result, * op; } memread;
    struct { Operand * result, * op; } memwrite;
    struct { Operand * label; } gotobranch;
    struct { Operand * op1, * relop, * op2, * label; } ifbranch;
    struct { Operand * op; } funcreturn;
    struct { Operand * op; int size; } memdec;
    struct { Operand * arg; } arg;
    struct { Operand * returnop; char funcname[MAXID]; } call;
    struct { Operand * param; } param;
    struct { Operand * op; } read;
    struct { Operand * op; } write;
  } u;
} InterCode;

typedef struct InterCodes {
  InterCode code;
  struct InterCodes * prev, * next;
} InterCodes;

extern InterCodes * ichead;
extern int temp_count;
extern int label_count;

void initvarlist();

Operand * get_value(int value);

Operand * get_relop(enum RelopValue e);

Operand * get_arglist(Operands * arg_list, int index);

int get_typesize (Type * type);

Operands * insert_arglist_head(Operands * arglist, Operand * arg);

Operand * new_addr();

Operand * new_temp();

Operand * new_label();

Operand * lookup_varlist(char * id);

InterCodes * gen_binop(enum Terminate e, Operand * result, Operand * op1, Operand * op2);

InterCodes * gen_label(Operand * label);

InterCodes * gen_funcdef(char * funcname);

InterCodes * gen_getaddr(Operand * result, Operand * op, int size);

InterCodes * gen_memread(Operand * result, Operand * op);

InterCodes * gen_memwrite(Operand * result, Operand * op);

InterCodes * gen_gotobranch(Operand * label);

InterCodes * gen_ifbranch(Operand * op1, Operand * relop, Operand * op2, Operand * label);

InterCodes * gen_funcreturn(Operand * op);

InterCodes * gen_memdec(Operand * op, int size);

InterCodes * gen_arg(Operand * arg);

InterCodes * gen_call(Operand * returnop, char *  funcname);

InterCodes * gen_param(Operand * param);

InterCodes * gen_read(Operand * op);

InterCodes * gen_write(Operand * op);

InterCodes * translate_Dec(node * dec, FieldList ** sym_table, Type * type);

InterCodes * translate_DecList(node * declist, FieldList ** sym_table, Type * type);

InterCodes * translate_Def(node * def, FieldList ** sym_table);

InterCodes * translate_DefList(node * deflist, FieldList ** sym_table);

InterCodes * translate_Args(node * args, FieldList ** sym_table, Operands ** arg_list);

InterCodes * translate_Exp(node * exp, FieldList ** sym_table, Operand * place);

InterCodes * translate_Unit(node * exp, FieldList ** sym_table, Operand * addr, Type ** type);

InterCodes * translate_Cond(node * exp, Operand * label_true, Operand * label_false, FieldList ** sym_table);

InterCodes * translate_Stmt(node * stmt, FieldList ** sym_table);

InterCodes * translate_StmtList(node * stmtlist, FieldList ** sym_table);

InterCodes * translate_ParamDec(node * paramdec, FieldList ** sym_table);

InterCodes * translate_VarList(node * varlist , FieldList ** sym_table);

InterCodes * translate_FunDec(node * fundec, FieldList ** sym_table);

InterCodes * translate_CompSt(node * compst, FieldList ** sym_table);

InterCodes * translate_ExtDef(node * extdef, FieldList ** sym_table);

InterCodes * translate_ExtDefList(node * extdeflist, FieldList ** sym_table);

void translate(node * p);

void printoperand(Operand * op); 

void printcode(InterCodes * code);

void fprintoperand(FILE * file, Operand * op); 

void fprintcode(FILE * file, InterCodes * code);

void writecode();

#endif
