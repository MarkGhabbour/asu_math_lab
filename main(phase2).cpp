#include"CMatrix.h"
#include"CParser.h"


int main(int argc,char** argv)
{
L1:	try{
		if(argc==1)
			CParser::take_input();
		else if(argc==2)
			CParser::take_input_file(argv[1]);

	}
	catch(const char* error)
	{
		cout<<error<<" at line "<<line<<endl;
		goto L1;
	}

	catch(int n)
  	{		 
		
		if(n==0) cout<<"error : "<<"Can't calculate log(0) or log(-ve)"<<endl;
		if(n==1) cout<<"error : "<<"Can't calculate tan(90)"<<endl;
		if(n==2) cout<<"error : "<<"sqrt(-ve) is an imaginary number"<<endl;
		if(n==3) cout<<"error : "<<"Syntax error"<<endl;
		if(n==4) cout<<"error : "<<"Can't calculate ln(0) or ln(-ve)"<<endl;
		if(n==5) cout<<"error : "<<"Can't divide by 0 "<<endl;
 	  }


	return 0;
}