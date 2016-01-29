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
};

//List of Nodes (Statements)
class Nodes : public std::list<Node *>
{
	
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
		return NULL;
	}
};

class CharsetNode : public Node
{
	public:
	char *charset;
	CharsetNode(char *charset)
	{
		this->charset = charset;
	}
	
	int *evaluate()
	{
		return NULL;
	}
};

class ImportNode : public Node
{
	public:
	char *import;
	ImportNode(char *import = NULL)
	{
		this->import = import;
	}
	
	int *evaluate()
	{
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
	char *prop;
	Nodes *value;
	bool important;
	
	DeclNode(char *p, Nodes *v, bool i=false)
	{
		this->prop = p;
		this->value = v;
		this->important = i;
	}
	
	int *evaluate()
	{
		return NULL;
	}
};

//List Node
class ListNode : public Node
{
	public:
	Nodes *node_list;
	ListNode(Nodes *n)
	{
		this->node_list = n;
	}
	
	int *evaluate()
	{
		
	}
};

//String Node
class StrNode : public Node
{
	public:
	char *str;
	StrNode(char *s)
	{
		this->str = s;
	}
	
	int *evaluate()
	{
		return NULL;
	}
};

//Identifier Node
class IdNode : public Node
{
	public:
	char *id;
	IdNode(char *i)
	{
		this->id = i;
	}
	
	int *evaluate()
	{
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
	char *name;
	Node *exp;
	FuncNode(char *n, Node *e)
	{
		this->name = n;
		this->exp = e;
	}
	
	int *evaluate()
	{
		return NULL;
	}
};

/*
	Function Node
	Example:
		:hover | :nth-of-type(even)
*/
class PseudoBlockNode : public Node
{
	public:
	char *ident = NULL;
	Node *function = NULL;
	
	PseudoBlockNode(char *i)
	{
		this->ident = i;
	}
	
	PseudoBlockNode(Node *f)
	{
		this->function = f;
	}
	
	int *evaluate()
	{
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
	char *name;
	char type;
	
	SelectorNode(char *n, char t = 't')
	{
		this->name = n;
		this->type = t;
	}
	
	int *evaluate()
	{
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
	char *ident;
	Node *value;
	
	AttrSelectNode(char *i, Node *v = NULL)
	{
		this->ident = i;
		this->value = v;
	}
	
	int *evaluate()
	{
		return NULL;
	}
};

//URI Node
class UriNode : public Node
{
	public:
	char *uri;
	UriNode(char *u)
	{
		this->uri = u;
	}
	
	int *evaluate()
	{
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
		return NULL;
	}
};

//Dimension Node
class DimNode : public Node
{
	public:
	double dim;
	char *type;
	DimNode(double d, char *t)
	{
		this->dim = d;
		this->type = t;
	}
	
	int *evaluate()
	{
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
	char *hash;
	HashNode(char *h)
	{
		this->hash = h;
	}
	
	int *evaluate()
	{
		return NULL;
	}
};