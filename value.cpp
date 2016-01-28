/*
	The Value Class for TCSS
*/

//Data Types Enum
enum Types
{
	INT,
	DUB,
	STR,
	NUL
};

//Value Union
union Val
{
	char *s;
	int i;
	double d;
	void *p;
};

//Value Class
class Value
{
	//Data Type
	Types t;
	
	//Value
	Val v;
	
	//Default Constructor
	Value()
	{
		this->t = NUL;
		this->v.p = NULL;
	}
	
	//String Constructor
	Value(char *s)
	{
		this->t = STR;
		this->v.s = s;
	}
	
	//Double Constructor
	Value(double d)
	{
		this->t = DUB;
		this->v.d = d;
	}
	
	//Integer Constructor
	Value(int i)
	{
		this->t = INT;
		this->v.i = i;
	}
};