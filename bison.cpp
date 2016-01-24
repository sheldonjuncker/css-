//The TCSS Bison File
%{
	#include <iostream>
	#include <cstdlib>
	#include <list>
	#include "value.cpp"
	#include "nodes.cpp"
	
	int yyerror(const char *p)
	{
		printf("%s\n", p);
		exit(1);
	}
	int yylex(void);
%}

%start stylesheet

%union{
    char *string;
	Node *exp;
	Nodes *stmts;
}

%token CHARSET_SYM
%token <string> DASHMATCH
%token <string> DIMENSION
%token <string> STRING
%token FUNCTION
%token <string> HASH
%token <string> IDENT
%token <string> INCLUDES
%token IMPORT_SYM IMPORTANT_SYM
%token MEDIA_SYM
%token <string> NUMBER
%token PAGE_SYM
%token <string> PERCENTAGE 
%token <string> URI

%type <string> type_selector
%type <string> id_selector
%type <string> class_selector

%type <string> attrib_eq
%type <string> attrib_value

%type <string> term
%type <string> property
%type <string> expr

%type <stmts> stylesheet
%type <exp> charset

%error-verbose
%locations

%%

stylesheet // : [ CHARSET_SYM STRING ';' ]?
           //   [S|CDO|CDC]* [ import [ CDO S* | CDC S* ]* ]*
           //   [ [ ruleset | media | page ] [ CDO S* | CDC S* ]* ]* ;
    : charset import_block body {
		
	}
;

charset
    : /* empty */
	{
		$$ = NULL;
	}
    | CHARSET_SYM STRING ';'
    {
        $$ = new CharsetNode($2);
    }
;

import_block
    :
    | import
;

body
    :
    | body ruleset
    | body media
    | body page
;

/* Import should be able to have media queries if they are to be included in the language. */
import // : IMPORT_SYM S* [STRING|URI] S* media_list? ';' S* ;
    : IMPORT_SYM STRING media_list ';'
    | IMPORT_SYM URI media_list ';'
    | IMPORT_SYM STRING ';'
    | IMPORT_SYM URI ';'
;
    
media // : MEDIA_SYM S* media_list '{' S* ruleset* '}' S* ;
    : MEDIA_SYM media_list '{' rulesets '}'
;

rulesets
    :
    | rulesets ruleset
;

media_list // : medium [ COMMA S* medium]* ;
    : medium
    | media_list ',' medium
;

medium // : IDENT S* ;
    : IDENT
;

page // : PAGE_SYM S* pseudo_page?
     //   '{' S* declaration? [ ';' S* declaration? ]* '}' S* ;
    : PAGE_SYM pseudo_page '{' page_declarations '}'
    | PAGE_SYM '{' page_declarations '}'
;

page_declarations
    : declaration
    | page_declarations ';' declaration
    | page_declarations ';'
;

pseudo_page // : ':' IDENT S* ;
    : ':' IDENT
;

operator // : '/' S* | ',' S* ;
    : '/'
    | ','
;

combinator // : '+' S* | '>' S* ;
    : '+'
    {
        
    }
    | '>'
    {
       
    }
;

unary_operator // : '-' | '+' ;
    : '-'
    | '+'
;

property // : IDENT S* ;
    : IDENT
    {
        $$ = $1;
    }
;

ruleset // : selector [ ',' S* selector ]* '{' S* declaration? [ ';' S* declaration? ]* '}' S* ;
    : selector_list '{' declarations '}'
    | selector_list '{' '}'
;

selector_list
    : complex_selector
    | universal_selector
    | selector_list ',' complex_selector
    | selector_list ',' universal_selector
;

/*
	I changed the following rules because it seemed that a complex selector matches any number of compound selectors which match any number of simple selectors. Therefore, the rules complex -> compound and complex -> complex compound are logically the same. (I previously removed the spaces between complex and compound which resulted in shift/reduce errors.) 
	
	It should now work.
*/

complex_selector // : simple_selector [ combinator selector | S+ [ combinator? selector ]? ]? ;
    : compound_selector
    | complex_selector combinator compound_selector
;

universal_selector
    :
    | '*'
;

compound_selector // : element_name [ HASH | class | attrib | pseudo ]* | [ HASH | class | attrib | pseudo ]+ ;
    : '*' type_selector
    | type_selector
    | '*' simple_selector
    | simple_selector
    | compound_selector simple_selector
;

simple_selector
    : attribute_selector
    | class_selector
    | id_selector
    | pseudo_class_selector
;

id_selector
    : '#' IDENT
    {  }
;

class_selector // : '.' IDENT ;
    : '.' IDENT
    {  }
;

type_selector // : IDENT | '*' ;
    : IDENT
    {  }
;

attribute_selector // : '[' S* IDENT S* [ [ '=' | INCLUDES | DASHMATCH ] S* [ IDENT | STRING ] S* ]? ']';
    : '[' IDENT ']'
    {  }
    | '[' IDENT attrib_eq attrib_value ']'
    {  }
;

attrib_eq
    : '='
    {   $$ = "=";    }
    | INCLUDES
    {   $$ = $1;    }
    | DASHMATCH
    {   $$ = $1;    }
;

attrib_value
    : IDENT
    {   $$ = $1;    }
    | STRING
    {   $$ = $1;    }
;

pseudo_class_selector // : ':' [ IDENT | FUNCTION S* [IDENT S*]? ')' ] ;
    : ':' pseudo_block
;

pseudo_block
    : IDENT
    | FUNCTION pseudo_block_function_ident ')'
;

pseudo_block_function_ident
    :
    | IDENT
;

declarations
    : declaration
    | declarations ';' declaration
    | declarations ';'
;

declaration // : property ':' S* expr prio? ;
    : property ':' expr prio
    {  }
    | property ':' expr
    {  }
;

prio // : IMPORTANT_SYM S* ;
    : IMPORTANT_SYM
;

expr //: term [ operator? term ]*;
    : term
    {
        $$ = $1;
    }
    | expr operator term
    | expr term
;

term // : unary_operator?
     // [ NUMBER S* | PERCENTAGE S* | LENGTH S* | EMS S* | EXS S* | ANGLE S* | TIME S* | FREQ S* ]
     // | STRING S* | IDENT S* | URI S* | hexcolor | function ;
    : unary_operator term_numeral
    | term_numeral
    | STRING
    {
        $$ = $1;
    }
    | IDENT
    {
        $$ = $1;
    }
    | URI
    {
        $$ = $1;
    }
    | hexcolor
    | function
;

term_numeral
    : NUMBER
    | PERCENTAGE
    | DIMENSION
;      

function // : FUNCTION S* expr ')' S* ;
    : FUNCTION expr ')'
;


// There is a constraint on the color that it must
// have either 3 or 6 hex-digits (i.e., [0-9a-fA-F])
// after the "#"; e.g., "#000" is OK, but "#abcd" is not.

hexcolor // : HASH S* ;
    : HASH
;

%%

/*
	I'll need nodes (classes) for the following:
	
	
*/

main()
{
    yyparse();
}