#include"CMatrix.h"
#include"CParser.h"



void CParser::take_input()
{
	while(1)
	{
		 CMatrix::print=1;	//this is the static variable of the class CMatrix::print
		int open_brakets=0,closed_brakets=0,equal_sign=0;
		string input="";
		int activate_enter=0;
		do
		{
			
			string h;
			getline(cin,h);
			if(activate_enter)
			{
				h+=13;
			}
			if(h.find('[')!=-1 &&(!activate_enter))
			{
				activate_enter=1;
				h+=13;
			}
			input+=h;   //should fix the newline problem

			for(int i=0;i<h.length();i++)
				{
					if(h[i]=='(' || h[i]=='[') open_brakets++;
					else if(h[i]==')' || h[i]== ']') closed_brakets++;
					else if(h[i]=='=') equal_sign++;
				}
		}
		while(open_brakets != closed_brakets); 

		//testing the input
		//cout<<input<<endl;

		//remove spaces from the input

		//storing the names of variables or matrices

		if(input[input.length()-1]==';') CMatrix::print=0;

		if(equal_sign==0) 
		{
			//so it's not an operation it's just printing the value of the operation in a temp value named as ans
			//first check if there's an operation to make before printing the value
			int op_found=input.find_first_of("+-/*.^()");
			if(op_found==string::npos)
			{
				int variable_found=0;
				//print the matrix or the variable and input= its name
				for(int i=0;i<mats.size();i++)
				{
					if(mats[i].name==input)
					{
						mats[i].print_mat();
						variable_found=1;
						break;
					}
				}
				if(!variable_found)
				{
					for(int i=0;i<vars.size();i++)
					{
						if(vars[i].name==input)
						{
							vars[i].print_var();
							variable_found=1;
							break;
						}
					}
				}
				if(variable_found==0)          // error handling
				{
					cout<<"Unidentified Variable"<<endl;
				}
			}
			else
			{
				//do the operation and print the value under the name "ans"
				int matrix_operation=0;
				for(int i=0;i<mats.size();i++)
				{
					if(input.find(mats[i].name)!=-1)    //if any matrix was in this line so it's a matrix operation
					{
						matrix_operation=1;
						CMatrix a=CMatrix::calculate_expression(input);
						string mat_name="ans";
						a.name=mat_name;
						mats.push_back(a);
						mats[mats.size()-1].print_mat();
						break;
					}
				}
				if(!matrix_operation)
				{
					string var_name="ans";
					CVariables a(calculate(input , First),var_name);
					vars.push_back(a);
					vars[vars.size()-1].print_var();
				}
			}

		}
		else if(input.find("rand")!= -1 || input.find("eye")!= -1 || input.find("zeros")!= -1 || input.find("ones")!= -1) 
		{
			// so it's a special  matrix definition
			string mat_name="";
				for(int i=0;input[i]!='=';i++)
				{
					mat_name+=input[i];
				}
			int nR=atof(input.substr(input.find("(")+1,input.find(",")-input.find("(")-1).c_str());
			int nC=atof(input.substr(input.find(",")+1,input.find(")")-input.find(",")-1).c_str());
			if(input.find("rand")!=-1)
			{
				//make a random matrix
				//use the constructor by giving it nR,nC and the random mode
				mats.push_back(CMatrix(nR,nC,3,mat_name));
				mats[mats.size()-1].print_mat();
			}
			if(input.find("eye")!=-1)
			{
				//make a unity matrix
				//use the constructor by giving it nR,nC and the unity mode
				mats.push_back(CMatrix(nR,nC,4,mat_name));
				mats[mats.size()-1].print_mat();
			}
			if(input.find("zeros")!=-1)
			{
				//make a zero matrix
				//use the constructor by giving it nR,nC and the zero mode
				mats.push_back(CMatrix(nR,nC,1,mat_name));
				mats[mats.size()-1].print_mat();
			}
			if(input.find("ones")!=-1)
			{
				//make a one matrix
				//use the constructor by giving it nR,nC and the one mode
				mats.push_back(CMatrix(nR,nC,2,mat_name));
				mats[mats.size()-1].print_mat();
			}

		}
		else if(input.find("[")!=-1)
		{
			string mat_name="";
				for(int i=0;input[i]!='=';i++)
				{
					mat_name+=input[i];
				}
			// it's a matrix definition
			int begin=input.find("[");
			int end=input.rfind("]");
			string mat_string=create_mat(input.substr(begin+1,end-begin-1),1);
			mats.push_back(CMatrix(mat_string,mat_name));
			mats[mats.size()-1].print_mat();
		}
		else
		{
			//it's a const or matrix operation
			//if no operations exist so it should be CVariable as it contains no []
			//if there are operations check the operands 
			//if operands are CMatrices--> it's a matrix
			//else --> it's a variable

			int op_found=input.find_first_of("+-*/^()",input.rfind('=')+2);//skipping the first character (A=-13.5) not an operation
			if(op_found==string::npos)
			{
				string var_name="";
				int i;
				for(i=0;input[i]!='=';i++)
				{
					var_name+=input[i];
				}
				double var_value=atof(input.substr(i+1).c_str());
					vars.push_back(CVariables(var_value,var_name));
					vars[vars.size()-1].print_var();
			}
			else 
			{
				int matrix_operation=0;
				for(int i=0;i<mats.size();i++)
				{
					if(input.find(mats[i].name)!=-1)    //if any matrix was in this line so it's a matrix operation
					{
						matrix_operation=1;
						string mat_to_be_calculated=input.substr(input.find('=')+1);
						CMatrix a=CMatrix::calculate_expression(mat_to_be_calculated);
						string mat_name="";
						for(int i=0;input[i]!='=';i++)
						{
							mat_name+=input[i];
						}
						a.name=mat_name;
						mats.push_back(a);
						mats[mats.size()-1].print_mat();
						break;
					}
				}
				if(!matrix_operation)
				{
					string var_name="";
					string var_to_be_calculated=input.substr(input.find('=')+1);
					for(int i=0;input[i]!='=';i++)
						{
							var_name+=input[i];
						}
					CVariables a(calculate(var_to_be_calculated , First),var_name);
					vars.push_back(a);
					vars[vars.size()-1].print_var();
				}
			}
		}
	}
}


