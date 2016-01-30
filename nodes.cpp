/*
	Nodes for TCSS
*/

//Definitions
class Node;
class Nodes;

//The Node Class
class Node
{
	public:
	virtual int *evaluate() = 0;
	void print(std::string s, bool nl = true)
	{
		std::cout << s;
		if(nl)
		{
			std::cout << "\n";
		}
	}
};

//List of Nodes (Statements)
class Nodes : public std::list<Node *>
{
	public:
	//Evaluate All Nodes in a List
	int *evaluate()
	{
		Nodes::iterator it;
		for(it=this->begin(); it != this->end(); ++it)
		{
			(*it)->evaluate();
		}
	}
};

/*
	Node Types
*/

//The Entire Stylesheet
class StyleNode : public Node
{
	public:
	Node *charset;
	Node *import;
	Nodes *body;
	
	StyleNode(Node *c, Node *i, Nodes *b)
	{
		this->charset = c;
		this->import = i;
		this->body = b;
	}
	
	int *evaluate()
	{
		if(this->charset)
			this->charset->evaluate();
		
		if(this->import)
			this->import->evaluate();
		
		if(this->body)
			this->body->evaluate();
		return NULL;
	}
};

class CharsetNode : public Node
{
	public:
	std::string charset;
	CharsetNode(std::string charset)
	{
		this->charset = charset;
	}
	
	int *evaluate()
	{
		print("@charset \"" + charset + "\"");
		return NULL;
	}
};

class ImportNode : public Node
{
	public:
	std::string import;
	ImportNode(std::string import = NULL)
	{
		this->import = import;
	}
	
	int *evaluate()
	{
		print("@import \"" + import + "\"");
		return NULL;
	}
};

//Operator Node
class OpNode : public Node
{
	public:
	Node *left, *right;
	char op;
	OpNode(Node *left, char op, Node *right)
	{
		this->left = left;
		this->op = op;
		this->right = right;
	}
	
	int *evaluate()
	{
		return NULL;
	}
};

/*
	Declaration Node
	Example:
		font:arial serif 30px !important;
*/
class DeclNode : public Node
{
	public:
	std::string prop;
	Nodes *value;
	bool important;
	
	DeclNode(std::string p, Nodes *v, bool i=false)
	{
		this->prop = p;
		this->value = v;
		this->important = i;
	}
	
	int *evaluate()
	{
		std::cout << "\t" << prop << ": ";
		this->value->evaluate();
		if(this->important)
		{
			std::cout << "!important";
		}
		std::cout << ";\n";
		return NULL;
	}
};

//String Node
class StrNode : public Node
{
	public:
	std::string str;
	StrNode(std::string s)
	{
		this->str = s;
	}
	
	int *evaluate()
	{
		print(str, false);
		return NULL;
	}
};

//Identifier Node
class IdNode : public Node
{
	public:
	std::string id;
	IdNode(std::string i)
	{
		this->id = i;
	}
	
	int *evaluate()
	{
		print(id, false);
		return NULL;
	}
};

/*
	Function Node
	Example:
		nth-of-child(even)
*/
class FuncNode : public Node
{
	public:
	std::string name;
	Node *exp;
	FuncNode(std::string n, Node *e)
	{
		this->name = n;
		this->exp = e;
	}
	
	int *evaluate()
	{
		std::cout << name;
		if(this->exp)
			this->exp->evaluate();
		std::cout << ")";
		
		return NULL;
	}
};

/*
	Page Node
	Example:
		@page [:first] { color:red; }
*/
class PageNode : public Node
{
	public:
	std::string type;
	Nodes *declarations;
	
	PageNode(std::string t, Nodes *d)
	{
		this->type = t;
		this->declarations = d;
	}
	
	int *evaluate()
	{
		std::cout << "@page";
		if(this->type != "")
			std::cout << " :" << type;
		std::cout << "{\n";
		this->declarations->evaluate();
		std::cout << "}";
		
		return NULL;
	}
};

/*
	Ruleset Node
	Example:
		.css { color:red; }
*/
class RulesetNode : public Node
{
	public:
	Nodes *selector_list;
	Nodes *declarations;
	
	RulesetNode(Nodes *s, Nodes *d = NULL)
	{
		this->selector_list = s;
		this->declarations = d;
	}
	
