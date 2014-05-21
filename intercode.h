#ifndef __INTERCODE_H__
#define __INTERCODE_H__

#include "tree.h"
#include "semantic.h"

typedef struct Operand_ {
  enum { opVARIABLE, opCONSTANT, opADDRESS, opVARADDRESS, opTEMP, opLABEL, opRELOP } kind;
  union {
    int var_no;
    int value;
    int addr_no;
    int temp_no;
    int label_no;
    enum RelopValue relop;
  } u;
} Operand;

typedef struct Operands {
  Operand * op;
  enum ArgType { atVALUE, atADDR } type;
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
extern int addr_count;
extern int label_count;

InterCodes * translate_Exp(node * exp, FieldList ** sym_table, Operand * place);

InterCodes * translate_Unit(node * exp, FieldList ** sym_table, Operand * addr, Type ** type);

InterCodes * translate_Cond(node * exp, Operand * label_true, Operand * label_false, FieldList ** sym_table);

InterCodes * translate_StmtList(node * stmtlist, FieldList ** sym_table);

InterCodes * translate_CompSt(node * compst, FieldList ** sym_table);
Operand * new_addr() ;
InterCodes * gen_binop(enum Terminate e, Operand * result, Operand * op1, Operand * op2);
InterCodes * gen_getaddr(Operand * result, Operand * op, int size);
void printoperand(Operand * op); 

void printcode(InterCodes * code);

void fprintoperand(FILE * file, Operand * op); 

void fprintcode(FILE * file, InterCodes * code);

void writecode();

#endif