string CParser::concat(CMatrix A,CMatrix B)
{
	string ret="";
	for(int i=0;i<A.nrows;i++)
	{
		for(int j=0;j<(A.ncols+B.ncols);j++)
		{
			if(j<A.ncols) ret+=to_string(A.pp_rows[i][j]);
			else ret+=to_string(B.pp_rows[i][j-A.ncols]);

			if(j!=(A.ncols+B.ncols-1)) ret+=" ";
		}
		if(i!=(A.nrows-1)) ret+=";";
	}
	//cout<<ret<<endl;
	return ret;
}



string CParser::create_mat(string h,int mode)
{
	static int temp_counter=0;
    //searching for CVariables
    //only in the first time
    if(mode==1)
    {
        for(int i=0;i<vars.size();i++)
        {
            int pos=h.find(vars[i].name);
            if(pos != -1)
                {h.replace(pos,vars[i].name.length(),to_string(vars[i].value)); i--;} //if it's repeated so find it again
            }
    }
    //searching for [ ]
  /*  while(h.find("[")!=-1)
    {
        int open_braket_pos=h.find("[");
        int closed_braket_pos=h.find("]");
        string mat_name=create_mat(h.substr(open_braket_pos+1,closed_braket_pos-open_braket_pos-1),2);
        h.replace(open_braket_pos,closed_braket_pos-open_braket_pos,mat_name);
        }*/
	int brakets=0,flag=0,open_braket_pos,closed_braket_pos,smallest_closed=0,first_open=0,first_open_pos;
	for(int i=0;i<=h.length();i++)
	{
		if(h[i]=='[') 
		{
			brakets++; 
			flag=1; 
			open_braket_pos=i;
			if(first_open==0)
			{
				first_open=1;
				first_open_pos=i;
			}
		}
		else if(h[i]==']') 
		{
			brakets--; 
			if(smallest_closed==0)
			{
				closed_braket_pos=i;
				smallest_closed=1;
				brakets=0;
			}
		}

		if(flag==1 && brakets==0)
		{
			string mat_name=create_mat(h.substr(open_braket_pos+1,closed_braket_pos-open_braket_pos-1),2);
			h.replace(open_braket_pos,closed_braket_pos-open_braket_pos+1,mat_name);
		//	i=open_braket_pos+1;
			i=first_open_pos-1;
			smallest_closed=0;
			first_open=0;
			flag=0;
		}
	}
   
    //here no [ ] exists only elements or matrices
    //searching for matrices
	for(int i=0;i<mats.size();i++)
	{
		int pos=h.find(mats[i].name);
		if(pos!=-1)
		{
			if( (pos==0 ||h[pos-1]==';' ) && (pos==h.length()-mats[i].name.length() ||h[pos+mats[i].name.length()]==';') )
			{
				h.replace(pos,mats[i].name.length(),concat(mats[i],CMatrix(0,0)));   //edit
			}
			else if((pos==0 ||h[pos-1]==';') && (h[pos+mats[i].name.length()]==' '||h[pos+mats[i].name.length()]==','))
			{
				int second_mat_pos=h.find_first_of(" ;",pos+mats[i].name.length()+1);
				string second_mat=h.substr(pos+mats[i].name.length()+1,second_mat_pos-pos-mats[i].name.length()-1);
				for(int j=0;j<mats.size();j++)
				{
					if(second_mat==mats[j].name)
					{
						h.replace(pos,mats[i].name.length()+mats[j].name.length()+1,concat(mats[i],mats[j]));
						i--;
						break;
					}
				}
			}
			i--; //added to find the same matrix again
		}
	}
    


	if(mode==2)
	{
		mats.push_back(CMatrix(h,"#"+to_string(temp_counter++)));
		return mats[mats.size()-1].name;
	}
	else return h;

}


