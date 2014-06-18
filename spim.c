#include <stdlib.h>
#include <stdbool.h>
#include "tree.h"
#include "semantic.h"
#include "intercode.h"
#include "optimize.h"
#include "spim.h"

int currentParam = 0;
int currentArg = 0;
int currentfp = 0;
int recordfp;
int count;
bool fparam = true;
bool farg = true;
int offset = 1;

int stack_sp[MAX_VARIABLE];
int stack_temp[MAX_VARIABLE];
bool arrayoffset[MAX_VARIABLE];

bool arg[4];
int param[MAX_VARIABLE];
/*
int varReg(int no){
	if (spimReg[no] != -1){
		return spimReg[no];
	}
	else {
	   	spimReg[no] = currentReg;
		currentReg++;
		return spimReg[no];
	}
}
int tReg(int no){
	if (tempReg[no] != -1){
		return tempReg[no];
	}
	else {
	   	tempReg[no] = currentReg;
		currentReg++;
		return tempReg[no];
	}
}
*/
bool checkmain(char * ch){
	bool f = false;
	if (strlen(ch) == 4 && ch[0] =='m' && ch[1] =='a' &&ch[2]=='i' && ch[3] == 'n') f = true;
	return f;
}

void initialvar(){
  int i;
  for (i = 0 ; i<MAX_VARIABLE ; i++){
  	stack_sp[i] = -1;
  	stack_temp[i] = -1;
	arrayoffset[i] = false;
  }
}

void spimcode(char * spimcode){
  FILE * file = fopen(spimcode , "w");
  if (file == NULL) {
    perror("Open file failed\n");
    exit(0);
  }
  initialvar();
  //spimoffset(ichead);
  fprintf(file , ".data\n");
  fprintf(file , "_ret: .asciiz \"\\n\"\n");
  fprintf(file , ".globl main\n");
  fprintf(file ,".text\n");
  //fprintread(file);
  //fprintwrite(file);
  fspimcode(file, ichead);
  fclose(file);
}

int fpoffset(Operand * op) {
  if (op != NULL)
  if (op->kind == opTEMP) {
      if (stack_temp[op->u.temp_no] != -1) return stack_temp[op->u.temp_no];
	else {
		currentfp = currentfp - 4;
		stack_temp[op->u.temp_no] = currentfp;
		return currentfp;
	}
   }else{
      if (stack_sp[op->u.var_no] != -1) return stack_sp[op->u.var_no];
	else {
		currentfp = currentfp - 4;
		stack_sp[op->u.var_no] = currentfp;
		return currentfp;
	}
	}
}

void offset_var_method(int no){
	if (stack_sp[no] < 0){
		stack_sp[no] = offset;
		offset++;
	}
}

void offset_temp_method(int no){
	if (stack_temp[no] < 0){
		stack_temp[no] = offset;
		offset++;
	}
}

