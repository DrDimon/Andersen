objs = lexer.o y.tab.o Andersen
LEX=flex
PARS = bison -y
andersen :
	bison -y -v -d parser.y
	flex lexer.l
	g++ -std=c++11 -c y.tab.c lex.yy.c Object.cpp ObjectInstance.cpp ObjectPath.cpp TextFragment.cpp PlaceholderFragment.cpp IncludeFragment.cpp Expression.cpp PathPart.cpp
	g++ -std=c++11 y.tab.o lex.yy.o Object.o ObjectInstance.o ObjectPath.o TextFragment.o PlaceholderFragment.o IncludeFragment.o Expression.o PathPart.o -o Andersen

clean:
	rm lex.yy.c y.tab.c  y.tab.h Object.o ObjectInstance.o ObjectPath.o TextFragment.o PlaceholderFragment.o IncludeFragment.o Expression.o PathPart.o