string CParser::handle_parentheses(string& s, char c )
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


string CParser::handle_priorities_2(string &s, string*op, int n_op, int continuee , char cc )
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



string CParser:: handle_priorities(string  &s)
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

string CParser::detect_operan(string &s)
{
string m;

if (s.find_first_not_of("0123456789.") == string::npos)
return "const";
//for matrix
else if ((s.find('[') != string::npos)||(s.find("rand") != string::npos)||(s.find("eye") !=
string::npos)||(s.find("ones") != string::npos)||(s.find("zeros") != string::npos))
return "matrix";
else
return "temp";
}

 void CParser::remove_spaces(string&str)
 {

	 int i=0, len, j,toll;
	 bool no3= true ;
	len=str.length();
	toll=str.length();
	string s= detect_operan(str) ;
 	if(s=="matrix")
		no3=false ;
	
	int c=0 ;
	for(i=0;i<len;i++)
	{
	if(str[0]==' ')
		{
			c++ ;
			str.erase(0,1) ;
		   len--;
		}
	}
	for(i=0; i<len; i++)
	{
		if(str[i]=='('&& s=="matrix")
		no3=true;
		if(str[i]==')'&& s=="matrix")
			no3=false ;
		if(str[i]==' '&&str[i+1]==' ')
		{
			
			c++ ;
			for(j=i; j<len; j++)
			{
				str[j]=str[j+1];
			}
		   len--;
		    i--;
		}
		
		if(str[i]==' '&&((str[i+1]=='s'&&no3==true)||(str[i+1]=='c'&&no3==true)||str[i+1]=='+'||str[i+1]=='*'||str[i+1]=='/'||str[i+1]=='l'
			||str[i-1]=='+'||str[i+1]=='='||str[i-1]=='='||str[i-1]=='n'||str[i-1]=='c'||str[i-1]=='*'||str[i-1]=='/'||str[i-1]=='l'||(no3==true&&(str[i+1]=='-'||str[i+1]=='('||str[i-1]==')'))
			||str[i-1]=='-'||str[i-1]=='('||str[i+1]==')'||str[i+1]=='.'||str[i+1]=='e'||str[i+1]=='t'||str[i+1]=='^'
			||str[i-1]=='^'||str[i-1]=='g'))
		{
			c++ ;
			for(j=i; j<len; j++)
			{
				str[j]=str[j+1];
			}
		len--;
		}
		 
	}
	str=str.erase(toll-c,toll) ;
 }


 /*


 float CParser::domath(string&a)
{
	string n1 = ""; float result=0;
	unsigned int length = a.length();
	 int no_of_char = 0;
	  string last="";
	vector <float> v;
	vector<char> signs;
	string function="";
	while(no_of_char < length)
	{
		if( CMatrix::checkchar(a[no_of_char]) ||(a[no_of_char]=='-' && n1==""&&last!=")") )
		{
			if(last=="^") 
			{
				while( (CMatrix::checkchar(a[no_of_char]))||(a[no_of_char]=='-'&&n1==""))
				{
					n1+=a[no_of_char];
					no_of_char++;
				}
			v.push_back(pow(result , atof( n1.c_str())));
			last = ")";
			n1="";
			}
			else
			{
			n1+=a[no_of_char];
			no_of_char++;
			last="!";
			}
		}

		else if (a[no_of_char]=='^')
		{
			result = atoi (n1.c_str());
			last = "^";
			n1="";
			no_of_char++;
		}
		else if(a[no_of_char]=='+' ||(a[no_of_char]=='-')||a[no_of_char]=='*'||a[no_of_char]=='/')
		{
			
			if(n1!="")
			{
				result = atof(n1.c_str());
				v.push_back(result);
				result = 0;
				n1="";
			}
			signs.push_back(a[no_of_char]);
			no_of_char++;
			last = "$";
		}
		else if( (a[no_of_char] == 'l'&&a[no_of_char+1]=='o'&&a[no_of_char+2]=='g')||(a[no_of_char] == 's'&&a[no_of_char+1]=='i'&&a[no_of_char+2]=='n')
			||(a[no_of_char] == 'c'&&a[no_of_char+1]=='o'&&a[no_of_char+2]=='s')||(a[no_of_char] == 't'&&a[no_of_char+1]=='a'&&a[no_of_char+2]=='n')||
			(a[no_of_char] == 's'&&a[no_of_char+1]=='e'&&a[no_of_char+2]=='c')||(a[no_of_char] == 'c'&&a[no_of_char+1]=='s'&&a[no_of_char+2]=='c') ||
			a[no_of_char] == 'e'&&a[no_of_char+1]=='x'&&a[no_of_char+2]=='p')
		{
			bool flag=0;
			string function="";
			function += a[no_of_char];
			function+=a[no_of_char+1];
			function+=a[no_of_char+2];
			no_of_char = no_of_char +3;
			if(n1!="")
			{
				result = atof(n1.c_str());
				v.push_back(result);
				result = 0;n1="";
			}
			while((CMatrix::checkchar(a[no_of_char])||(a[no_of_char]=='-'&&n1==""))&&no_of_char < length)
					{
						n1+=a[no_of_char];
						no_of_char++;
					}
			result = atof(n1.c_str());
			n1="";
			if(function=="sin") v.push_back(sin(result));
			if(function=="cos")v.push_back(cos(result));
			if(function=="tan")v.push_back(tan(result));
			if(function=="log"){if (result<=0 ) throw(0); else {v.push_back(log10(result));}}
			if(function=="sec")v.push_back(1.0/cos(result));
			if(function=="csc"){ if (result == 1.5707 ) throw (2) ; else {v.push_back(1.0/cos(result));}}
			if(function=="exp")v.push_back(exp(result));
			result =0;
			last=")";

		}
		else
			no_of_char++;
	}
l1:	if(n1 != "")
	{
		float ni=stof(n1);
		v.push_back(ni);
	}

	return vector_cal ( v , signs );
}

*/

