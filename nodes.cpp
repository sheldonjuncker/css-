/*
	Nodes for TCSS
*/

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
		return NULL;
	}
};