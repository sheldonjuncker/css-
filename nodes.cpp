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