float CParser::vector_cal ( vector <float> v , vector<char>signs )
{
	float result = 0;
	int size = v.size();
	bool flag = false;int i,j;
	float *p=new float[size];
	if(size==1)
	{
		result = v[0];
		return result;
	}
	for(int r =0;r<size;r++)
	{
		p[r] = v[r];
	}

	char *sign = new char[signs.size()];
	for(int q =0;q<signs.size();q++)
	{
		sign[q] = signs[q];
	}
	float total;
	while(size > 1)
	{
		for(i=0;i<size-1;i++)
		{
			if(sign[i]=='*' || sign[i]=='/')
			{
				flag = true;
				break;
			}
		}
		if(flag == true)
		{
			flag = false;
			size--;
			float *temp = new float[size];
			for(j=0;j<size;j++)
			{
				if(j==i && sign[i] == '*')
				{temp[j] = p[j]*p[j+1];if(size==1) return temp[j];}
				else if(j==i && sign[i] == '/')
				{  if(p[j+1]==0) throw(4); else { temp[j] = p[j]/p[j+1];if(size==1) return temp[j];} }
				else if ( j<i)
					temp[j] = p[j];
				else
					temp[j] = p[j+1];
			}
			p=temp;
			char*temps = new char [size-1];
			for(j=0;j<size-1;j++)
			{
				if(j<i)
					temps[j] = sign[j];
				else
					temps[j]=sign[j+1];
			}
			sign = temps;
		}
		else
		{
			size--;
			float *temp = new float[size];
			if(sign[0] == '-') {temp[0] = p[0]-p[1];result = temp[0];}
			if(sign[0] == '+') {temp[0] = p[0]+p[1];result = temp[0];}
			if(size>1)
			{
				for(j=1;j<size;j++)
				{
					temp[j] = p[j+1];
				}
				p=temp;
			}
			if(size-1 >=1)
			{
				char*temps = new char [size-1];
				for(j=0;j<size-1;j++)
				{
					temps[j]=sign[j+1];
				}
				sign = temps;
			}
		}
	}
	return result;
}



