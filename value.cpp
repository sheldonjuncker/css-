/*
	The Value Class for TCSS
*/

//Value Definition
class Value;

//Dimension Type
struct Dim
{
	public:
	double value;
	std::string dim; //px, em, %, etc.
};

//Data Types Enum
enum Types
{
	INT,
	DUB,
	STR,
	NUL,
	DIM,
	HEX,
	LIST
};

//Value Union
union Val
{
	std::string *s;
	int i;
	double d;
	void *p;
	Dim *dim;
	std::list<Value *> *l;
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
	Value(std::string s)
	{
		this->t = STR;
		this->v.s = new std::string(s);
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
	
	/*
		Member Functions
	*/
	
	//Convert to String
	std::string toString()
	{
		switch(this->t)
		{
			case INT:
			
			break;
			
			case DUB:
			
			break;
			
			case STR:
				return *(v.s);
			break;
			
			case NUL:
				return "";
			break;
			
			case DIM:
			
			break;
			
			case HEX:
				return "#" + *(v.s);
			break;
			
		}
		return "";
	}
};