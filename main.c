#include <stdio.h>
#include "tree.h"

extern FILE * yydebug;
//extern node * forest;
//extern void printnode(node *);

int main(int argc ,char** argv){
	if (argc<=1) return 1;
		
	FILE *f = fopen(argv[1], "r");
	if (!f){
	  perror(argv[1]);
	  return 1;	
	}
	yyrestart(f);

  yydebug = 1;
  yyparse();
	
  traversal(forest, 0, printnode);
  
  return 0;
}
