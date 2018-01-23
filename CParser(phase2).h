#ifndef parser_m
#define parser_m

#include"CMatrix.h"

using namespace std;
class CParser
{
public:

	static void take_input();
	static string create_mat(string h,int mode=1);
    static string concat(CMatrix A,CMatrix B);

};


#endif