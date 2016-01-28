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
	Node *node;
	Nodes *node_list;
	char c;
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

%type <c> operator
%type <node_list> body expr_list
%type <node> stylesheet charset import_block expr term term_numeral hexcolor ruleset page media property attrib_eq attrib_value function

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
    | CHARSET_SYM STRING
    {
        $$ = new CharsetNode($2);
    }
;

/* Import should be able to have media queries if they are to be included in the language. */
import_block
    :
	{
		$$ = NULL;
	}
    | IMPORT_SYM STRING
	{ 
		$$ = new ImportNode($2);
	}
;

body
    :
	{
		$$ = new Nodes();
	}
    | body ruleset
	{
		$1->push_back($2);
		$$ = $1;
	}
    | body media
	{
		$1->push_back($2);
		$$ = $1;
	}
    | body page
	{
		$1->push_back($2);
		$$ = $1;
	}
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
	| '.'
	| '*'
	| '+'
	| '-'
;


	
combinator // : '+' S* | '>' S* ;
    : '+'
    {
        
    }
    | '>'
    {
       
    } 
	| '~' //css3 rule
	{
		
	}
;

property // : IDENT S* ;
    : IDENT
    {
        $$ = new IdNode($1);
    }
;

ruleset // : selector [ ',' S* selector ]* '{' S* declaration? [ ';' S* declaration? ]* '}' S* ;
    : selector_list '{' declarations '}'
    | selector_list '{' '}'
;

/*
	My simpler selector list
*/
selector_list
    : complex_selector
	| complex_selector ',' selector_list
	| complex_selector selector_list
;

/*
	I changed the following rules because it seemed that a complex selector matches any number of compound selectors which match any number of simple selectors. Therefore, the rules complex -> compound and complex -> complex compound are logically the same. (I previously removed the spaces between complex and compound which resulted in shift/reduce errors.) 
	
	It should now work.
*/

complex_selector // : simple_selector [ combinator selector | S+ [ combinator? selector ]? ]? ;
    : compound_selector
	| complex_selector combinator compound_selector
    | universal_selector
;

universal_selector
    : '*'
;

compound_selector // : element_name [ HASH | class | attrib | pseudo ]* | [ HASH | class | attrib | pseudo ]+ ;
    : type_selector
    | simple_selector
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
    {   $$ = new StrNode("=");    }
    | INCLUDES
    {   $$ = new StrNode($1);    }
    | DASHMATCH
    {   $$ = new StrNode($1);    }
;

attrib_value
    : IDENT
    {   $$ = new IdNode($1);    }
    | STRING
    {   $$ = new StrNode($1);    }
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
    : declaration ';'
    | declarations declaration ';'
;

declaration // : property ':' S* expr prio? ;
    : property ':' expr_list prio
    {  }
    | property ':' expr_list
    {  }
;

expr_list
	: expr
	{
		$$ = new Nodes();
		$$->push_back($1);
	}
	| expr_list expr
	{
		$1->push_back($2);
		$$ = $1;
	}

prio // : IMPORTANT_SYM S* ;
    : IMPORTANT_SYM
;

expr //: term [ operator? term ]*;
    : term
    {
        $$ = $1;
    }
    | expr operator term
	{
		$$ = new OpNode($1, $2, $3);
	}

term // : unary_operator?
     // [ NUMBER S* | PERCENTAGE S* | LENGTH S* | EMS S* | EXS S* | ANGLE S* | TIME S* | FREQ S* ]
     // | STRING S* | IDENT S* | URI S* | hexcolor | function ;
    : term_numeral
	{
		$$ = $1;
	}
    | STRING
    {
        $$ = new StrNode($1);
    }
    | IDENT
    {
        $$ = new IdNode($1);
    }
    | URI
    {
        $$ = new UriNode($1);
    }
    | hexcolor
	{
		$$ = $1;
	}
    | function
	{
		$$ = $1;
	}
	| '(' expr ')'
	{
		$$ = $2;
	}
;

term_numeral
    : NUMBER
	{
		$$ = new NumNode(atof($1));
	}
    | PERCENTAGE
	{
		$$ = new PerNode(atof($1));
	}
    | DIMENSION
	{
		$$ = new DimNode(atof($1), "px");
	}
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