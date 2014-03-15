lex = lexical.l
file = test.c
analyze:
	flex $(lex)
	gcc lex.yy.c -o lex
	./lex $(file) | less

clean:
	-rm *.l~
	-rm lex.yy.c
	-rm lex