	int *evaluate()
	{
		this->selector_list->evaluate();
		print("{");
		if(this->declarations)
			this->declarations->evaluate();
		print("}");
		return NULL;
	}
};

/*
	Pseudo Selector Node
	Example:
		a:hover | a:nth-of-type(even)
*/
class PseudoSelectorNode : public Node
{
	public:
	Node *selector;
	Node *pseudo;
	
	PseudoSelectorNode(Node *s, Node *p)
	{
		this->selector = s;
		this->pseudo = p;
	}
	
	int *evaluate()
	{
		this->selector->evaluate();
		this->pseudo->evaluate();
		return NULL;
	}
};

/*
	Pseudo Block Node
	Example:
		:hover | :nth-of-type(even)
*/
class PseudoBlockNode : public Node
{
	public:
	std::string ident;
	Node *function = NULL;
	
	PseudoBlockNode(std::string i)
	{
		this->ident = i;
	}
	
	PseudoBlockNode(Node *f)
	{
		this->function = f;
	}
	
	int *evaluate()
	{
		if(this->ident != "")
			print(":" + this->ident, false);
		else
		{
			std::cout << ":";
			this->function->evaluate();
		}
		return NULL;
	}
};

/*
	Separator Node
	Used for printing out commas and spaces in lists of items.
	Example
		html, body | html body
*/
class SeparatorNode : public Node
{
	public:
	std::string sep;
	
	SeparatorNode(std::string s)
	{
		this->sep = s;
	}
	
	int *evaluate()
	{
		std::cout << sep;
		return NULL;
	}
};


/*
	Selector Node
	Example:
		body | .class | #id
*/
class SelectorNode : public Node
{
	public:
	std::string name;
	
	SelectorNode(std::string n)
	{
		this->name = n;
	}
	
	int *evaluate()
	{
		print(this->name, false);
		return NULL;
	}
};

/*
	Combinator Node
	Example:
		h1 + h2 | h1 > h2 | h1 ~ h2
*/
class CombinatorNode : public Node
{
	public:
	Node *left;
	char op;
	Node *right;
	
	CombinatorNode(Node *l, char o, Node *r)
	{
		this->left = l;
		this->right = r;
		this->op = o;
	}
	
	int *evaluate()
	{
		this->left->evaluate();
		std:: cout << " " << this->op << " ";
		this->right->evaluate();
		return NULL;
	}
};

/*
	Attribute Selector Node
	Example:
		[tcss] | [tcss=value]
*/
class AttrSelectNode : public Node
{
	public:
	std::string ident;
	Node *value;
	
	AttrSelectNode(std::string i, Node *v = NULL)
	{
		this->ident = i;
		this->value = v;
	}
	
	int *evaluate()
	{
		print("[", false);
		print(this->ident, false);
		if(this->value)
		{
			print("=", false);
			this->value->evaluate();
		}
		print("]", false);
		return NULL;
	}
};

//URI Node
class UriNode : public Node
{
	public:
	std::string uri;
	UriNode(std::string u)
	{
		this->uri = u;
	}
	
	int *evaluate()
	{
		print(this->uri, false);
		return NULL;
	}
};

//Number Node
class NumNode : public Node
{
	public:
	double num;
	NumNode(double n)
	{
		this->num = n;
	}
	
	int *evaluate()
	{
		std::cout << num;
		return NULL;
	}
};

//Percent Node
class PerNode : public Node
{
	public:
	double per;
	PerNode(double p)
	{
		this->per = p;
	}
	
	int *evaluate()
	{
		std::cout << per;
		return NULL;
	}
};

//Dimension Node
class DimNode : public Node
{
	public:
	double dim;
	std::string type;
	DimNode(double d, std::string t)
	{
		this->dim = d;
		this->type = t;
	}
	
	int *evaluate()
	{
		std::cout << dim << type;
		return NULL;
	}
};

/*
	Hash Node
	Example:
		#33aaff | #3af
*/
class HashNode : public Node
{
	public:
	std::string hash;
	HashNode(std::string h)
	{
		this->hash = h;
	}
	
	int *evaluate()
	{
		print(hash, false);
		return NULL;
	}
};