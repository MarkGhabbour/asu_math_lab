

#include"CMatrix.h"
#include <iostream>
#include <string>

using namespace std;



string handle_parentheses(string& s, char c );
string handle_priorities_2(string &s, string*op, int n_op, int continuee, char cc );
string  handle_priorities(string  &s);
CMatrix calculate_expression(string s);