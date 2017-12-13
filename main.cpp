#include"CMatrix.h"
#include <fstream>


 void create_matrix(string&s) ;
 int check(string);
 void dop(string&s);
 void  detect_instruction(string &s);


int main(int argc,char*argv[])
{
	 CMatrix::print=1;

/*if (argc == 1)
	{
		string h;
		while (getline(cin,h))
		{
			int equal=h.find("=");
			int pos_open_square=h.find("[");
			int pos_close_square=h.find("]");
			if(equal== -1 ) {continue;}
			if(pos_open_square!=-1 && pos_close_square== -1)
			{
				string x;
				while(getline(cin,x))
				{
					h=h+x;
					if(x.find(']')!=-1) {break;}
				}
			}
			if(h[h.length()-1]==';')  {CMatrix::print=0;}
			else CMatrix::print=1;
			detect_instruction(h);
		}
	}
	else if (argc == 2)
	{
		string s;
		ifstream file(argv[1]);
		while (getline(file, s))
		{
			int equal=s.find("=");
			int pos_open_square=s.find("[");
			int pos_close_square=s.find("]");
			if(equal== -1 ) {/*cin.ignore()*//*; continue;}
			if(pos_open_square!=-1 && pos_close_square== -1)
			{
				string x;
				while(getline(file,x))
				{

					s+=x;
					if(x.find(']')!=-1) {break;}
				}
			}
			if(s[s.length()-1]==';')  {CMatrix::print=0;}
			else CMatrix::print=1;
			detect_instruction(s);
		}
	}
	*/
	 try
	 {
	string h="a=[1 2 ; 3 4]";
	detect_instruction(h);
	 h="b=[1 2 ; 5 6]";
	 detect_instruction(h);
	 h="c=a+b";
	 detect_instruction(h);
	 string f [] = {"a"  , ".*" , "2" , "$$" , "#0" , "+" , "a" };
	 CMatrix res = calculatemat(f , 7);
	 res.print_matrix("f");
	 string j = "1-2*log(-4)/2";
	 cout<<calculate(j);
	 }
	 catch (int n)
	 {
		 if(n == 0){ cout<<"Cant calculate log -ve";}
		 if(n==1) {cout<"<Cant calculate sqrt -ve";}
	 }

	return 0;
}
