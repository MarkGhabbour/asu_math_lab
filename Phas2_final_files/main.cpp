#include"CMatrix.h"
#include"CParser.h"


int main()
{
	try
	{
		CParser::take_input();
	}
	catch(int n)
	{
		if(n==0) cout<<"Can't calculate log(0) or log(-ve)"<<endl;
		if(n==1) cout<<"Can't calculate tan(90)"<<endl;
		if(n==2) cout<<"sqrt(-ve) is an imaginary number"<<endl;
		if(n==3) cout<<"Braces aren't synchronizied"<<endl;
		if(n==4) cout<<"Can't calculate ln(0) or ln(-ve)"<<endl;
	}
	return 0;
}