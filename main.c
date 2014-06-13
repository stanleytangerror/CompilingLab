#include <stdio.h>
#include "tree.h"
#include "semantic.h"
#include "intercode.h"
#include "spim.h"
#include "optimize.h"

extern FILE * yydebug;
//extern node * forest;
//extern void printnode(node *);

int main(int argc ,char** argv){
	if (argc<=1) return 1;
	
  compile = true;

	FILE *f = fopen(argv[1], "r");
	if (!f){
	  perror(argv[1]);
	  return 1;	
	}
	yyrestart(f);

//  yydebug = 1;
  yyparse();
  InitialHashTable();
  if (compile) {
    //traversal(forest, 0, printnode);
    traversal(forest, 0, nothingnode);
    //printf("============================\n");
    //traversal(forest, 0, addvariable);
    initialDefaultFunc();
    semantic(forest, NULL,NULL);
    //gettypelist();
    //getvarlist();
    //getfunclist();
    //getfuncdeclist();
    checkfunc();
    translate(forest);
    //printcode(ichead);
    //writecode();
    getcode(ichead);
    getcode(ichead);
    getcode(ichead);
    getcode(ichead);
    getcode(ichead);
    exchange(ichead);
    //optimizecode();
    spimcode();
  }

  return 0;
}
