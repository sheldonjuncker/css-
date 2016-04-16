//The TCSS Bison File
%{
	#include <iostream>
	#include <stdio.h>
	#include <cstdlib>
	#include <list>
	#include <map>
	#include "tcss.cpp"
	#include "to_string.cpp"
	#include "value.cpp"
	#include "UserAgent.cpp"
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
	char *op;
}

%token CHARSET_SYM
%token <string> DASHMATCH
%token <string> DIMENSION
%token <string> STRING
%token <string> FUNCTION
%token <string> HASH
%token <string> IDENT VAR
%token <string> INCLUDES
%token IMPORT_SYM IMPORTANT_SYM MEDIA_SYM
%token <string> NUMBER
%token PAGE_SYM
%token <string> URI
%token <op> CMP_OP
%token IF
%left '!'
%left AND OR

%type <c> operator combinator
%type <node_list> body expr_list declarations selector_list rulesets media_query_list media_expr_list
%type <node> stylesheet charset import_block expr term term_numeral hexcolor page attrib_eq attrib_value function declaration  pseudo_block pseudo_expr pseudo_class_selector attribute_selector id_selector type_selector class_selector simple_selector compound_selector universal_selector complex_selector ruleset variables media_expr media_query if cond_expr
%type <string> media_feature media_type

%error-verbose
%locations

%%

