#include"CMatrix.h"
#include "parsing_functions_prototypes.h"
#include <iostream>
#include <string>

using namespace std;



///*****mark_part_starts*****////

//a function to handle_inputs containing multiple parentheses ()
// if an input is  (     (A+B)/ ( C*D./4)    )
//output will be : A+B_C*D./4_$0/$1    ///////// where $0 refers to A+B and $1 refers to C*D./4
//the output of this function should be the input to another function that sorts it according to the priorities of operation
//in the same line.. * has higher priorty than +      ..etc

string handle_parentheses(string& s, char c = '0')
{
	string ss = "";  string o = "";  //ss temp string to get a string between 2 brackets and call urself
                                     // o is a string to accumlate what i have finished
	int start = 0; int end = 0;      // start and end are variables to find '(' and corresponding  ')'

	int i = 0; static int count = -1; //i var to loop on string, //count var to be seen in all recursions to get $1, $2..etc

	if (s.find(')') ==-1)  //if a string does not contain any brackets then return
			return s;

	while (    i<s.length()   ) //loop on the string
	{
		if (s[i] == '(')
		{        //if i find a opening of a bracket
			int j = 0;
			{
				while (j < s.length()) //getting the position of the corresponding ')'
				{
					if (s[j] == '(')
						start++;
					else if (s[j] == ')')
						end++;
					if (start == end && (s[j] == '(' || s[j] == ')'))
						break;
					j++;
				}
				ss = s.substr(i + 1, j - i - 1); //getting the string between these '(' ')'
			}
			o += handle_parentheses(ss , '1') + "_"; 
			
			//calling myself with a string betwwen '('  ')'
			//the whole idea is just about calling yourself with a smaller string between '(' ')'
			//until u find a string that does not contain any brackets then return
			//then replace that bracket with all what is inside it with a variable name $n 
			//dont care about this 1 parameter now, it will be explained later

			count++;
			s.replace(i, j - i + 1, "$" + to_string(count) );
			
			if (s.find('(') == -1)
			{
				if (s.length() < 4) //if s is only $1 after replacement, decrement count
				{
					count--;
				}
				else
				  o += s;
			}
			//if my string contains no brackets then all brackets have been replaced
			// with dollar sign variables, then i will add this to string o
		}		
		i++;
    }
	if (c == '0')
	{     
		count = 0;
		i = o.length() - 1;

		while (o[i] == '_')
		{
			o.erase(i,1);
			i--;
		}

	}
	//c is a dummy variable, if c='0' that means that i will go to main after return o
	// so i must reset static variable count to zero for next time using this function
	//if the last character of the string to be returned to main=='_' then remove this underscore
	 
	return o;
}

//a function to handle priorities in a string, it takes a string and an array containing operators having the same priority
//for example the array can be * , / ,%..... another array can be sin, cos, tan, log
//if the input string is A*B+C/3 the output will be A*B_C/3_


string handle_priorities_2(string &s, string*op, int n_op, int continuee = 1, char cc = '0')
{
	//the whole idea of the function is taking a string that contain many operations(A*B+C/3), and taking an array of strings
	//that contain the operations that have the same priority like *,/ then finding which of them occurs first
	//then store the operation and its operands in string q, then replacing the original string with a hash and an index
	//then calling yourself again with the new string, and accumlating strings in string o

	int  j = 0, k = 0, l, pos = -1, start = 0;
	static int count = 0;
	//j first time counter for operation in array, k to put where you found the operation
	//l to know index of operation in the array that comes first
	//pos to know position of which op will be first
	string o, q;
	if (continuee == 0)
	{
		count = 0;
	}

	for (j = 0; j < n_op; j++)
	{
		//here i look for the first operation appearing in the string, note that the array contains
		//operations with the same priority

		k = s.find(op[j]);
		if ((k < pos && k != -1) || (pos == -1 && k > pos))
		{
			if (k > 0)
			{
				if (op[j].length() == 1 & s[k - 1] == '.')
					continue;//handling the condition for * , .*
			}
			     pos = k;
				 l = j;
			}
		}
	
	j = pos - 1; start = pos;
	if (pos != -1)
	{
		while (j < s.length() && j >= 0)//getting the operands of the operation backwards
		{
			if (s[j] != '*' &  s[j] != '/' &  s[j] != '^' &  s[j] != '+' & s[j] != '-' & s[j] != '\'')
			{ 
				q = s[j] + q; start = j;
			}
			else
				break;
			j--;
		}
		j = pos + op[l].length(); q += op[l];
		//pushing j to right hanside if operation

		while (j < s.length() && j >= 0)//getting the operands of the operation forward
		{
			if( s[j]!='*' &  s[j] != '/' &  s[j] != '^' &  s[j] != '+' & s[j] != '-' & s[j] != '\'' )
			{
				q += s[j];
			}
			else
				break;
			j++;
		}
		if ( q[q.length() - 1] == '.') //3shan lw hwa bya5od mn odamo lw la2a ./ msln hya5od el dot
										//bs fe nfs el wa2t lw da raqam f momkn ykon feh decimal
			q.erase(q.length()-1,1);

		o += q + "_";
		//replacing the operation and its operands with a hash after adding it to o
		
		s.replace(start, q.length() , "#" + to_string(count));
		count++;
		o += handle_priorities_2(s, op, n_op, 1, '1') + "_";
	}
	if (cc == '0')
	{    //cc is a dummy variable to know that u are in the last recursion and will return to main
		//this section only deletes unnecessary underscores of the string

		j = 0; 
		if (o.length() == 0)
		return o;
		while (j < o.length() - 1)
		{
		    if (o[j] == '_' & o[j + 1]=='_')
			{
				o.erase(j, 1); j--;
			}
			  j++;
		}
		
	}
	return o;
}



