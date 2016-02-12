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
	Why not remove all of these and simply use characters? The parser is case-insensetive so it shouldn't make a difference.
*/

%%

{s} /* ignore spaces */

\/\*[^*]*\*+([^/*][^*]*\*+)*\/       /* ignore comments */
\/\/.*
"#!".*

"~="                    {return INCLUDES;}
"|="                    {return DASHMATCH;}

{string}                { yylval.string = strdup(yytext);
                          return STRING;
                        }

{ident}	                { yylval.string = strdup(yytext);
                          return IDENT;}

"$"{ident}				{ yylval.string = strdup(yytext); return VAR; }

"#"[0-9a-f]{3}([0-9a-f]{3})?	{ yylval.string = strdup(yytext); return HASH; } /*
"#"{name}               { yylval.string = strdup(yytext); return HASH;}
*/

"@import"     		{return IMPORT_SYM;}
"@page"           	{return PAGE_SYM;}
"@media"       		{return MEDIA_SYM;}
"@charset"          {return CHARSET_SYM;}
"!important"        {return IMPORTANT_SYM;} 
{num}({ident}|%)			{yylval.string = strdup(yytext); return DIMENSION;}
{num}			        { yylval.string = strdup(yytext); return NUMBER;}

"url("{string}")" {return URI;}
"url("{url}")"    {return URI;}

{ident}"("		        {yylval.string = strdup(yytext); return FUNCTION;}

.			            {return *yytext;}

%%