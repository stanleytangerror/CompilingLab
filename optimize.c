#include <stdlib.h>
#include <stdbool.h>
#include "tree.h"
#include "semantic.h"
#include "intercode.h"
#include "optimize.h"

Operand * reg[1000];//record the register
Operand* addr[1000];//record the array address
InterCodes * address_val[1000];

void getoperand(Operand * op) {
  if (op != NULL)
  switch (op->kind) {
    case opVARADDRESS:
    case opVARIABLE:
      printf(" %s ", varlist[op->u.var_no]->name );
      break;
    case opCONSTANT:
      printf(" #%d ", op->u.value );
      break;
    case opADDRESS:
      printf(" t%d(addr) ", op->u.temp_no );
      break;
    case opTEMP:
      printf(" t%d ", op->u.temp_no );
      break;
    case opLABEL:
      printf(" label%d ", op->u.var_no );
      break;
    case opRELOP:
      printf(" %s ", stringRelopValue[op->u.relop]);
      break;
    default :
      break;
  }
}

bool checktemp(InterCodes * code , int k){
	InterCodes * p = code;
	while (p != NULL) {
		switch (p->code.kind){
		case icASSIGN:
			if (p->code.u.assign.left->kind == opTEMP && p->code.u.assign.left->u.temp_no == k)
			 	return false;
		break;
		case icBINOP:
        		if (p->code.u.binop.result->kind == opTEMP && p->code.u.binop.result->u.temp_no == k)
				return false;
			if (p->code.u.binop.result->kind == opADDRESS && p->code.u.binop.result->u.addr_no == k)
				return false;
		break;
		case icGETADDR:
        		if (p->code.u.getaddr.result->kind == opTEMP && p->code.u.getaddr.result->u.temp_no == k)
			return false;
			if (p->code.u.getaddr.result->kind == opADDRESS && p->code.u.getaddr.result->u.addr_no == k)
			return false;
       		break;
		case icMEMREAD:
        		if (p->code.u.memread.result->kind == opTEMP && p->code.u.memread.result->u.temp_no == k)
			return false;
			if (p->code.u.memread.result->kind == opADDRESS && p->code.u.memread.result->u.addr_no == k)
			return false;
        	break;
		case icMEMWRITE:
			if (p->code.u.memwrite.result->kind == opTEMP && p->code.u.memwrite.result->u.temp_no == k)
			return false;
			if (p->code.u.memwrite.result->kind == opADDRESS && p->code.u.memwrite.result->u.addr_no == k)
			return false;
        	break;
		default :
        	break;
		}
		p = p->next;
	}
	return true;
}

void exchange(InterCodes * code){
	InterCodes *p = code;
	while (p!=NULL){
		if (p->code.kind == icMEMDEC){
			int var = p->code.u.memdec.op->u.var_no;
			InterCodes * t = p->next;
			while (t!=NULL){
				InterCodes * tsl = t->next;
				if (t->code.kind == icGETADDR){	
					if (t->code.u.getaddr.op->u.var_no == var){
						add(p,t);
					}
				}
				t = tsl;			
			}
		}
		p = p->next;
	}
}

