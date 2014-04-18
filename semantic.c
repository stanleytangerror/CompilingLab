#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "semantic.h"

FieldList* varlist[MAX_VARIABLE];

unsigned int hash_pjw(char *name){
	unsigned int val = 0, i;
	for (; *name ; ++ name){
		val = (val << 2) + *name;
		if (i = val & ~0x3fff)
		    val = (val ^ (i>>12)) & 0x3fff;
	}
	return val;
}

void InitialHashTable(){
	int i=0;
	for (i=0 ; i<MAX_VARIABLE; i++){
		varlist[i] = NULL ;	
	}
}
void InitialFieldList(FieldList * variable){
	variable->type = NULL;
	variable->tail = NULL;
}

int addVar(FieldList* variable){
	char *name = variable->name;
	unsigned int probe = hash_pjw(name);
	while (varlist[probe] != NULL){
		char *Hashname = varlist[probe]->name;		
		if ( strcmp(name , Hashname) != 0 ) probe++;
		else return -1; 	
	}
	varlist[probe] = variable;
}

int findVar(char *name){
	unsigned int probe = hash_pjw(name);
	while (varlist[probe] != NULL){
		char *Hashname = varlist[probe]->name;		
		if ( strcmp(name , Hashname) == 0 ) return probe;
		else probe++;
	}
	return -1;	
}







