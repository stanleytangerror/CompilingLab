file=testcodes/test01.c

analyze:
	flex lexical.l
	bison -d -v syntax.y
	gcc -g main.c tree.c syntax.tab.c -lfl -ly -o parser
	./parser $(file)

debug:
	flex lexical.l
	bison -d -t syntax.y
	gcc main.c tree.c syntax.tab.c -lfl -ly -o parser
	./parser $(file)

clean:
	-rm *~
	-rm lex.yy.c
	-rm syntax.tab.*
	-rm parser