void getcode(InterCodes * code) {
  if (code == NULL) {
    printf("no intermediate code\n");
    return ;
  }
  int k = 0;
  for (k=0 ; k<1000; k++){
	reg[k] = NULL;
	addr[k] = NULL;
	address_val[k] = NULL;
	}
  InterCodes * p = code;
  while (p != NULL) {
    switch (p->code.kind) {
      case icASSIGN:
    if (p->code.u.assign.left->kind == opVARIABLE && p->code.u.assign.right->kind == opTEMP){
		InterCodes* q = p->prev;
		if (q->code.kind == icBINOP && q->code.u.binop.result->kind == opTEMP 
			&& (q->code.u.binop.result->u.temp_no == p->code.u.assign.right->u.temp_no)){
			q->code.u.binop.result = p->code.u.assign.left;
			q->next = p->next;
			p->next->prev = q;
		} 
	}
    if ((p->code.u.assign.left->kind == opTEMP) && (p->code.u.assign.right->kind == opCONSTANT || p->code.u.assign.right->kind == opVARIABLE) ){
			int i = p->code.u.assign.left->u.temp_no ;
			if (checktemp(p->next , i)) { 
			reg[i] = p->code.u.assign.right;
			p->next->prev = p->prev;
			p->prev->next = p->next;
			}
			InterCodes * q = p->next;
			if (q->code.kind == icBINOP && q->code.u.binop.result->kind == opTEMP 
				&& (q->code.u.binop.result->u.temp_no == p->code.u.assign.left->u.temp_no)){
				if (q->code.u.binop.op1->kind == opTEMP && (p->code.u.assign.left->u.temp_no == q->code.u.binop.op1->u.temp_no) ){
					q->code.u.binop.op1 = p->code.u.assign.right;				
				}
				p->prev->next = q;
				q->prev = p->prev;			
			}		
		}
	if ( p->code.u.assign.right->kind == opTEMP && reg[p->code.u.assign.right->u.temp_no]!= NULL)
		p->code.u.assign.right = reg[p->code.u.assign.right->u.temp_no];
        break;
      case icBINOP:
	if (p->code.u.binop.result->kind == opADDRESS && p->code.u.binop.op1->kind == opADDRESS 
		&& addr[p->code.u.binop.op1->u.addr_no] !=NULL)	{
			p->code.u.binop.op1 = addr[p->code.u.binop.op1->u.addr_no];
		}
	if (p->code.u.binop.result->kind == opADDRESS && addr[p->code.u.binop.result->u.addr_no] !=NULL){
			addr[p->code.u.binop.result->u.addr_no] = NULL;
		}
	if (p->code.u.binop.op1->kind == opTEMP && reg[p->code.u.binop.op1->u.temp_no] != NULL){
			p->code.u.binop.op1 = reg[p->code.u.binop.op1->u.temp_no];	
		}
	if (p->code.u.binop.op2->kind == opTEMP && reg[p->code.u.binop.op2->u.temp_no] != NULL){
			p->code.u.binop.op2 = reg[p->code.u.binop.op2->u.temp_no];	
		}
	if (p->code.u.binop.op1->kind == opADDRESS && reg[p->code.u.binop.op1->u.addr_no] != NULL){
			p->code.u.binop.op1 = reg[p->code.u.binop.op1->u.temp_no];	
		}
	if (p->code.u.binop.op2->kind == opADDRESS && reg[p->code.u.binop.op2->u.addr_no] != NULL){
			p->code.u.binop.op2 = reg[p->code.u.binop.op2->u.temp_no];	
		}
	if (p->code.u.binop.result->kind == opTEMP && p->code.u.binop.op1->kind == opCONSTANT && p->code.u.binop.op2->kind == opCONSTANT){
			int temp = 0;
			switch (p->code.u.binop.sign){
				case 3:
					temp = p->code.u.binop.op1->u.value + p->code.u.binop.op2->u.value;
				break;
				case 4:
					temp = p->code.u.binop.op1->u.value - p->code.u.binop.op2->u.value;
				break;
				case 5:
					temp = p->code.u.binop.op1->u.value * p->code.u.binop.op2->u.value;
				break;
				case 6:
					temp = p->code.u.binop.op1->u.value / p->code.u.binop.op2->u.value;
				break;
				default:
				break;			
			}	
			p->code.kind = icASSIGN;
			p->code.u.binop.op1->u.value = temp;
			p->code.u.binop.op2 = p->code.u.binop.result;
			p->code.u.binop.result = p->code.u.binop.op1;
			p->code.u.binop.op1 = p->code.u.binop.op2;  	
		}
        break;
      case icGETADDR:
	 {
	 int slot = p->code.u.getaddr.result->u.addr_no;
	 int i = 0 ;
	 bool check = false;
	 for (i = 0 ;i<1000 ;i++){
		if (address_val[i] !=NULL){
			if ( address_val[i]->code.u.getaddr.op->u.var_no == p->code.u.getaddr.op->u.var_no &&
				address_val[i]->code.u.getaddr.size == p->code.u.getaddr.size)
			{
				check =true;
				break;
			}
		} 
	 }
	 if (check) {
		addr[slot] = address_val[i]->code.u.getaddr.result;
		p->next->prev = p->prev;
		p->prev->next = p->next;
	 }else{
		Operand * newaddr = new_addr();
		//InterCodes *q = gen_binop( p->code.u.binop.sign, newaddr, p->code.u.binop.op1 , p->code.u.binop.op2);
		//InterCodes * gen_getaddr(Operand * result, Operand * op, int size);
		InterCodes *q = gen_getaddr(newaddr ,p->code.u.getaddr.op, p->code.u.getaddr.size);
	 	address_val[slot] = q;
		q->next = p;
		q->prev = p->prev;
		p->prev->next = q;
		p->prev = q;
 	 }}
        break;
      case icMEMREAD:
	if (p->code.u.memread.op->kind == opTEMP && reg[p->code.u.memread.op->u.temp_no] !=NULL)
		p->code.u.memread.op =  reg[p->code.u.memread.op->u.temp_no];
        break;
      case icMEMWRITE:
        if (p->code.u.memwrite.op->kind == opADDRESS) printf(" *");
	if (p->code.u.memwrite.op->kind == opTEMP && reg[p->code.u.memwrite.op->u.temp_no] !=NULL)
		p->code.u.memwrite.op =  reg[p->code.u.memwrite.op->u.temp_no];
        break;
      case icIFBRANCH:
	if (p->code.u.ifbranch.op1->kind == opTEMP && reg[p->code.u.ifbranch.op1->u.temp_no] != NULL)
		p->code.u.ifbranch.op1 = reg[p->code.u.ifbranch.op1->u.temp_no];
	if (p->code.u.ifbranch.op2->kind == opTEMP && reg[p->code.u.ifbranch.op2->u.temp_no] != NULL)
		p->code.u.ifbranch.op2 = reg[p->code.u.ifbranch.op2->u.temp_no];
        break;
      case icFUNCRETURN:
	if (p->code.u.funcreturn.op->kind == opTEMP && reg[p->code.u.funcreturn.op->u.temp_no] !=NULL)
		p->code.u.funcreturn.op =  reg[p->code.u.funcreturn.op->u.temp_no];
        break;
      case icARG:
	if (p->code.u.arg.arg->kind == opTEMP && reg[p->code.u.arg.arg->u.temp_no] !=NULL)
		p->code.u.arg.arg =  reg[p->code.u.arg.arg->u.temp_no];
	if (p->code.u.arg.arg->kind == opADDRESS && addr[p->code.u.arg.arg->u.addr_no] !=NULL)	{
			p->code.u.arg.arg = addr[p->code.u.arg.arg->u.addr_no];
		}
        break;
      case icWRITE:
	if (p->code.u.write.op->kind == opTEMP && reg[p->code.u.write.op->u.temp_no] !=NULL)
		p->code.u.write.op =  reg[p->code.u.write.op->u.temp_no];
        break;
      default :
        break;
    }
    p = p->next;
  }
  return ;
}

void optimizecode() {
  FILE * file = fopen("test02.ir", "w");
  if (file == NULL) {
    perror("Open file failed\n");
    exit(0);
  }
  fprintcode(file, ichead);
  fclose(file);
}

void delete(InterCodes *p){
	p->prev->next = p->next;
	p->next->prev = p->prev;
}
//q after p
void add(InterCodes *p , InterCodes * q){
	delete(q);	
	q->next = p->next;
	q->prev = p;
	p->next->prev = q;
	p->next = q;	
}
