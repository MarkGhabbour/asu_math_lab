#ifndef parser_m
#define parser_m


#include"CMatrix.h"
extern vector <CVariables> vars;
extern vector <CMatrix> mats;
extern int line;

enum NoCalling { First , Other };
using namespace std;


class CParser
{
public:

	static void take_input();
	static void take_input_file(char* file_path);
	static void detect_input(string input);
	static string create_mat(string h,int mode=1);
    static string concat(CMatrix A,CMatrix B);
	static string handle_parentheses(string& s, char c = '0');
	static string handle_priorities_2(string &s, string*op, int n_op, int continuee = 1, char cc = '0');
	static string  handle_priorities(string  &s);
	static  void remove_spaces(string&str);
	static string m_remove_spaces(string s);
	static string detect_operan(string &s);
	//static float domath(string&a);
	static double calculate(string a , enum NoCalling detector);
	static double vector_cal ( vector <double> v , vector<char>signs );
	static double subcal(string h,int nb);
//	static float calculate(string a);
};



#endif
