#ifndef __INTERCODE_H__
#define __INTERCODE_H__

typedef struct Operand_ {
  enum { opVARIABLE, opCONSTANT, opADDRESS, opTEMP, opLABEL } kind;
  union {
    int var_no;
    int value;
    
  } u;
} Operand;

typedef struct InterCode {
  enum { icASSIGN, icADD, icSUB, icMUL,
    icLABEL, icFUNCDEF, icGETADDR, icMEMREAD, icMEMWRITE, 
    icGOTOBRANCH, icIFBRANCH, 
    icFUNCRETURN, iciMEMDEC, icARG, icCALL, icPARAM, 
    icREAD, icWRITE
  } kind;
  union {
    struct { Operand * right, * left; } assign;
    struct { Operand * result, * op1, * op2; } binop;
    struct { Operand * label; } label;
    struct { char * funcname; } funcdef;
    struct { Operand * result, * op; } getaddr;
    struct { Operand * result, * op; } memread;
    struct { Operand * result, * op; } memwrite;
    struct { Operand * label; } gotobranch;
    struct { Operand * op1, * op2, * label; } ifbranch;
    struct { Operand * op; } funcreturn;
    struct { Operand * size; } memdec;
    struct { Operand * arg; } arg;
    struct { Operand * returnvalue; char * funcname; } call;
    struct { Operand * param; } param;
    struct { Operand * op; } read;
    struct { Operand * op; } write;
  } u;
} InterCode;

typedef struct InterCodes {
  InterCode code;
  struct InterCodes * prev, * next;
} InterCodes;

#endif
