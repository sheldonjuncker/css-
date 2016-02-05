/*
	The Value Class for TCSS
*/

//Value Definition
class Value;

//Dimension Type
struct Dim
{
	double value;
	std::string dim; //px, em, %, etc.
	
	Dim(double v = 0, std::string d = "")
	{
		value = v;
		dim = d;
	}
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
	public: 
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
	
	//Dimension Constructor
	Value(Dim *d)
	{
		this->t = DIM;
		this->v.dim = d;
	}
	
	/*
		Member Functions
	*/
	
	/*
		Operators:
		+
		-
		*
		/
	*/
	
	//Addition
	Value *add(Value *v1)
	{
		//Integers
		if(this->t == INT && v1->t == INT)
		{
			return new Value(v.i + v1->v.i);
		}
		
		//Doubles
		else if(this->t == DUB && v1->t == DUB)
		{
			return new Value(v.d + v1->v.d);
		}
		
		//String
		else if(this->t == STR && v1->t == STR)
		{
			return new Value(*(v.s) + *(v1->v.s));
		}
		
		//Dimension
		else if(this->t == DIM && v1->t == DIM && v.dim->dim == v1->v.dim->dim)
		{
			Dim *dim = new Dim();
			dim->value = this->v.dim->value + v1->v.dim->value;
			dim->dim = this->v.dim->dim;
			return new Value(dim);
		}
		
		//Return NULL
		TCSS::warning("Using operators on invalid data types.");
		return new Value();
	}
	
	//Convert to String
	std::string toString()
	{
		switch(this->t)
		{
			case INT:
				return String(v.i);
			break;
			
			case DUB:
				return String(v.d);
			break;
			
			case STR:
				return *(v.s);
			break;
			
			case NUL:
				return "";
			break;
			
			case DIM:
				return String(v.dim->value) + v.dim->dim;
			break;
			
			case HEX:
				return "#" + *(v.s);
			break;
			
		}
		return "";
	}
};