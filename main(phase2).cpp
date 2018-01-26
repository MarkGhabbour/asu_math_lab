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
		if(n==0) cout<<"Can't calculate log (-ve ) or log(0)"<<endl;
		if(n==1) cout<<"Can't calculate tan(90)"<<endl;
		if(n==2) cout<<"Can't calculate sqrt(-ve)"<<endl;
		if(n==3) cout<<"Numbers of open and closed braces aren't synchronizied"<<endl;
	}
	return 0;
}