float CParser:: subcal(string h,int nb)
{
	//cout<<nb<<endl;
	if(nb==0)
		return calculate(h , Other); // modified instead of domath(h)
	else
	{
		float result;
		string part="";
		int *pos=new int[nb];
		int length=h.length();
		int d=0;int i;int count=0;
		for(i=0;i<length;i++)
		{
			i=h.find('(',i);
			if(i==-1)break;
			pos[d]=i;
			cout<<pos[d]<<endl;
			d++;count++;
		}
		d=0;i=0;bool flag=true;
		for(i=0;i<count;i++)
		{
			d=pos[i];
			d++;
			while(d<length)
			{
				if(h[d]=='(')
				{part="";break;}
				else if(h[d]==')')
				{
					flag=false;
					break;
				}
				else part+=h[d];
				d++;
			}
			if(flag==false)
			{
				flag=true;
				break;
			}
		}
		h=h.replace(pos[i],d-pos[i]+1,to_string(calculate(part , Other)));
		//cout<<"my favourite part is "<<h<<endl;
		if(h.find('(',0)==-1)
			return calculate(h , Other);   //modified instead of domath(h)
		else
		{
			int l=0, count2=0;
			while(l<h.length())
			{
				if(h.find('(',l)!=-1)
				{
					count2++;
					l=h.find('(',l)+1;
					cout<<l<<endl;
				}
				else break;
			}
			 result = subcal(h,count2);
		}
		return result;
	}
}