stylesheet // : [ CHARSET_SYM STRING ';' ]?
           //   [S|CDO|CDC]* [ import [ CDO S* | CDC S* ]* ]*
           //   [ [ ruleset | media | page ] [ CDO S* | CDC S* ]* ]* ;
    : charset import_block body {
		$$ = new StyleNode($1, $2, $3);
		std::cout << $$->evaluate();
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
	| body MEDIA_SYM media_query_list '{' rulesets '}'
	{
		$1->push_back(new MediaQueryNode($3, $5));
		$$ = $1;
	}
	| body variables
	{
		$1->push_back($2);
		$$ = $1;
	}
	| body if
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
/*
	Begin:	IF
	Desc:	This section is the grammar for if statements.
	They take the form of:
	@if condition{
		.css{
			
		}
	}
*/
if
	: IF cond_expr '{' body '}'
	{
		$$ = new IfNode($2, $4);
	}
	
cond_expr
	: IDENT
	{
		$$ = new CondIdNode($1);
	}
	| IDENT CMP_OP expr
	{
		//For >=, <=, and =
		$$ = new CondCmpNode($1, $2, $3);
	}
	| IDENT '>' expr
	{
		$$ = new CondCmpNode($1, ">", $3);
	}
	| IDENT '<' expr
	{
		$$ = new CondCmpNode($1, "<", $3);
	}
	| cond_expr AND cond_expr
	{
		$$ = new CondAndNode($1, $3);
	}
	| cond_expr OR cond_expr
	{
		$$ = new CondOrNode($1, $3);
	}
	| '!' cond_expr
	{
		$$ = new CondNotNode($2);
	}
	| '(' cond_expr ')'
	{
		$$ = $2;
	}

media_query_list
	: media_query
	{
		$$ = new Nodes();
		$$->push_back($1);
	}
	| media_query_list OR media_query
	{
		//Separator or
		$1->push_back(new SeparatorNode(", "));
		$1->push_back($3);
		$$ = $1;
	}

media_query
	: IDENT media_expr_list
	{
		$$ = new QueryNode($1, $2);
	}

	| media_expr_list
	{
		$$ = new QueryNode("", $1);
	}

media_expr_list
	: media_expr
	{
		$$ = new Nodes();
		$$->push_back($1);
	}
	| media_expr_list AND media_expr
	{
		//Separator and
		$1->push_back(new SeparatorNode(" and "));
		$1->push_back($3);
		$$ = $1;
	}
	
media_expr
	: media_type
	{
		$$ = new IdNode($1);
	}
	| '(' media_feature ')'
	{
		$$ = new MediaFeatureNode($2, NULL, NULL);
	}
	| '(' media_feature CMP_OP expr ')'
	{
		$$ = new MediaFeatureNode($2, $3, $4);
	}
	| '(' media_feature ':' expr ')'
	{
		$$ = new MediaFeatureNode($2, ":", $4);
	}

media_type
	: IDENT
	{
		$$ = $1;
	}

media_feature
	: IDENT
	{
		$$ = $1;
	}


page // : PAGE_SYM S* pseudo_page?
     //   '{' S* declaration? [ ';' S* declaration? ]* '}' S* ;
    : PAGE_SYM ':' IDENT '{' declarations '}'
	{
		$$ = new PageNode($3, $5);
	}
    | PAGE_SYM '{' declarations '}'
	{
		$$ = new PageNode("", $3);
	}
;

operator // : '/' S* | ',' S* ;
    : '/'
	{
		$$ = '/'
	}
    | ','
	{
		$$ = ','
	}
	| '.'
	{
		$$ = '.'
	}
	| '*'
	{
		$$ = '*'
	}
	| '+'
	{
		$$ = '+'
	}
	| '-'
	{
		$$ = '-'
	}
;


	
combinator // : '+' S* | '>' S* ;
    : '+'
    {
        $$ = '+';
    }
    | '>'
    {
       $$ = '>';
    } 
	| '~' //css3 rule
	{
		$$ = '~';
	}
;

/*
	If a property is just an IDENT, it isn't a necessary part of the grammar.
	
property // : IDENT S* ;
    : IDENT
    {
        $$ = new IdNode($1);
    }
;
*/

/*
ruleset // : selector [ ',' S* selector ]* '{' S* declaration? [ ';' S* declaration? ]* '}' S* ;
    : selector_list '{' declarations '}'
	{
		$$ = new RulesetNode($1, $3);
	}
    | selector_list '{' '}'
	{
		$$ = new RulesetNode($1);	
	}
;
*/

ruleset // : selector [ ',' S* selector ]* '{' S* declaration? [ ';' S* declaration? ]* '}' S* ;
    : selector_list '{' '}'
	{
		$$ = new RulesetNode($1, NULL, NULL);
	}
	| selector_list '{' declarations '}'
	{
		$$ = new RulesetNode($1, $3, NULL);
	}
	| selector_list '{' rulesets '}'
	{
		$$ = new RulesetNode($1, NULL, $3);
	}
	| selector_list '{' declarations rulesets '}'
	{
		$$ = new RulesetNode($1, $3, $4);
	}
;

rulesets
	: ruleset
	{
		$$ = new Nodes();
		$$->push_back($1);
	}
	| rulesets ruleset
	{
		$1->push_back($2);
		$$ = $1;
	}
;
/*
	Variable Declarations and Assignments
*/
variables
	: VAR '{' declarations '}'
	{
		$$ = new VarDeclNode($1, $3);
	}
	| VAR '{' '}'
	{
		$$ = new VarDeclNode($1, NULL);	
	}
	| VAR ':' expr_list ';'
	{
		$$ = new VarAssignNode($1, $3);
	}

/*
	My simpler selector list
*/
selector_list
    : complex_selector
	{
		$$ = new Nodes();
		$$->push_back($1);
	}
	| selector_list ',' complex_selector
	{
		$1->push_back(new SeparatorNode(", "));
		$1->push_back($3);
		$$ = $1;
	}
	| selector_list complex_selector
	{
		$1->push_back(new SeparatorNode(" "));
		$1->push_back($2);
		$$ = $1;
	}
;

/*
	I changed the following rules because it seemed that a complex selector matches any number of compound selectors which match any number of simple selectors. Therefore, the rules complex -> compound and complex -> complex compound are logically the same. (I previously removed the spaces between complex and compound which resulted in shift/reduce errors.) 
	
	It should now work.
*/

complex_selector // : simple_selector [ combinator selector | S+ [ combinator? selector ]? ]? ;
    : compound_selector
	{
		$$ = $1;
	}
	| complex_selector combinator compound_selector
	{
		$$ = new CombinatorNode($1, $2, $3);
	}
    | universal_selector
	{
		$$ = $1;
	}
	| ':' ':' IDENT
	{
		$$ = new PseudoElementNode($3);
	}
	| pseudo_class_selector
	{
		$$ = $1;
	}
;

universal_selector
    : '*'
	{
		$$ = new SelectorNode("*");
	}
;

compound_selector // : element_name [ HASH | class | attrib | pseudo ]* | [ HASH | class | attrib | pseudo ]+ ;
    : type_selector
	{
		$$ = $1;
	}
    | simple_selector
	{
		$$ = $1;
	}
;

simple_selector
    : attribute_selector
	{
		$$ = $1;
	}
    | class_selector
	{
		$$ = $1;
	}
    | id_selector
	{
		$$ = $1;
	}/*
    | pseudo_class_selector
	{
		$$ = $1;
	}*/
;

id_selector
    : '#' IDENT
    { 
		std::string tmp = $2;
		$$ = new SelectorNode("#" + tmp);
	}
;

class_selector // : '.' IDENT ;
    : '.' IDENT
    { 
		std::string tmp = $2;
		$$ = new SelectorNode("." + tmp);
	}
;

type_selector // : IDENT | '*' ;
    : IDENT
    { 
		$$ = new SelectorNode($1);
	}
;

attribute_selector // : '[' S* IDENT S* [ [ '=' | INCLUDES | DASHMATCH ] S* [ IDENT | STRING ] S* ]? ']';
    : '[' IDENT ']'
    { 
		$$ = new AttrSelectNode($2);
	}
    | '[' IDENT attrib_eq attrib_value ']'
    { 
		$$ = new AttrSelectNode($2, $3, $4);
	}
;

attrib_eq
    : '='
    {   $$ = new StrNode("=");    }
    | INCLUDES
    {   $$ = new StrNode("~=");    }
    | DASHMATCH
    {   $$ = new StrNode("|=");    }
;

attrib_value
    : IDENT
    {   $$ = new IdNode($1);    }
    | STRING
    {   $$ = new StrNode($1);    }
;

pseudo_class_selector // : ':' [ IDENT | FUNCTION S* [IDENT S*]? ')' ] ;
    : ':' pseudo_block
	{
		$$ = $2;
	}
;

pseudo_block
    : IDENT
	{
		$$ = new PseudoBlockNode($1);
	}
    | function
	{
		$$ = new PseudoBlockNode($1);
	}
;

declarations
    : declaration ';'
	{
		$$ = new Nodes();
		$$->push_back($1);
	}
    | declarations declaration ';'
	{
		$1->push_back($2);
		$$ = $1;
	}
;

declaration // : property ':' S* expr prio? ;
	: IDENT ':' expr_list prio
    { 
		$$ = new DeclNode($1, $3, true);
	}
    | IDENT ':' expr_list
    { 
		$$ = new DeclNode($1, $3);
	}
	| VAR
	{
		$$ = new VarDeclIncNode($1);
	}
;

expr_list
	: expr
	{
		$$ = new Nodes();
		$$->push_back($1);
	}
	| expr_list expr
	{
		$1->push_back(new SeparatorNode(" "));
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
	| VAR
	{
		$$ = new VarNode($1);
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
		$$ = new NumNode($1);
	}
    | DIMENSION
	{
		//Fix this
		$$ = new DimNode($1);
	}
;      

function // : FUNCTION S* expr ')' S* ;
    : FUNCTION pseudo_expr ')'
	{
		$$ = new FuncNode($1, $2);
	}
;

pseudo_expr
	: 
	{
		$$ = NULL;
	}
	| expr
	{
		$$ = $1;
	}
	| pseudo_class_selector
	{
		$$ = $1;
	}

hexcolor // : HASH S* ;
    : HASH
	{
		$$ = new HashNode($1);
	}
;

%%

/*
	I'll need nodes (classes) for the following:
*/

int main(int argc, char **argv)
{
	//Comparsion Identifiers
	std::string agent_info[] = {
		//Browsers
		"IE",
		"Firefox",
		"Chrome",
		"Safari",
		"Opera",
		"AndroidBrowser",
		"KindleBrowser",
		"UnknownBrowser",
		"Version",
		//Operating Systems
		"Windows",
		"Linux",
		"Apple",
		"Nintendo",
		"PlayStation",
		"Xbox",
		"Kindle",
		"Android",
		"UnknownOS",
		//Platform Type
		"Mobile",
		"Desktop",
		"Console",
		"UnknownType"
	};
	
	//Initialize with False
	for(int i=0; i<(sizeof(agent_info) / sizeof(agent_info[0])); i++)
	{
		agent_idents[agent_info[i]] = 0;
	}
	
	//Gather and Parse User Agent Info
	AgentInfo *agent = UserAgent::getAgentInfo();
		
	std::string platform;
	std::string browser;
	std::string platform_type;
	
	//Platform
	if(!strncmp(agent->platform.c_str(), "Windows", 7))
	{
		platform = "Windows";
		if(agent->platform == "Windows")
			platform_type = "Desktop";
		else
			platform_type = "Mobile";
	}
	else if(agent->platform == "Linux")
	{
		platform = "Linux";
		platform_type = "Desktop";
	}
	else if(agent->platform == "Android")
	{
		platform = "Android";
		platform_type = "Mobile";
	}
	else if(agent->platform == "Kindle")
	{
		platform = "Kindle";
		platform_type = "Mobile";
	}
	else if(agent->platform == "Macintosh")
	{
		platform = "Apple";
		platform_type = "Desktop";
	}
	else if(agent->platform == "iPod")
	{
		platform = "Apple";
		platform_type = "Mobile";
	}
	else if(agent->platform == "iPhone")
	{
		platform = "Apple";
		platform_type = "Mobile";
	}
	else if(agent->platform == "iPad")
	{
		platform = "Apple";
		platform_type = "Mobile";
	}
	else if(!strncmp(agent->platform.c_str(), "Xbox", 4))
	{
		platform = "Xbox";
		platform_type = "Console";
	}
	else if(!strncmp(agent->platform.c_str(), "Nintendo", 8))
	{
		platform = "Nintendo";
		platform_type = "Console";
	}
	else if(!strncmp(agent->platform.c_str(), "PlayStation", 11))
	{
		platform = "PlayStation";
		platform_type = "Console";
	}
	else
	{
		platform = "UnknownOS";
		platform_type = "UnknownType";
	}
	
	//Browser
	if(agent->browser == "Firefox")
		browser = "Firefox";
	else if(agent->browser == "Chrome")
		browser = "Chrome";
	else if(agent->browser == "MSIE")
		browser = "IE";
	else if(agent->browser == "Safari")
		browser = "Safari";
	else if(agent->browser == "Opera")
		browser = "Opera";
	else if(agent->browser == "Kindle")
		browser = "KindleBrowser";
	else if(!strncmp(agent->browser.c_str(), "Android", 7))
		browser = "AndroidBrowser";
	else
		browser = "UnknownBrowser";
	
	
	//Add Platform
	agent_idents[platform] = 1;
	
	//Add Browser
	agent_idents[browser] = 1;
	
	//Add Browser Version
	agent_idents["Version"] = atoi(agent->version.c_str());
	
	extern FILE *yyin;
	
	//Process File (CGI)
	if(argc >= 2)
	{
		std::cout << "Content-type: text/css\n\n";
		yyin = fopen(argv[1], "r");
	}
	
	yyparse();
	
	AgentInfo *uagent = UserAgent::getAgentInfo();
	
	if(agent != NULL)
	{
		std::cout << uagent->platform;
		std::cout << uagent->browser;
		std::cout << uagent->version;
	}
	
	else
	{
		printf("Error!");
	}
	
	return 0;
}