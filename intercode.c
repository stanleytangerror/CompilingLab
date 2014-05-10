#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "tree.h"
#include "semantic.h"
#include "intercode.h"

InterCodes * ichead = NULL;
int var_count = 0;
int temp_count = 0;
int addr_count = 0;
int label_count = 0;

Operand * get_value(int value) {
  Operand * op = (Operand *) malloc(sizeof(Operand));
  op->kind = opCONSTANT;
  op->u.value = value;
  return op;
}

Operand * get_relop(enum RelopValue e) {
  Operand * op = (Operand *) malloc(sizeof(Operand));
  op->kind = opRELOP;
  op->u.relop = e;
  return op;

Operand * new_temp() {
  Operand * op = (Operand *) malloc(sizeof(Operand));
  op->kind = opTEMP;
  op->u.temp_no = temp_count;
  temp_count ++;
  return op;
}

Operand * new_label() {
  Operand * op = (Operand *) malloc(sizeof(Operand));
  op->kind = opLABEL;
  op->u.label_no = label_count;
  label_count ++;
  return op;
}

Operand * lookup(char * id) {
  Operand * op = (Operand *) malloc(sizeof(Operand));
  op->kind = opVARIABLE;
  op->u.var_no = findVar(id);
  assert(op->u.var_no >= 0);
  return op;
}

InterCodes * linkcode(InterCodes * code1, InterCodes * code2) {
  if (code1 != NULL && code2 != NULL) {
    InterCodes * p = code1;
    while (p->next != NULL) {
      p = p->next;
    }
    p->next = code2;
    code2->prev = p;
    return code1;
  }
  return NULL;
}

InterCodes * gen_assign(Operand * left, Operand * right) {
  InterCodes * codeline = (InterCodes *) malloc(sizeof(InterCodes));
  codeline->prev = NULL;
  codeline->next = NULL;
  codeline->code.kind = icASSIGN;
  codeline->code.u.assign.right = right;
  codeline->code.u.assign.left = left;
  return codeline;
}

InterCodes * gen_binop(enum Terminate e, Operand * result, Operand * op1, Operand * op2) {
  InterCodes * codeline = (InterCodes *) malloc(sizeof(InterCodes));
  codeline->prev = NULL;
  codeline->next = NULL;
  switch (e) {
    case ePLUS:
      codeline->code.kind = icADD;
      break;
    case eMINUS:
      codeline->code.kind = icSUB;
      break;
    case eSTAR:
      codeline->code.kind = icMUL;
      break;
    case eDIV:
      codeline->code.kind = icDIV;
      break;
    default :
      break;
  }
  codeline->code.u.binop.result = result;
  codeline->code.u.binop.op1 = op1;
  codeline->code.u.binop.op2 = op2;
  return codeline;
}

InterCodes * gen_label(Operand * label) {
  InterCodes * codeline = (InterCodes * ) malloc(sizeof(InterCodes));
  codeline->prev = NULL;
  codeline->next = NULL;
  codeline->code.kind = icLABEL;
  codeline->code.u.label.label = label;
  return codeline;
}

InterCodes * gen_gotobranch(Operand * label) {
  InterCodes * codeline = (InterCodes * ) malloc(sizeof(InterCodes));
  codeline->prev = NULL;
  codeline->next = NULL;
  codeline->code.kind = icGOTOBRANCH;
  codeline->code.u.gotobranch.label = label;
  return codeline;
}

InterCodes * gen_ifbranch(Operand * op1, Operand * relop, Operand * op2, Operand * label) {
  InterCodes * codeline = (InterCodes * ) malloc(sizeof(InterCodes));
  codeline->prev = NULL;
  codeline->next = NULL;
  codeline->code.kind = icIFBRANCH;
  codeline->code.u.ifbranch.op1 = op1;
  codeline->code.u.ifbranch.relop = relop;
  codeline->code.u.ifbranch.op2 = op2;
  codeline->code.u.ifbranch.label = label;
  return codeline;
}

InterCodes * gen_memdec(Operand * op, int size) {
  InterCodes * codeline = (InterCodes *) malloc(sizeof(InterCodes));
  codeline->prev = NULL;
  codeline->next = NULL;
  codeline->code.kind = icMEMDEC;
  codeline->code.u.memdec.op = op;
  codeline->code.u.memdec.size = size;
  return codeline;
}

InterCodes * translate_Exp(node * exp, FieldList ** sym_table, Operand * place) {
  if (exp->child->label == NODE_NONTERMINATE && exp->child->ntype.type_nonterm == Exp
      && exp->child->sibling == NULL) {
    // Exp
  }
  if (exp->child->label == NODE_INT && exp->child->sibling == NULL) {
    // INT
    Operand * value = get_value(exp->child->nvalue.value_int);
    return gen_assign(place, value);
  }
  if (exp->child->label == NODE_ID && exp->child->sibling == NULL) {
    // ID
    Operand * variable = lookup(exp->child->nvalue.value_id);
    return gen_assign(place, variable);
  }
  if (exp->child->label == NODE_TERMINATE && exp->child->ntype.type_term == eMINUS
      && exp->child->sibling->sibling == NULL) {
    // MINUS Exp
    Operand * t1 = new_temp();
    InterCodes * code1 = translate_Exp(exp->child, varlist, t1);
    InterCodes * code2 = gen_binop(eMINUS, place, get_value(0), t1);
    linkcode(code1, code2);
    return code1;
  }
  if (exp->child->label == NODE_TERMINATE && exp->child->ntype.type_term == eNOT
      && exp->child->sibling->sibling == NULL) {
    // NOT Exp
    return NULL;
  }
  if (exp->child->sibling->label == NODE_TERMINATE && exp->child->sibling->ntype.type_term == eASSIGNOP) {
    // Exp ASSIGNOP Exp
    node * p = exp->child;
    Operand * t1 = new_temp();
    assert(p->child->label == NODE_ID);
    Operand * variable = lookup(p->child->nvalue.value_id);
    InterCodes * code1 = translate_Exp(p->sibling->sibling, varlist, t1);
    InterCodes * code2 = gen_assign(variable, t1);
    linkcode(code2, gen_assign(place, variable));
    linkcode(code1, code2);
    return code1;
  }
  if (exp->child->sibling->label == NODE_TERMINATE 
      && (exp->child->sibling->ntype.type_term == ePLUS ||
      exp->child->sibling->ntype.type_term == eMINUS ||
      exp->child->sibling->ntype.type_term == eSTAR ||
      exp->child->sibling->ntype.type_term == eDIV) ) {
    // Exp PLUS/MINUS/STAR/DIV Exp
    Operand * t1 = new_temp();
    Operand * t2 = new_temp();
    InterCodes * code1 = translate_Exp(exp->child, varlist, t1);
    InterCodes * code2 = translate_Exp(exp->child, varlist, t2);
    InterCodes * code3 = gen_binop(exp->child->sibling->ntype.type_term, place, t1, t2);
    linkcode(code1, code2);
    linkcode(code2, code3);
    return code1;
  }
  if ( exp->child->sibling->label == NODE_RELOP
      || (exp->child->sibling->label == NODE_TERMINATE && exp->child->sibling->ntype.type_term == eAND)
      || (exp->child->sibling->label == NODE_TERMINATE && exp->child->sibling->ntype.type_term == eOR)
      || (exp->child->label == NODE_TERMINATE && exp->child->ntype.type_term == eNOT)  ) {
    // Exp RELOP/AND/OR Exp / NOT Exp
    Operand * label1 = new_label();
    Operand * label2 = new_label();
    InterCodes * code0 = gen_assign(place, get_value(0));
    InterCodes * code1 = translate_Cond(exp, label1, label2, varlist);
    InterCodes * code2 = gen_label(label1);
    linkcode(code2, gen_assign(place, get_value(0)));
    linkcode(code0, code1);
    linkcode(code1, code2);
    linkcode(code2, gen_label(label2));
    return code0;
  }
}

InterCodes * translate_Cond(node * exp, Operand * label_true, Operand * label_false, FieldList ** sym_table) {
  if (exp->child->label == NODE_TERMINATE && exp->child->ntype.type_term == eNOT) {
    // NOT Exp
    return translate_Cond(exp->child->sibling, label_false, label_true, sym_table);
  }
  if ( exp->child->sibling->label == NODE_RELOP) {
    // Exp RELOP Exp
    node * p = exp->child;
    Operand * t1 = new_temp();
    Operand * t2 = new_temp();
    InterCodes * code1 = translate_Exp(p, sym_table, t1);
    InterCodes * code2 = translate_Exp(p->sibling->sibling, sym_table, t2);
    Operand * op = get_relop();
    InterCodes * code3 = gen_ifbranch(t1, op, t2, label_true);
    linkcode(code1, code2);
    linkcode(code2, code3);
    linkcode(code3, gen_gotobranch(label_false));
    return code1;
  }
  if (exp->child->sibling->label == NODE_TERMINATE && exp->child->sibling->ntype.type_term == eAND) {
    // Exp AND Exp
    node * p = exp->child;
    Operand * label1 = new_label();
    InterCodes * code1 = translate_Cond(p, label1, label_false, sym_table);
    InterCodes * code2 = translate_Cond(p->sibling->sibling, label_true, label_false, sym_table);
    linkcode(code1, gen_gotobranch(label1));
    linkcode(code1, code2);
    return code1;
  }
  if (exp->child->sibling->label == NODE_TERMINATE && exp->child->sibling->ntype.type_term == eOR) {
    // Exp OR Exp
    node * p = exp->child;
    Operand * label1 = new_label();
    InterCodes * code1 = translate_Cond(p, label_true, label1, sym_table);
    InterCodes * code2 = translate_Cond(p->sibling->sibling, label_true, label_false, sym_table);
    linkcode(code1, gen_gotobranch(label1));
    linkcode(code1, code2);
    return code1;
  }
  if (true) {
    // [other cases]
    Operand * t1 = new_temp();
    InterCodes * code1 = translate_Exp(exp, sym_table, t1);
    InterCodes * t2 = gen_ifbranch(t1, get_relop(eNE), get_value(0), label_true);
    linkcode(code1, code2);
    linkcode(code2, gen_gotobranch(label_false));
    return code1;
  }
}

void translate(node * p) {
  if (p != NULL) {
    if (p->label == NODE_NONTERMINATE ) {
      printf("%s (%d)\n", stringNonTerminate[p->ntype.type_nonterm], p->lineno);
      switch (p->ntype.type_nonterm) {
        case Def:
        case ParamDec:
        case ExtDef:
        case Stmt:
          break;
        case Exp:
          translate_Exp(p, varlist, NULL); 
        default:
          break;
      }
    } else {
      translate(p->child);
      translate(p->sibling);
    }
  }
}

void printcode(InterCodes * code) {
  if (code == NULL) {
    printf("no intermediate code\n");
    return ;
  }
  InterCodes * p = code;
  while (p != NULL) {
    switch (p->code.kind) {
      case icASSIGN:
        printoperand(p->code.u.assign.left);
        printf(" := ");
        printoperand(p->code.u.assign.right);
        break;
      case icADD:
        break;
      case icSUB:
        break;
      case icMUL:
        break;
      case icDIV:
        break;
      case icLABEL:
        break;
      case icFUNCDEF:
        break;
      case icGETADDR:
        break;
      case icMEMREAD:
        break;
      case icMEMWRITE:
        break;
      case icGOTOBRANCH:
        break;
      case icIFBRANCH:
        break;
      case icFUNCRETURN:
        break;
      case icMEMDEC:
        break;
      case icARG:
        break;
      case icCALL:
        break;
      case icPARAM:
        break;
      case icREAD:
        break;
      case icWRITE:
        break;
      default :
        break;
    }
    printf("\n");
    p = p->next;
  }
  return ;
}

void printoperand(Operand * op) {
  switch (op->kind) {
    case opVARIABLE:
      printf(" var%d ", op->u.var_no );
      break;
    case opCONSTANT:
      printf(" #%d ", op->u.value );
      break;
    case opADDRESS:
      printf(" addr%d ", op->u.addr_no );
      break;
    case opTEMP:
      printf(" temp%d ", op->u.temp_no );
      break;
    case opLABEL:
      printf(" label%d ", op->u.var_no );
      break;
    default :
      break;
  }
}
