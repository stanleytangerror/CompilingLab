#ifndef __OPTIMIZE_H__
#define __OPTIMIZE_H__

#include "tree.h"
#include "semantic.h"
#include "intercode.h"

void getoperand(Operand * op);

void exchange(InterCodes * code);

void getcode(InterCodes * code);

void deletecode(InterCodes *p);

void add(InterCodes *p , InterCodes * q);

void getcode(InterCodes * code);

void optimizecode();

#endif
