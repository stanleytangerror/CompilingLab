#ifndef __OPTIMIZE_H__
#define __OPTIMIZE_H__

#include "tree.h"
#include "semantic.h"
#include "intercode.h"

void getoperand(Operand * op);
void getcode(InterCodes * code);
void optimizecode();

#endif