float CParser::calculate(string a , enum NoCalling detector)
{
	string w="";int no_of_char=0;string last;string mod;unsigned int coo=0;
	int length = a.length();vector<float> v;vector<char>signs;float result=0;
	while(no_of_char<length)
	{
		if(CMatrix::checkchar(a[no_of_char]) || (a[no_of_char]=='-' &&w==""&&last!=")") )
		{
			w+=a[no_of_char];
			if(last!="^")last = "!";
			else last="^";
			if(no_of_char+1 == length&&last=="^") { v.push_back(pow(result,atof(w.c_str())));w="";result=0;}
			no_of_char++;
		}
		else if(a[no_of_char]=='+' ||(a[no_of_char]=='-')||a[no_of_char]=='*'||a[no_of_char]=='/')
		{
			if(result!=0&&last==")")
			{
				v.push_back(result);
				result=0;
				coo=0;
			}
			if(last=="^" && w!="")
			{
				v.push_back(pow(result,atof(w.c_str())));
				w="";
				result=0;
			}
			if(w!="")
			{
			result=atof(w.c_str());
			v.push_back(result);
			result=0;w="";
			}
			signs.push_back(a[no_of_char]);
			no_of_char++;
			last="$";
		}


		else if(a[no_of_char]=='^')
		{
			if(w!="")
			{
				result = atof(w.c_str());
				w="";
			}
			no_of_char++;
			last="^";
		}

		else if ( a[no_of_char]=='.' && a[no_of_char+1]=='^')
		{
			if(w!="")
			{
				result = atof(w.c_str());
				w="";
			}
			no_of_char=no_of_char+2;
			last="^";
		}

		else if( (a[no_of_char] == 'l'&&a[no_of_char+1]=='o'&&a[no_of_char+2]=='g')||(a[no_of_char] == 's'&&a[no_of_char+1]=='i'&&a[no_of_char+2]=='n')
			||(a[no_of_char] == 'c'&&a[no_of_char+1]=='o'&&a[no_of_char+2]=='s')||(a[no_of_char] == 't'&&a[no_of_char+1]=='a'&&a[no_of_char+2]=='n')||
			(a[no_of_char] == 's'&&a[no_of_char+1]=='e'&&a[no_of_char+2]=='c')||(a[no_of_char] == 'c'&&a[no_of_char+1]=='s'&&a[no_of_char+2]=='c') 
			||(a[no_of_char] == 'e'&&a[no_of_char+1]=='x'&&a[no_of_char+2]=='p')||(a[no_of_char]=='l'&&a[no_of_char+1]=='n'))
		{

			if ( detector == 0)

			{

			if(result!=0&&last==")")
			{
				v.push_back(result);
				result=0;
			}
			string function="";
			if(a[no_of_char]=='l'&&a[no_of_char+1]=='n') {function = "ln" ; }
			else
			{
				function+=a[no_of_char];
				function+=a[no_of_char+1];
				function+=a[no_of_char+2];
			}
			no_of_char=a.find('(',no_of_char);
			if(w!="")
			{
				v.push_back(atof(w.c_str()));
				w="";
				signs.push_back('*');
				last= function;
			}
			int beginning = no_of_char;
			w+=a[no_of_char];
			no_of_char++;
			int noopen=1;
			int noclosed=0 ;
			bool is_error = true;
			while(no_of_char < a.length())
			{
				w+=a[no_of_char];
				if(a[no_of_char]=='(')noopen++;
				if(a[no_of_char]==')')noclosed++;
				if(noopen==noclosed) {is_error=false;break;}
				else 
				{
					no_of_char++;
					continue;
				}
			}
			if(is_error) throw(3) ;
			int end = no_of_char;
			w=w.erase(0,1);w=w.erase(w.length()-1,1);
			float res = subcal(w,noopen-1);
			w="";
			if(function=="sin") v.push_back(sin(res));
			if(function=="cos")v.push_back(cos(res));
			if(function=="tan") if(res==1.5708) throw(0) ; else v.push_back(tan(res));
			if(function=="log"){ if (res<=0 ) throw(1);else { v.push_back(log10(res));}}
			if(function=="sec")v.push_back(1.0/cos(res));
			if(function=="csc")v.push_back(1.0/sin(res));
			if(function=="exp")v.push_back(exp(res));
			if(function=="ln" ) if(res<=0) throw(4) ; else v.push_back(log10(res));
			result =0;
			last = ")";
			no_of_char=end+1;
		}
		else
		{
			bool flag=0;
			string function="";
			function += a[no_of_char];
			function+=a[no_of_char+1];
			function+=a[no_of_char+2];
			no_of_char = no_of_char +3;
			while((CMatrix::checkchar(a[no_of_char])||(a[no_of_char]=='-'&&w==""))&&no_of_char < length)
					{
						w+=a[no_of_char];
						no_of_char++;
					}
			result = atof(w.c_str());
			w="";
			if(function=="sin") v.push_back(sin(result));
			if(function=="cos")v.push_back(cos(result));
			if(function=="tan")v.push_back(tan(result));
			if(function=="log"){if (result<=0 ) throw(0); else {v.push_back(log10(result));}}
			if(function=="sec")v.push_back(1.0/cos(result));
			if(function=="csc")v.push_back(1.0/sin(result));
			if(function=="exp")v.push_back(exp(result));
			result =0;
			last=")";
		}
		}

		else if ( a[no_of_char]=='s' && a[no_of_char+1]=='q')
		{
			no_of_char=a.find('(',no_of_char);
			int beginning = no_of_char;
			w+=a[no_of_char];
			no_of_char++;
			int noopen=1;
			int noclosed=0 ;
			bool is_error = true;
			while(no_of_char < a.length())
			{
				w+=a[no_of_char];
				if(a[no_of_char]=='(')noopen++;
				if(a[no_of_char]==')')noclosed++;
				if(noopen==noclosed){is_error = false;break;}
				else 
				{
					no_of_char++;
					continue;
				}
			}
			if(is_error) throw(3);
			int end = no_of_char;
			w=w.erase(0,1);w=w.erase(w.length()-1,1);
			float res = subcal(w,noopen-1);
			w="";
			if (res < 0) throw (2) ;
			else v.push_back(sqrt(res));
			result =0;
			last = ")";
			no_of_char=end+1;
		}

		else if(a[no_of_char]=='(')
		{
			coo++;
			if(w!="")
			{
				result = atof(w.c_str());
				v.push_back(result);
				signs.push_back('*');
				result=0;last="(";w="";
			}
			w+=a[no_of_char];
			int beginning = no_of_char;
			no_of_char++;
			int noopen=1;
			int noclosed=0 ;
			bool is_error = true ;
			while(no_of_char < a.length())
			{
				w+=a[no_of_char];
				if(a[no_of_char]=='(')noopen++;
				if(a[no_of_char]==')')noclosed++;
				if(noopen==noclosed) {is_error= false; break;}
				else 
				{
					no_of_char++;
					continue;
				}
			}
			if(is_error) throw(3) ; 
			int end = no_of_char;
			cout<<noclosed<<"       "<<noopen<<" "<<w<<endl;
			w=w.erase(0,1);w=w.erase(w.length()-1,1);
			if(last!="^")
			{
				if(coo==1) result=subcal(w,noopen-1);
				if(coo==2) {if(result!=0){v.push_back(result);}result=subcal(w,noopen-1);v.push_back(result);coo=0;}
			}
			if(last=="^")
			{
				float e = subcal(w,noopen-1);
				v.push_back( pow( result,e ));
				coo=0;result = 0;
			}
			w="";
			last=")";
			no_of_char=end+1;
		}
		else
		{
			no_of_char++;
			continue;
		}
	}
	if(result!=0)
	{
		v.push_back(result);
	}

	result=0;
	if(w != "")
	{
		float ni=stof(w);
		v.push_back(ni);
	}
	return vector_cal ( v , signs );
	}
