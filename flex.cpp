%{
	#include "bison.tab.cpp"
%}
%option case-insensitive
%option noyywrap

h              [0-9a-f]
nonascii       [\240-\377]
unicode        \\{h}{1,6}(\r\n|[ \t\r\n\f])?
escape         {unicode}|\\[^\r\n\f0-9a-f]
nmstart        [_a-z]|{nonascii}|{escape}
nmchar         [_a-z0-9-]|{nonascii}|{escape}
string1        \"([^\n\r\f\\"]|\\{nl}|{escape})*\"
string2        \'([^\n\r\f\\']|\\{nl}|{escape})*\'
ident          -?{nmstart}{nmchar}*
name           {nmchar}+
num            (-|\+)?[0-9]+|[0-9]*"."[0-9]+
string         {string1}|{string2}
url	           ([!#$%&*-~]|{nonascii}|{escape})*
s              [ \t\r\n\f]+
w              {s}?
nl             \n|\r\n|\r|\f

/*
A		a|\\0{0,4}(41|61)(\r\n|[ \t\r\n\f])?
C		c|\\0{0,4}(43|63)(\r\n|[ \t\r\n\f])?
D		d|\\0{0,4}(44|64)(\r\n|[ \t\r\n\f])?
E		e|\\0{0,4}(45|65)(\r\n|[ \t\r\n\f])?
G		g|\\0{0,4}(47|67)(\r\n|[ \t\r\n\f])?|\\g
H		h|\\0{0,4}(48|68)(\r\n|[ \t\r\n\f])?|\\h
I		i|\\0{0,4}(49|69)(\r\n|[ \t\r\n\f])?|\\i
K		k|\\0{0,4}(4b|6b)(\r\n|[ \t\r\n\f])?|\\k
L       l|\\0{0,4}(4c|6c)(\r\n|[ \t\r\n\f])?|\\l
M		m|\\0{0,4}(4d|6d)(\r\n|[ \t\r\n\f])?|\\m
N		n|\\0{0,4}(4e|6e)(\r\n|[ \t\r\n\f])?|\\n
O		o|\\0{0,4}(4f|6f)(\r\n|[ \t\r\n\f])?|\\o
P		p|\\0{0,4}(50|70)(\r\n|[ \t\r\n\f])?|\\p
R		r|\\0{0,4}(52|72)(\r\n|[ \t\r\n\f])?|\\r
S		s|\\0{0,4}(53|73)(\r\n|[ \t\r\n\f])?|\\s
T		t|\\0{0,4}(54|74)(\r\n|[ \t\r\n\f])?|\\t
U       u|\\0{0,4}(55|75)(\r\n|[ \t\r\n\f])?|\\u
X		x|\\0{0,4}(58|78)(\r\n|[ \t\r\n\f])?|\\x
Z		z|\\0{0,4}(5a|7a)(\r\n|[ \t\r\n\f])?|\\z
*/
/*
	Why not remove all of these and simply use characters? The parser is case-insensetive so it shouldn't make a difference.
*/

%%

{s} /* ignore spaces */

\/\*[^*]*\*+([^/*][^*]*\*+)*\/       /* ignore comments */

"~="                    {return INCLUDES;}
"|="                    {return DASHMATCH;}

{string}                { yylval.string = strdup(yytext);
                          return STRING;
                        }

{ident}	                { yylval.string = strdup(yytext);
                          return IDENT;}

"#"[0-9a-f]{3}([0-9a-f]{3})?	{ yylval.string = strdup(yytext); return HASH; } /*
"#"{name}               { yylval.string = strdup(yytext); return HASH;}
*/

"@import"     		{return IMPORT_SYM;}
"@page"           	{return PAGE_SYM;}
"@media"       		{return MEDIA_SYM;}
"@charset"          {return CHARSET_SYM;}
"!important"        {return IMPORTANT_SYM;} /*
Why can't the above simply be "!important"?

{num}{E}{M}	            {return EMS;}
{num}{E}{X}	            {return EXS;}
{num}{P}{X}	            {return LENGTH;}
{num}{C}{M}	            {return LENGTH;}
{num}{M}{M}	            {return LENGTH;}
{num}{I}{N}	            {return LENGTH;}
{num}{P}{T}	            {return LENGTH;}
{num}{P}{C}	            {return LENGTH;}
{num}{D}{E}{G}		    {return ANGLE;}
{num}{R}{A}{D}		    {return ANGLE;}
{num}{G}{R}{A}{D}	    {return ANGLE;}
{num}{M}{S}		        {return TIME;}
{num}{S}		        {return TIME;}
{num}{H}{Z}		        {return FREQ;}
{num}{K}{H}{Z}		    {return FREQ;}
{num}{ident}		    {return DIMENSION;}
	Why not change all of these to {num}{ident}?
	Let the evaluator determine which is which as most (all?) will be printed out as text anyway.
*/
{num}{ident}			{return DIMENSION;}
{num}%			        {return PERCENTAGE;}
{num}			        {return NUMBER;}

"url("{string}")" {return URI;}
"url("{url}")"    {return URI;}

{ident}"("		        {yylval.string = strdup(yytext); return FUNCTION;}

.			            {return *yytext;}

%%