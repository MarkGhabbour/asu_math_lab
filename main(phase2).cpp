#include"CMatrix.h"
#include"CParser.h"


int main(int argc,char argv[])
{
	try{
		if(argc==1)
			CParser::take_input();
		else if(argc==2)
			CParser::take_input_file(argv);

	}
	catch(char* error)
	{
		cout<<error<<endl;
	}
	return 0;
}