string  handle_priorities(string  &s)
{
	//just creating arrays containing the operations to be used
	//op0 operators have higher priority than op1 operators..
	string op0[7];
	op0[0] = "sin"; op0[1] = "cos"; op0[2] = "log"; op0[3] = "ln"; op0[4] = "sqrt";
	op0[5] = "^"; op0[6] = "'";
	string op1[3];
	op1[0] = "*"; op1[1] = "/"; op1[2] = "%";
	string op2[2];
	op2[0] = "+"; op2[1] = "-";
	string op3[4];
	op3[0] = ".^"; op3[3] = ".*"; op3[1] = "./"; op3[2] = ".%";
	string op4[2];
	op4[0] = ".+"; op4[1] = ".-";

	//there may be a bug here, because priority of + is higher than .+ operations
	//if we have an example1: A./3+4 (it will be treated as A./(3+4) bec, i divided A by 3, the matrix result has no operation on a 
	//const just by using + operator. matrix+const is invalid).. if we have an operation like A./3+4./B (it will be treated as (A./7)./B)
	//one may think that we should divide A by 3, 4 divide B, then add the 2 matrices, the conflict appears because
	//the operators not having same order of priorities in the 2 examples
	//i chose to treat it as ( (A./7)./B) bec it is still a valid operation (one can do A./B where a and b are matrices)
	//but if i chose dot '.' operations to be higher in priority than + operation, example one will not be a valid operation
	//if the user just wanted (A./3)+(4./B) he must use brackets
	//it is so tedious to handle the case of changing priorities depending on the different combinations of the operands!

	
	int i = 0, k = 0, start = 0; string q ,o, temp;
	s += '_';
	//splitting a part of the string using '_', handing this part to handle_priorities_2, which operates on the priorities
	//accumlating in o to be returned
	while (s.find('_', start) != -1)
	{
		k = s.find('_', start);
		q = s.substr(start, k - start);
		start = k + 1;
		temp= handle_priorities_2(q, op0, sizeof(op0)/sizeof(op0[0]), 0);
		temp+= handle_priorities_2(q, op1, sizeof(op1) / sizeof(op1[0]), 1);
		temp+= handle_priorities_2(q, op2, sizeof(op2) / sizeof(op2[0]), 1);
		temp+= handle_priorities_2(q, op3, sizeof(op3) / sizeof(op3[0]), 1);
		temp += handle_priorities_2(q, op4, sizeof(op4) / sizeof(op4[0]), 1);
		o += temp;
		if (temp.length() != 0)
			o += "!";
		else
			o += q + "_" + "!";
	}
	

	return o;
}


