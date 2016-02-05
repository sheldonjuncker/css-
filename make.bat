flex flex.cpp
bison bison.cpp --report=all
g++ lex.yy.c
@echo Finished making %1 %2.