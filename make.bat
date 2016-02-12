flex flex.cpp
bison bison.cpp --report=all
g++ lex.yy.c -static -o tcss
@echo Finished making %1 %2.