CMatrix calculate_expression(string s)
{
	//////// cout part /////////////
	//cout << endl << "  my_string = " << s << endl<<endl;
	
	string my_string_after_parentheses = handle_parentheses(s);
	string my_string_after_priorities = handle_priorities(my_string_after_parentheses);

	/////////// cout part //////////
	//cout << "  my_string_after_parentheses = " << my_string_after_parentheses << endl<<endl;
	//cout << "  my_string_after_priorities  = " << my_string_after_priorities << endl<<endl;

	

	//the part before this line working fine

	string my_operation_as_lines[30];//just an arbitary number for the max 
									 //parsing string after priorities..i.e if i have c*D_#0.+4 
									 //the [0] = c*D, [1]= #0.+4 [2]="$$"

	int counter_arr_myop_as_lines = 0; int start = 0;

   

	for (int j = 0; j < my_string_after_priorities.length(); j++)
	{
		if (my_string_after_priorities[j] == '_')
		{
			my_operation_as_lines[counter_arr_myop_as_lines] = my_string_after_priorities.substr(start, j - start );
			start = j + 1;
			counter_arr_myop_as_lines++;
		}
		else if (my_string_after_priorities[j] == '!')
		{
			my_operation_as_lines[counter_arr_myop_as_lines] = "$$";
			start = j + 1;
			counter_arr_myop_as_lines++;
		}
	}
	my_operation_as_lines[counter_arr_myop_as_lines-1] = "00";

	///////////////   cout part //////////////

	//cout << endl << "  my_operation_as_lines = " << endl;
	//for (int j = 0; j < counter_arr_myop_as_lines; j++)
	//	cout << "  " << my_operation_as_lines[j] << endl;
	//part before this line working fine
	

	string my_operation[100];
	
	//array similar to one we dealt with in dop in phase1, [0]=c ,[1]=*, [2]=D [4]=$$
	//difference is between operation of the same line there is "$$"
	//when the line ends there is "##"
	//at the end of the array at the end of all lines there is "00"

	int  index = 0, pos = 0, my_operation_index = 0;
	string q;

	q = my_operation_as_lines[index];
	
	//index is a variable to loop on an a previous array, it is not very important
	//my_operation_index is a variable used to store in my_operation array, after this loo
	//my_operation_index carries number of elements in my_operation array
	//q is just a temp string to hold the string in my_operation_as_lines[index]
	//pos is integer to store where i find the operators like .+ , / , *

	
	while (index < counter_arr_myop_as_lines)
	{
		//^,*,/,+,-,.^,*,./,.+,.-,sin,cos,log,ln,'

		q = my_operation_as_lines[index];

		if (q == "$$")
		{
			my_operation[my_operation_index-1] = "##";
			
		}
		else if (q == "00")
		{
			my_operation[my_operation_index-1] = "00";
			my_operation_index++;
			break;
		}


		else if (q.find("sin") != -1 || q.find("cos") != -1 || q.find("'") != -1
			|| q.find("log") != -1 || q.find("ln")!=-1 || q.find("sqrt") != -1)
		{
			my_operation[my_operation_index] = q;
			my_operation_index++;
			my_operation[my_operation_index] = "$$";
			my_operation_index++;
		}

		else if ((pos = q.find(".^")) != -1 || (pos = q.find(".*")) != -1 ||
			(pos = q.find("./")) != -1 || (pos = q.find(".+")) != -1 ||
			(pos = q.find(".-")) != -1)
		{
			my_operation[my_operation_index] = q.substr(0, pos);
			my_operation_index++;
			my_operation[my_operation_index] = q.substr(pos, 2); //length of ./
			my_operation_index++;
			my_operation[my_operation_index] = q.substr(pos + 2, q.length() - (pos + 2) + 1 );//pos+2 is the number of char after ./ 
			my_operation_index++;																			//+1 to take the last char
			my_operation[my_operation_index] = "$$";
			my_operation_index++;
			
		}
		else if ((pos = q.find("^")) != -1 || (pos = q.find("*")) != -1 ||
			(pos = q.find("/")) != -1 || (pos = q.find("+")) != -1 ||
			(pos = q.find("-")) != -1)
		{
			my_operation[my_operation_index] = q.substr(0, pos);
			my_operation_index++;
			my_operation[my_operation_index] = q.substr(pos, 1); //length of +
			my_operation_index++;
			my_operation[my_operation_index] = q.substr(pos + 1, q.length() - (pos + 1 ) + 1 );
			my_operation_index++;
			my_operation[my_operation_index] = "$$";
			my_operation_index++;
		}
		else
		{
			my_operation[my_operation_index] = q;
			my_operation_index++;																			
			my_operation[my_operation_index] = "##";
			my_operation_index++;
		}
		
		index++;
	}

	/////////// cout part ////////////////////
	//cout << endl<<"  my operation = "<<endl;
	//for (int j = 0; j < my_operation_index; j++)
	//	cout<<"  "<<my_operation[j] << endl;
	//part before this line is working fine

	//here martin or farOoOoOoga will use the array my_operation to calculate the operations  in a similar way to phase1
	//first 
	//lines are divided in the array by ##, that means if an element in this array is ## this is a new line
	//C*D, #0.+4 //means you will calculate C*D then store it, #0 refers to c*D, then #0.+4 refers to #1 and can be used
	//then there is a new line u know this when you find ##
	//the value of (C*D).+4 should be stored and known as $0
    // my_operation may contain an element as a variable with no operation 
	//say my_operation[1]="D"; then you will only store $1 as the value of D
	
	CMatrix result = calculatemat(my_operation, my_operation_index);




}

///***mark_part_ended***//////
