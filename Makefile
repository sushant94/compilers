# Makefile example -- scanner and parser.
LEX     = lex -l
YACC    = yacc
YFLAGS  = -dt
objects = scanner.o parser.o symbol.o
								 
symbol: $(objects)
scanner.o: scanner.l parser.c
parser.o: parser.y
symbol.o: symbol.c
 
clean: 
	rm *.o parser.c y.tab.h
