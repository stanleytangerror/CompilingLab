#ifndef __SPIM_H__
#define __SPIM_H__

#include "tree.h"
#include "semantic.h"
#include "intercode.h"
#include "optimize.h"


void spimcode();
void fspimcode(FILE * file, InterCodes * code);

#endif