int paramcount(InterCodes * code){
	int i = 1;
	InterCodes * p = code;
	p = p->next;
	while (p->code.kind == icPARAM){
		i++;
		p = p->next;
	}
	return i;
}
/*
void spimoffset(InterCodes * code){
  int i;
  for (i = 0 ; i<MAX_VARIABLE ; i++){
  	stack_sp[i] = -1;
  	stack_temp[i] = -1;
  	param[i] = -1;
  }
  InterCodes * p = code;
    if (p->code.kind == icASSIGN){
		if (p->code.u.assign.left->kind == opVARIABLE) offset_var_method(p->code.u.assign.left->u.var_no);
		if (p->code.u.assign.left->kind == opTEMP) offset_temp_method(p->code.u.assign.left->u.temp_no);		
	} 
    if (p->code.kind == icBINOP){
		if (p->code.u.binop.result->kind == opVARIABLE) offset_var_method(p->code.u.binop.result->u.var_no);
		if (p->code.u.binop.result->kind == opTEMP) offset_temp_method(p->code.u.binop.result->u.temp_no);
	}
    if (p->code.kind == icREAD){
		offset_var_method(p->code.u.read.op->u.var_no);
	}
    if (p->code.kind == icCALL){
		if (p->code.u.call.returnop->kind == opVARIABLE) offset_var_method(p->code.u.call.returnop->u.var_no);
		if (p->code.u.call.returnop->kind == opTEMP) offset_temp_method(p->code.u.call.returnop->u.temp_no);
	}

  offset = (offset + 4) * 4;
  for (i= 0; i< MAX_VARIABLE ; i++){
	if (stack_sp[i] > 0) stack_sp[i] = offset - 4*(stack_sp[i]);
	if (stack_temp[i] > 0) stack_temp[i] = offset - 4*(stack_temp[i]);
  }
}
*/
void fspimcode(FILE * file, InterCodes * code) {
  if (code == NULL) {
    fprintf(file, "no intermediate code\n");
    return ;
  }
  int i = 0;
  InterCodes * p = code;
  while (p != NULL) {
    switch (p->code.kind) {
      case icASSIGN:{
	Operand * opleft = p->code.u.assign.left;
	Operand * opright = p->code.u.assign.right;
	if (opright->kind == opCONSTANT) {
			fprintf(file, "li $t0, %d\n" , opright->u.value);
			if (opleft->kind == opVARIABLE) fprintf(file ,"sw $t0 , %d($fp)" , fpoffset(opleft) );
			else fprintf(file , "sw $t0 , %d($fp)" , fpoffset(opleft) );
		}
	if (opright->kind == opVARIABLE || opright->kind == opVARADDRESS) {
		fprintf(file , "lw $t0 , ");
		fprintf(file , "%d($fp)\n" , fpoffset(opright));
		if (opleft->kind == opVARIABLE) fprintf(file ,"sw $t0 , %d($fp)" , fpoffset(opleft) );
			else fprintf(file , "sw $t0 , %d($fp)" , fpoffset(opleft) );
	}
        //fspimoperand(file, p->code.u.assign.left);
        //fprintf(file, ", ");
        break;
	}
      case icGETADDR:
        //fprintoperand(file, p->code.u.getaddr.result);
        fprintf(file, "lw $t0 %d($fp)\n" , fpoffset(p->code.u.getaddr.op) );
        //fprintoperand(file, p->code.u.getaddr.op);
	int addroffset = -4 - p->code.u.getaddr.size;
        fprintf(file, "addi $t1, $t0, %d\n", addroffset);
	fprintf(file , "sw $t1 %d($fp)" , fpoffset(p->code.u.getaddr.result));
	arrayoffset[-1*fpoffset(p->code.u.getaddr.result)] = true;
        break;
      case icBINOP:{
	if (p->code.u.binop.op1->kind == opCONSTANT){
		fprintf(file , "li $t0 , %d\n" , p->code.u.binop.op1->u.value);
	}	
	else{
		fprintf(file , "lw $t0 , %d($fp)\n" , fpoffset(p->code.u.binop.op1) );
	}
	
	if (p->code.u.binop.op2->kind == opCONSTANT){
		fprintf(file , "li $t1 , %d\n" , p->code.u.binop.op2->u.value);
	}
	else{
		fprintf(file , "lw $t1 , %d($fp)\n" , fpoffset(p->code.u.binop.op2) );
	}
	bool fdiv = false;
	if (arrayoffset[ -1*fpoffset(p->code.u.binop.op1) ]){
		fprintf(file , "li $t3 -1\n");		
		fprintf(file , "mul $t1 $t1 $t3\n");
	}
	switch (p->code.u.binop.sign){
		case 3:
			fprintf(file , "add ");			
		break;
		case 4:
			fprintf(file , "sub ");
		break;
		case 5:
			fprintf(file , "mul ");		
		break;
		case 6:
			fprintf(file , "div ");
			fdiv = true;
		break;
			
	} 
	if (fdiv){
		fprintf(file , "$t0 , $t1\n");
		fprintf(file , "mflo $t2\n");
	}
	else {
	fprintf(file , "$t2, $t0, $t1\n");
	}
	fprintf(file , "sw $t2 %d($fp)" , fpoffset(p->code.u.binop.result) );
        break;
	}
      case icLABEL:
        fprintf(file, " label%d " , p->code.u.label.label->u.var_no);
        //fspimoperand(file, p->code.u.label.label);
	fprintf(file , ":");
        break;
      case icFUNCDEF:
        fprintf(file, "\n");
        fprintf(file, "%s :\n", p->code.u.funcdef.funcname);
	currentfp = 0;
	if (checkmain(p->code.u.funcdef.funcname)) {
			fprintf(file , "\n");
			fprintf(file , "move $fp $sp\n");
		}
	fprintf(file , "addi $sp, $sp, -2048\n");
        break;
      case icMEMREAD:
        //fspimoperand(file, p->code.u.memread.result);
        fprintf(file, "lw $t0 %d($fp)\n" , fpoffset(p->code.u.memread.op) );
	fprintf(file, "add $t0, $t0, $fp\n");
	fprintf(file , "lw $t1 0($t0)\n");
	fprintf(file , "sw $t1 , %d($fp)" , fpoffset(p->code.u.memread.result));
        //fspimoperand(file, p->code.u.memread.op);
        break;
      case icMEMWRITE:
        //fspimoperand(file, p->code.u.memwrite.result);
        if (p->code.u.memwrite.op->kind == opCONSTANT) fprintf(file, "li $t0 %d\n" , p->code.u.memwrite.op->u.value );
	else fprintf(file , "lw $t0 %d($fp)\n" , fpoffset(p->code.u.memwrite.op));
	fprintf(file , "lw $t1 %d($fp)\n" , fpoffset(p->code.u.memwrite.result));
	fprintf(file , "add $t1, $t1, $fp\n");
	fprintf(file , "sw $t0 0($t1)");        
	//if (p->code.u.memwrite.op->kind == opADDRESS) fprintf(file, " *");
        //fspimoperand(file, p->code.u.memwrite.op);
        break;
      case icGOTOBRANCH:
        fprintf(file, "j");
        fprintf(file, " label%d" , p->code.u.gotobranch.label->u.var_no );
        break;
      case icIFBRANCH:{
	if (p->code.u.ifbranch.op1->kind == opCONSTANT) fprintf(file , "li $t0 , %d\n" , p->code.u.ifbranch.op1->u.temp_no);
	   else	fprintf(file , "lw $t0 , %d($fp)\n" , fpoffset(p->code.u.ifbranch.op1));
	if (p->code.u.ifbranch.op2->kind == opCONSTANT) fprintf(file , "li $t1 , %d\n" , p->code.u.ifbranch.op2->u.temp_no);
	   else	fprintf(file , "lw $t1 , %d($fp)\n" , fpoffset(p->code.u.ifbranch.op2));
	switch (p->code.u.ifbranch.relop->u.relop){
	case 0:
		fprintf(file , "blt ");
	break;
	case 1:
		fprintf(file , "ble ");
	break;
	case 2:
		fprintf(file , "bgt ");
	break;
	case 3:
		fprintf(file , "bge ");
	break;
	case 4:
		fprintf(file , "beq ");
	break;
	case 5:
		fprintf(file , "bne ");
	break;
	default:
	break;
	}	        
        fprintf(file, "$t0, $t1, label%d" , p->code.u.ifbranch.label->u.var_no);
        break;}
      case icFUNCRETURN:
	fprintf(file , "addi $sp, $sp, 2048\n");
	if (p->code.u.funcreturn.op->kind == opCONSTANT ) fprintf(file ,"li $v0 , %d" , p->code.u.funcreturn.op->u.var_no);
	else fprintf(file, "lw $v0 %d($fp)" , fpoffset(p->code.u.funcreturn.op) );
	fprintf(file ,"\n");
	fprintf(file ,"jr $ra");
        break;
      case icMEMDEC:
        //fprintf(file, " DEC ");
        fprintf(file , "li $t0 %d\n" , fpoffset( p->code.u.memdec.op ) );
	fprintf(file , "sw $t0 %d($fp)" , fpoffset( p->code.u.memdec.op) );
	currentfp = currentfp - p->code.u.memdec.size ;
        //fprintf(file, " %d ", p->code.u.memdec.size);
        break;
      case icARG:
        if (p->code.u.arg.arg->kind == opCONSTANT) {
		fprintf(file , "li $t%d %d, " , currentArg , p->code.u.arg.arg->u.value);
	}
	else fprintf(file , "lw $t%d , %d($fp)\n" , currentArg ,fpoffset(p->code.u.arg.arg) );
	currentArg ++;
        break;
      case icCALL:{
	recordfp = currentfp;
	int i = 0;
	for (i=0 ; i<currentArg ; i++) {
		currentfp = currentfp - 4;
		fprintf(file , "sw $t%d %d($fp)\n" , currentArg-1-i , currentfp);
	}	
	currentArg =0;
	currentfp = currentfp - 4;
	fprintf(file , "sw $fp %d($fp)\n" , currentfp);
	currentfp = currentfp - 4;
	fprintf(file , "sw $ra %d($fp)\n" , currentfp);
        fprintf(file,"addi $fp , $fp , %d\n" , currentfp);
	fprintf(file, "jal %s\n" , p->code.u.call.funcname);
	currentfp = 0;
	fprintf(file, "lw $ra , 0($fp)\n");
	fprintf(file , "lw $fp , 4($fp)\n");
	currentfp = recordfp;
	if (p->code.u.call.returnop != NULL)
	fprintf(file , "sw $v0 %d($fp)" , fpoffset(p->code.u.call.returnop));
        //fprintf(file, " := CALL %s ", p->code.u.call.funcname);
        break;}
      case icPARAM:{
	if (fparam) { count = paramcount(p); fparam = false;}
	fprintf(file , "lw $t1 %d($fp)\n" , 8+(count-1-currentParam)*4);
	fprintf(file , "sw $t1 %d($fp)" , fpoffset(p->code.u.param.param));
	InterCodes * q = p->next;
	if (q->code.kind == icPARAM) currentParam++;
	else { currentParam = 0; fparam = true;}
        break;}
      case icREAD:
	fprintf(file , "li $v0 5\n");
	fprintf(file , "syscall\n");
	fprintf(file , "sw $v0, %d($fp)" , fpoffset(p->code.u.read.op) );
        break;
      case icWRITE:
	if (p->code.u.write.op->kind == opADDRESS) {
		fprintf(file , "lw $t0 , %d($fp)\n" , fpoffset(p->code.u.write.op));
		fprintf(file , "add $t0, $t0, $fp\n");
		fprintf(file , "lw $a0 , 0($t0)\n");
	}else if (p->code.u.write.op->kind == opCONSTANT){
		fprintf(file , "li $a0 , %d\n" , p->code.u.write.op->u.value);
	}else fprintf(file , "lw $a0 ,  %d($fp)\n" , fpoffset(p->code.u.write.op) );
	fprintf(file , "li $v0 1\n");
	fprintf(file , "syscall\n");
	fprintf(file , "li $v0 4\n");
	fprintf(file , "la $a0 _ret\n");
	fprintf(file , "syscall\n");
        
        break;
      default :
        break;
    }
    fprintf(file, "\n");
    p = p->next;
  }
  return ;
}
