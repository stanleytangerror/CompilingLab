
analyze:
	flex lexical.l
	bison -d syntax.y
	gcc main.c syntax.tab.c -lfl -ly -o parser	
	./parser test.c

clean:
	-rm *~
	-rm lex.yy.c
	-rm syntax.tab.*
	-rm parser
