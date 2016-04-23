tcssmake:
	flex flex.cpp
	bison bison.cpp --report=all
	g++ -o tcss tcss.cpp lex.yy.c -static 