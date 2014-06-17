input=testcodes/A-1.c
output=spim.s

analyze:
	flex lexical.l
	bison -d syntax.y
	gcc -g main.c optimize.c spim.c intercode.c semantic.c tree.c syntax.tab.c -lfl -ly -o parser
	./parser $(input) $(output)

debug:
	flex lexical.l
	bison -d -t syntax.y
	gcc -g -ggdb3 main.c optimize.c spim.c intercode.c semantic.c tree.c syntax.tab.c -lfl -ly -o parser
	gdb ./parser

clean:
	-rm *~
	-rm testcodes/*.c~
	-rm lex.yy.c
	-rm syntax.tab.*
	-rm syntax.output
	-rm parser
	-rm core
	-rm core.*
