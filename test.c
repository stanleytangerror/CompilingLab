#include<stdio.h>
#include<string.h>
void f(int i){
	printf("%d\n",i);
}

int main()
{
	char name[10];
	char check[10];
	char *n = name;
	char *c = check;
	scanf("%s" , n);
	printf("%s\n" , n);
	scanf("%s" , c);
	printf("%s\n" , c);
	strcpy(name , check);
	printf("%s\n" , name);
}
