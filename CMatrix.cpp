#include"CMatrix.h"
#include<vector>
vector <CMatrix> v ;
vector <string> names;


/////////////********** Phase 2 ********//////////

vector < CVariables > variables;   /////////vector for constants created as L=sin(30) *6 L will be stored here.
float vector_cal ( vector <float> v , vector<char>signs );
int check_for_var(string name_to_check )   /*function to check existance of variable like that of matrix*/
{
	for(int i=0; i < variables.size() ; i++ )
	{
		if(variables[i].name == name_to_check)
			return i;
	}
	return -1;
}



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















/////////////****************************////////////

int CMatrix::print;

CMatrix::CMatrix(int r, int c)     
	{
		nrows = r;
		ncols = c;
		pp_rows = new double*[r];
		for (int i = 0; i<r; i++) pp_rows[i] = new double[c];
		for (int i = 0; i<r; i++)
			for (int j = 0; j<c; j++)
				pp_rows[i][j] = 0;
	}

CMatrix::CMatrix()
	{
		nrows = 0;
		ncols = 0;
		pp_rows = NULL;
	}

void CMatrix::destroy_matrix()
	{
		if (pp_rows)
		{
			for (int i = 0; i<nrows; i++)
				delete[] pp_rows[i];
			delete[] pp_rows;
		}
		nrows = 0;
		ncols = 0;
		pp_rows = NULL;
	}

void CMatrix::copy_matrix(const CMatrix &m) 
	{
		this->destroy_matrix();
		this->nrows = m.nrows;
		this->ncols = m.ncols;
		if ((nrows*ncols) == 0) { pp_rows = NULL; return; }
		pp_rows = new double*[nrows];
		for (int i = 0; i<nrows; i++) pp_rows[i] = new double[ncols];
		for (int i = 0; i<nrows; i++)
			for (int j = 0; j<ncols; j++)
				pp_rows[i][j] = m.pp_rows[i][j];
	}

CMatrix::CMatrix(const CMatrix &m)
	{
		nrows = 0; ncols = 0; pp_rows = NULL;
		copy_matrix(m);
	}

CMatrix::~CMatrix()
	{
		destroy_matrix();
	}

CMatrix& CMatrix::operator=(const CMatrix& m) 
	{
		copy_matrix(m);
		return *this;
	}

void CMatrix::set_element(int a, int b, double value)
	{
		pp_rows[a][b] = value;
	}    

CMatrix CMatrix::operator+(CMatrix &m) // the name of the resulting matrix should be set manually
	{
		CMatrix c(nrows, ncols);
		for (int i = 0; i<nrows; i++)
			for (int j = 0; j<ncols; j++)
				c.set_element(i, j, pp_rows[i][j] + m.pp_rows[i][j]);
		return c;
	}

CMatrix CMatrix::operator-(CMatrix &m)
	{
		CMatrix c(nrows, ncols);
		for (int i = 0; i<nrows; i++)
			for (int j = 0; j<ncols; j++)
				c.set_element(i, j, pp_rows[i][j] - m.pp_rows[i][j]);
		return c;
	}

CMatrix CMatrix::operator*(CMatrix &m)
	{
		CMatrix c(nrows, m.ncols);
		for (int i = 0; i<nrows; i++)
		{
			for (int j = 0; j<m.ncols; j++)
			{
				double el = 0;
				for (int k = 0; k<ncols; k++)
					el += pp_rows[i][k] * m.pp_rows[k][j];
				c.set_element(i, j, el);
			}
		}
		return c;
	}

CMatrix CMatrix::transpose() 
	{
		CMatrix m(this->ncols, this->nrows);
		for (int i = 0; i<this->nrows; i++)
		{
			for (int j = 0; j<this->ncols; j++)
				m.set_element(j, i, pp_rows[i][j]);
		}
		return m;
	}

CMatrix CMatrix::get_cofactor(int r,int c)
	{
		CMatrix m(nrows-1,ncols-1);
		for(int i=0;i<m.nrows;i++)
		{
			for(int j=0;j<m.ncols;j++)
			{
				int sR = (i<r)?i:i+1;
				int sC = (j<c)?j:j+1;
				m.pp_rows[i][j] = pp_rows[sR][sC];
			}
		}
		return m;
	}

double CMatrix::get_determinant()
	{
		if(nrows==1 && ncols==1) return pp_rows[0][0];
		double values=0; int m=1;
		for(int i=0;i<nrows;i++)
		{
			values+=m*pp_rows[0][i]*get_cofactor(0,i).get_determinant();
			m*=-1;
		}
		return values;
	}

CMatrix CMatrix::operator/(CMatrix &m)
	{
	//	if(m.nrows!=m.ncols || this->nrows!=this->ncols) {cout<<"non-square matrix"<<endl; return;}
		double a=m.get_determinant_LU();
	//	if (a==0) {cout<<"the determinant of the matrix=0"<<endl; return;}
		CMatrix x(m.nrows,m.ncols); int sign=1;
		for(int i=0;i<x.nrows;i++)
		{
			for(int j=0;j<x.ncols;j++)
			{
				if(i%2 != j%2) sign=-1; else sign=1;
				x.set_element(i,j,sign*m.get_cofactor(i,j).get_determinant_LU());
				//sign*=-1;
			}
		}
		x=x.transpose();
		return *this*x*(1.0/a);
	}

CMatrix CMatrix::operator/(double d)
	{
		CMatrix r1(this->nrows, this->ncols);
		for (int i = 0; i<r1.nrows; i++)
			for (int j = 0; j<r1.ncols; j++)
				r1.set_element(i, j, double(this->pp_rows[i][j]) / d);
		return r1;
	}

CMatrix CMatrix::operator*(double d)
	{
		CMatrix r1(this->nrows, this->ncols);
		for (int i = 0; i<r1.nrows; i++)
			for (int j = 0; j<r1.ncols; j++)
				r1.set_element(i, j, this->pp_rows[i][j] * d);
		return r1;
	}

CMatrix CMatrix::operator+(double d)
	{
		CMatrix r1(this->nrows, this->ncols);
		for (int i = 0; i<r1.nrows; i++)
			for (int j = 0; j<r1.ncols; j++)
				r1.set_element(i, j, this->pp_rows[i][j] + d);
		return r1;
	}

CMatrix CMatrix::operator-(double d)
	{
		CMatrix r1(this->nrows, this->ncols);
		for (int i = 0; i<r1.nrows; i++)
			for (int j = 0; j<r1.ncols; j++)
				r1.set_element(i, j, this->pp_rows[i][j] - d);
		return r1;
	}

CMatrix CMatrix::operator-()
	{
		CMatrix r1(this->nrows, this->ncols);
		for (int i = 0; i<r1.nrows; i++)
			for (int j = 0; j<r1.ncols; j++)
				r1.set_element(i, j, -1 * this->pp_rows[i][j]);
		return r1;
	}

CMatrix CMatrix::num_sub_mat(double d)
	{
		return -*this + d;
	}

CMatrix CMatrix::num_div_mat(double d)
	{
		CMatrix r1(this->nrows, this->ncols);
		for (int i = 0; i<this->nrows; i++)
			for (int j = 0; j<this->ncols; j++)
				r1.set_element(i, j, 1.0 / this->pp_rows[i][j]);
		return r1*d;
	}

void CMatrix::print_matrix(string name)
	{
		if(print==0) return;
		cout << name << " =" << endl;
		for (int i = 0; i<nrows; i++)
		{
			for (int j = 0; j<ncols; j++)
				cout << "\t" << pp_rows[i][j];
			cout << endl;
		}
	}

////////////////////////////for the loving memory of jordan and guass/////////////////

CMatrix::CMatrix(int r,int c,string type)
{
		nrows = r;
		ncols = c;
		pp_rows = new double*[r];
		for (int i = 0; i<r; i++) pp_rows[i] = new double[c];
		for (int i = 0; i<r; i++){
			for (int j = 0; j<c; j++){
				if(i==j) pp_rows[i][j]=1;
				else pp_rows[i][j] = 0;
			}
		}
}

CMatrix CMatrix::inv()
{
	CMatrix m=*this;
	CMatrix x(m.nrows,m.ncols,"unity");
	for(int i=0;i<m.nrows;i++)
	{
		for(int j=0;j<m.ncols;j++)
		{
			double a=m.pp_rows[i][i];
			m.pp_rows[i][j]/=a;
			x.pp_rows[i][j]/=a;
		}
		for(int k=0;k<m.nrows;k++)
		{
			if(k==i) continue;
			else 
			{
				for(int z=0;z<m.ncols;z++)
				{
					double b=m.pp_rows[k][i];
					m.pp_rows[k][z]+=-1*b*m.pp_rows[i][z];
					x.pp_rows[k][z]+=-1*b*x.pp_rows[i][z];
				}
			}
		}
	}
	for(int i=0;i<m.nrows;i++)
	{
		for(int j=0;j<m.ncols;j++)
			cout<<m.pp_rows[i][j]<<" \t";
		cout<<endl;
	}
	for(int i=0;i<x.nrows;i++)
	{
		for(int j=0;j<x.ncols;j++)
			cout<<x.pp_rows[i][j]<<" \t";
		cout<<endl;
	}
	return x;
}

bool CMatrix::check_singularity()
{
	int zero=0;
	for(int i=0;i<this->nrows-1;i++)
	{
		double a=pp_rows[i+1][0]/pp_rows[i][0];
		for(int j=1;j<this->ncols;j++)
		{
			double b=pp_rows[i+1][j]/pp_rows[i][j];
			if(a!=b) {break;}
			if(j==this->ncols-1){zero=1;}
		}
		if(zero) break;
	}
	if(zero) return true;
	else if(this->get_determinant_LU()==0) return true;
	else return false;
}

double CMatrix::get_determinant_LU()
{
	CMatrix m=*this;
	double det=1;
	for(int i=0;i<m.nrows;i++)
	{
		double a=m.pp_rows[i][i];
		if(a==0)
		{
			for(int x=i+1;x<m.nrows;x++)
			{
				int found=0;
				if(m.pp_rows[x][i]!=0)
				{
					found=1;
					a=m.pp_rows[x][i];
					for(int y=0;y<m.ncols;y++)
					{
						double temp=m.pp_rows[i][y];
						m.pp_rows[i][y]=m.pp_rows[x][y];
						m.pp_rows[x][y]=temp;
					}
				}
			if(found==1) {det*=-1; break;}
			}
		}
		for(int j=i+1;j<m.nrows;j++)
		{
		//	a=m.pp_rows[i][i];
			double b=m.pp_rows[j][i];
			if(b==0) continue;
			for(int k=0;k<m.ncols;k++)
			{
				double c=-b/a;
				m.pp_rows[j][k]=m.pp_rows[j][k]+m.pp_rows[i][k]*c;
			}
		}
	}
	for(int i=0;i<m.nrows;i++) {det*=m.pp_rows[i][i];}
//	for(int i=0;i<m.nrows;i++){for(int j=0;j<m.ncols;j++){cout<<m.pp_rows[i][j]<<"\t";}cout<<endl;}
//	cout<<det;
	return det;
}

//////////////////////////////////parsing////////////////////////////////////////
int check(string name)
{
	int s = 0; unsigned int i;
	for (i = 0; i < names.size(); i++)
	{
		if (names[i] == name)
		{
			s = 1;
			break;
		}
	}
	if (s == 1)
		return i;
	else
		return -1;
}




void  dop(string&s)
{
	// updated on 29_10_2017 // to support cascading equal signs in operation line and the "./" operation
	s += ";";
	int j = 0, k = 0, t = 0, no_variables_equated = 0;
	string my_operation[100];

	string *ptr_variables_equated; string q = "";

	while (s[j] != ';')  //counting no_variables_equated, variables before the last equal sign
	{
		if (s[j] == '=') { no_variables_equated++; }
		j++;
	}
	// creating an array of string with size = no_variables_equated +1
	//it will be ended by "00"

	ptr_variables_equated = new string[no_variables_equated + 1];

	j = 0; t = 0; q = "";

	//filling the array with the names of variables to be equated
	//putting all the variables before the last equal sign in the array pointed by ptr_variables_equated

	while ((t<no_variables_equated))
	{

		if ((s[j] >= 'a' && s[j] <= 'z') || (s[j] >= 'A' && s[j] <= 'Z') || (s[j] >= '0' && s[j] <= '9') || (s[j] == '_'))
		{
			q += s[j];
			if (!((s[j + 1] >= 'a' && s[j + 1] <= 'z') || (s[j + 1] >= 'A' && s[j + 1] <= 'Z') || (s[j + 1] >= '0' && s[j + 1] <= '9') || (s[j + 1] == '_')))
			{
				ptr_variables_equated[t] = q; q = "";


				t++;
			}
		}

		j++;
	}

	//putting "00" in the last element of the array of strings pointed by ptr_variables_equated
	ptr_variables_equated[t] = "00";


	// i created an array of strings named my_operation to hold all the elements after the last equal sign
	// filling this array
	while (s[j] != ';')
	{
		if ( (s[j] >= '0' && s[j] <= '9')  || ( (s[j]== '-') && (s[j + 1] >= '0' && s[j + 1] <= '9') )
			|| ((s[j] == '.') && (s[j + 1] >= '0' && s[j + 1] <= '9'))       )

		{
			q += s[j];
			if (!(s[j + 1] >= '0' && s[j + 1] <= '9')  )
			{

				my_operation[k] = q;

				q = ""; k++;

			}

		}

		else if ((s[j] >= 'a' && s[j] <= 'z') || (s[j] >= 'A' && s[j] <= 'Z') || (s[j] == '_') || (s[j] >= '0' && s[j] <= '9'))

		{
			
			q += s[j];
			if (!((s[j + 1] >= 'a' && s[j + 1] <= 'z') || (s[j + 1] >= 'A' && s[j] <= 'Z') || (s[j + 1] == '_') || (s[j + 1] >= '0' && s[j + 1] <= '9')))
			{

				my_operation[k] = q;
				q = ""; k++;

			}

		}
		else if ((s[j] == '+') || (s[j] == '-') || (s[j] == '*') || (s[j] == '/') || (s[j] == '\'') || (s[j] == '.'))
		{
			if (s[j] == '.')
			{
				int xyz = j + 1;
				while (1)
				{
					if (s[xyz] == '/' || s[xyz]=='*' || s[xyz] == '+' || s[xyz] == '-')
					{
						my_operation[k] = s[j]; my_operation[k] += s[xyz]; // cout<< my_operation[k];
						break;
					}
					xyz++;
				}
				j = xyz + 1;

			}
			else  my_operation[k] = s[j];

			k++;
		}


		j++;
	}
	// putting "00" in the last element of my_operation array
	my_operation[k] = "00";

	//here, mr fady farag should use the array named my_operation to do the operations on the matrices
	//then, put the result in all the elements that are in the array pointed by ptr_variables_equated
	//you should loop in all these elements till you find "00"


	int a1 = -1, b1 = -1, tran = 0, inv = 0, dot = 0, defined = 0, dot_operand = -1, matched = 1;
	CMatrix op_res;

	if (my_operation[1] == "'")
	{
		tran = 1;
	}
	else if (my_operation[1] == "./" || my_operation[1] == ".*" || my_operation[1] == ".+" || my_operation[1] == ".-") { dot = 1; }

	for (unsigned int counter = 0; counter<v.size(); counter++)
	{
		if (names[counter] == my_operation[0] && (a1 == -1) && (!dot)) { a1 = check(names[counter]); /*cout<<a1<<endl;*/ }
		if ((!tran) && (!dot))
		{
			if (names[counter] == my_operation[2] && (b1 == -1)) { b1 = check(names[counter]);/*cout<<b1<<endl;*/ }
		}
		if (dot) {
			if (((int(my_operation[0][0]) >= 48) && (int(my_operation[0][0]) <= 57)) && (names[counter] == my_operation[2]))
			{
				a1 = atof(my_operation[0].c_str()); dot_operand = 0; b1 = check(names[counter]);
			}

			else  if ((int(my_operation[2][0]) >= 48 && int(my_operation[2][0]) <= 57) && (names[counter] == my_operation[0]))
			{
				b1 = atof(my_operation[2].c_str()); dot_operand = 2; a1 = check(names[counter]);
			}
		}
	}
	// cout<<a1<<" "<<b1<<" "<<tran<<" "<<inv<<" "<<endl;

	if ((((tran == 1)) && (a1 != -1)) || ((dot == 1) && (b1 != -1 || a1 != -1)) || ((inv == 0) && (tran == 0) && (a1 != -1) && (b1 != -1)))
	{
		defined = 1;
		if (my_operation[1] == "+")
		{
			if ((v[a1].nrows == v[b1].nrows) && (v[a1].ncols == v[b1].ncols))
				op_res = v[a1] + v[b1];
			else { cout << "unmatched dimensions of the two matrices" << endl; matched = 0; }
		}

		else if (my_operation[1] == "-")
		{
			if ((v[a1].nrows == v[b1].nrows) && (v[a1].ncols == v[b1].ncols))
				op_res = v[a1] - v[b1];
			else { cout << "unmatched dimensions of the two matrices" << endl; matched = 0; }
		}

		if (my_operation[1] == "*")
		{
			if ((v[a1].ncols == v[b1].nrows))
				op_res = v[a1] * v[b1];
			else { cout << "unmatched dimensions of the two matrices" << endl; matched = 0; }
		}

		if (my_operation[1] == "/")
		{
			//if(v[b1].get_determinant()==0) cout<<"the second matrix has determinant of zero"<<endl;
			if(v[b1].check_singularity()) {cout<<"the matrix "<<names[b1]<<" is singular."<<endl; matched=0;}
			else if ((v[a1].nrows == v[a1].ncols) && (v[b1].nrows == v[b1].ncols) && (v[a1].nrows == v[b1].ncols))
			op_res = v[a1] / v[b1];
			//op_res = v[a1]* v[b1].inv();
			else { cout << "unmatched dimensions of the two matrices" << endl; matched = 0; }
		}

		if (my_operation[1] == "'") { op_res = v[a1].transpose(); }

		if (my_operation[1] == "./")
		{
			if (dot_operand == 0) { op_res = v[b1].num_div_mat(a1); }
			else if (dot_operand == 2) { op_res = v[a1] / b1; }
		}

		if (my_operation[1] == ".*")
		{
			if (dot_operand == 0) { op_res = v[b1] * a1; }
			else if (dot_operand == 2) { op_res = v[a1] * b1; }
		}

		if (my_operation[1] == ".-")
		{
			if (dot_operand == 0) { op_res = v[b1].num_sub_mat(a1); }
			else if (dot_operand == 2) { op_res = v[a1] - b1; }
		}

		if (my_operation[1] == ".+")
		{
			if (dot_operand == 0) { op_res = v[b1] + a1; }
			else if (dot_operand == 2) { op_res = v[a1] + b1; }
		}


		//	 op_res.print_matrix();
	}
	else cout << "undefined matrices" << endl;
	if (defined&&matched)
	{
		for (int i = 0; ptr_variables_equated[i] != "00"; i++)
		{
			int place = check(ptr_variables_equated[i]);
			if (place != -1) { v[place] = op_res; }
			else {
				CMatrix temp;
				names.push_back(ptr_variables_equated[i]);
				temp = op_res;
				v.push_back(temp);
			}

		}
		op_res.print_matrix(ptr_variables_equated[0]);
	}
}



void create_matrix(string &s)
{
	s += ";";
	string q = "";
	int n_rows = 1, n_cols = 0, k = 0, j = 0, t = 0, no_variables_to_be_created = 0;
	double **p; string *ptr_variables_to_be_created;

	//i should create an array that contains the names of the variables to be created, the array's ptr is ptr_variables_to_be_created

	while (s[j] != '[')  //counting no_variables_to_be_created
	{
		if (s[j] == '=') { no_variables_to_be_created++; }
		j++;
	}

	// creating an array of string with size = no_variables_to_be_created +1 , bec it will be ended by "00"

	ptr_variables_to_be_created = new string[no_variables_to_be_created + 1];

	j = 0;


	//filling the array with the names of variables to be created
	while (s[j] != '[')
	{
		if ((s[j] >= 'a' && s[j] <= 'z') || (s[j] >= 'A' && s[j] <= 'Z') || (s[j] >= '0' && s[j] <= '9') || (s[j] == '_'))
		{
			q += s[j];
			if (!((s[j + 1] >= 'a' && s[j + 1] <= 'z') || (s[j + 1] >= 'A' && s[j + 1] <= 'Z') || (s[j + 1] >= '0' && s[j + 1] <= '9') || (s[j + 1] == '_')))
			{
				ptr_variables_to_be_created[t] = q; q = ""; t++;
			}
		}

		j++;
	}
	ptr_variables_to_be_created[t] = "00";
	t = 0;

	string name = q; q = "";

	while (s[j] != ']')
	{
		if (s[j] == ';')
		{
			if (s[j + 1] == ']')
				n_rows--;
			n_rows++;
		}
		j++;
	}

	j = 0;
	while (s[j] != ';')
	{
		if (((s[j] >= '0' && s[j] <= '9') || (s[j] == '-') || (s[j] == '.')))
		{
			if (!((s[j + 1] >= '0' && s[j + 1] <= '9') || (s[j + 1] == '-') || (s[j + 1] == '.')))
			{

				n_cols++;
			}
		}
		j++;

	}
	// cout<<n_rows<<" "<<n_cols<<"  "<<name<<endl;
	p = new double*[n_rows];

	for (int i = 0; i<n_rows; i++)
		p[i] = new double[n_cols];

	k = 0; t = 0; j = 0; q = "";
	while (s[j] != ']')
	{

		if (((s[j] >= '0' && s[j] <= '9') || (s[j] == '-') || (s[j] == '.')))
		{
			q += s[j];
			if (!((s[j + 1] >= '0' && s[j + 1] <= '9') || (s[j + 1] == '-') || (s[j + 1] == '.')))
			{
				p[k][t] = atof(q.c_str());
				//cout<<p[k][t]<<endl;
				q = "";
				t++;
			}

		}
		if (s[j] == ';') { k++; t = 0; }

		j++;

	}

	unsigned int counter = 0;


	while (ptr_variables_to_be_created[counter] != "00")
	{
		CMatrix c;
		if (check(ptr_variables_to_be_created[counter]) == -1)
		{
			names.push_back(ptr_variables_to_be_created[counter]);
			c.nrows = n_rows;
			c.ncols = n_cols;
			c.pp_rows = new double *[c.nrows];
			for (int qw = 0; qw < c.nrows; qw++)
			{
				c.pp_rows[qw] = new double[c.ncols];
			}
			for (int a = 0; a < c.nrows; a++)
			{
				for (int s = 0; s < c.ncols; s++)
				{
					c.pp_rows[a][s] = p[a][s];
				}
			}
			v.push_back(c);
			if (counter == 0)
				c.print_matrix(ptr_variables_to_be_created[0]);
		}
		else
		{
			for (int del = 0; del < v[check(ptr_variables_to_be_created[counter])].nrows; del++)
			{
				delete[]v[check(ptr_variables_to_be_created[counter])].pp_rows[del];
			}
			delete[]v[check(ptr_variables_to_be_created[counter])].pp_rows;
			///////deletion of old existed is complete.

			///////creation of new matrix in the same place of the old one.

			v[check(ptr_variables_to_be_created[counter])].nrows = n_rows;
			v[check(ptr_variables_to_be_created[counter])].ncols = n_cols;
			v[check(ptr_variables_to_be_created[counter])].pp_rows = new double *[v[check(ptr_variables_to_be_created[counter])].nrows];

			for (int qw = 0; qw < v[check(ptr_variables_to_be_created[counter])].nrows; qw++)
			{
				v[check(ptr_variables_to_be_created[counter])].pp_rows[qw] = new double[v[check(ptr_variables_to_be_created[counter])].ncols];
			}
			for (int a = 0; a < v[check(ptr_variables_to_be_created[counter])].nrows; a++)
			{
				for (int s = 0; s < v[check(ptr_variables_to_be_created[counter])].ncols; s++)
				{
					v[check(ptr_variables_to_be_created[counter])].pp_rows[a][s] = p[a][s];
				}
			}
			if (counter == 0)
				v[check(ptr_variables_to_be_created[counter])].print_matrix(ptr_variables_to_be_created[0]);
		}
		counter++;
	}
}







//now we must create another copies of the matrix c and put them in the vector, using ptr_variables_to_be_created,
// these matrices have the same elements as matrix c only differing in sring name 
// u shoud start creating matrices starting from ptr_variables_to_be_created[1] till you find "00" ...

void detect_instruction(string&s)
{
	if (s.find("[", 0) != -1) create_matrix(s);
	else dop(s);
}


//////////////////**********************  Phase 2 starts here    *************///////////////////


bool checkchar(char x)  
{
	if( x=='0' || x =='1' || x=='2'||x=='3'||x=='4'||x=='5'||x=='6'||x=='7'||x=='8'||x=='9'||x=='.')
	{
		return true;
	}
	else return false;
}

  /*        Number Parsing   */

float domath(string&a)
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
		if( checkchar(a[no_of_char]) ||(a[no_of_char]=='-' && n1==""&&last!=")") )
		{
			if(last=="^") 
			{
				while( (checkchar(a[no_of_char]))||(a[no_of_char]=='-'&&n1==""))
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
			while((checkchar(a[no_of_char])||(a[no_of_char]=='-'&&n1==""))&&no_of_char < length)
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
			if(function=="csc")v.push_back(1.0/sin(result));
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

float vector_cal ( vector <float> v , vector<char>signs )
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
					{temp[j] = p[j]/p[j+1];if(size==1) return temp[j];}
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








float subcal(string h,int nb)
{
	//cout<<nb<<endl;
	if(nb==0)
		return domath(h);
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
		 h=h.replace(pos[i],d-pos[i]+1,to_string(domath(part)));
		cout<<"my favourite part is "<<h<<endl;
		if(h.find('(',0)==-1)
			return domath(h);
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




float calculate(string a)
{
	string w="";int no_of_char=0;string last;string mod;unsigned int coo=0;
	int length = a.length();vector<float> v;vector<char>signs;float result=0;
	while(no_of_char<length)
	{
		if(checkchar(a[no_of_char]) || (a[no_of_char]=='-' &&w==""&&last!=")") )
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

		else if( (a[no_of_char] == 'l'&&a[no_of_char+1]=='o'&&a[no_of_char+2]=='g')||(a[no_of_char] == 's'&&a[no_of_char+1]=='i'&&a[no_of_char+2]=='n')
			||(a[no_of_char] == 'c'&&a[no_of_char+1]=='o'&&a[no_of_char+2]=='s')||(a[no_of_char] == 't'&&a[no_of_char+1]=='a'&&a[no_of_char+2]=='n')||
			(a[no_of_char] == 's'&&a[no_of_char+1]=='e'&&a[no_of_char+2]=='c')||(a[no_of_char] == 'c'&&a[no_of_char+1]=='s'&&a[no_of_char+2]=='c') 
			||(a[no_of_char] == 'e'&&a[no_of_char+1]=='x'&&a[no_of_char+2]=='p'))
		{
			if(result!=0&&last==")")
			{
				v.push_back(result);
				result=0;
			}
			string function="";
			function+=a[no_of_char];
			function+=a[no_of_char+1];
			function+=a[no_of_char+2];
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
			while(1)
			{
				w+=a[no_of_char];
				if(a[no_of_char]=='(')noopen++;
				if(a[no_of_char]==')')noclosed++;
				if(noopen==noclosed)break;
				else 
				{
					no_of_char++;
					continue;
				}
			}
			int end = no_of_char;
			w=w.erase(0,1);w=w.erase(w.length()-1,1);
			float res = subcal(w,noopen-1);
			w="";
			if(function=="sin") v.push_back(sin(res));
			if(function=="cos")v.push_back(cos(res));
			if(function=="tan")v.push_back(tan(res));
			if(function=="log"){ if (res<=0 ) throw(0);else { v.push_back(log10(res));}}
			if(function=="sec")v.push_back(1.0/cos(res));
			if(function=="csc")v.push_back(1.0/sin(res));
			if(function=="exp")v.push_back(exp(res));
			result =0;
			last = ")";
			no_of_char=end+1;
		}

		else if ( a[no_of_char]=='s' && a[no_of_char+1]=='q')
		{
			no_of_char=a.find('(',no_of_char);
			int beginning = no_of_char;
			w+=a[no_of_char];
			no_of_char++;
			int noopen=1;
			int noclosed=0 ;
			while(1)
			{
				w+=a[no_of_char];
				if(a[no_of_char]=='(')noopen++;
				if(a[no_of_char]==')')noclosed++;
				if(noopen==noclosed)break;
				else 
				{
					no_of_char++;
					continue;
				}
			}
			int end = no_of_char;
			w=w.erase(0,1);w=w.erase(w.length()-1,1);
			float res = subcal(w,noopen-1);
			w="";
			if (res < 0) throw (1) ;
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
			while(1)
			{
				w+=a[no_of_char];
				if(a[no_of_char]=='(')noopen++;
				if(a[no_of_char]==')')noclosed++;
				if(noopen==noclosed)break;
				else 
				{
					no_of_char++;
					continue;
				}
			}
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

	/* End of number_parsing */







CMatrix trigofmatrix (CMatrix &a , string type)   /* Calculate sine,cosine,etc.. of matrix */
{
	///// this function wheather type is sqrt or sin or log or ln of matrix a then calculate it 
	////and return result
	return a;
}

CMatrix cal_vectors ( vector<CMatrix>renew , string op )
{
	CMatrix result;
	if((op == ".*" )|| (op == "./") ||(op == ".+" )||(op == ".-" ) ) /* matrix .* matrix  OR no ./ matrix */
	{
		if(renew[0].type==true && renew[1].type ==true)  /* matrix .- matrix */
		{
			if( op == ".+" ) return result ;   /* should return renew[0] .+ renew[1] */
			else if ( op ==".-") return result ; 
			else if ( op == ".*" ) return result ;
			else return result;
		}
		else    // num .+ matrix 
		{
			float number = 0;
			if(!renew[0].type) {number = renew[0].pp_rows[0][0];result = renew[1];}
			else{ number = renew[1].pp_rows[0][0]; result = renew[0];}
			if( op == ".*" ) return result*number ;
			else if(op=="./"){ if(!renew[0].type) return result.num_div_mat(number); } 
			else if ( op==".+") return result + number;
			else { if(renew[0].type){return result - number;} else return result - (-1*number);}
		}
	}
	else  /* number+number OR matrix + matrix */
	{
		if(renew[0].type && renew[1].type )  /* matrix + matrix */
		{
			if(op=="+") return renew[0] + renew[1];
			else if (op == "-") return renew[0] - renew[1];
			else if (op == "*") return renew[0] * renew[1];
			else return renew[0]/renew[1];
		}
		else /* number + number */
		{
			result.type=false;result.nrows=1;result.ncols=1;
			if(op == "+") result = renew[0]+renew[1];
			else if ( op =="-") result = renew[0]-renew[1];
			else if (op == "*") result = renew[0]*renew[1];
			else result = renew[0]/renew[1];
		}
	}
}



CMatrix calculatemat(string a[],int n)  /* Return result of matrix operation */
{
	CMatrix first(0,0) ,second(0,0);
	vector<CMatrix> hash ;
	vector<CMatrix> ds;
	vector<CMatrix>renew;
	int no =0;
	string temp="";
	string last="";
	bool pushhash=0;
	while(no<n)
	{
		temp= a[no];
		int length = temp.length();
		int no_char =0 ;

		if((temp[0]=='s'&&temp[1]=='i')||(temp[0]=='l'&&temp[1]=='o')||(temp[0]=='c'&&temp[1]=='o')||(temp[0]=='t'&&temp[1]=='a')
			||(temp[0]=='e'&&temp[1]=='x')) // therefore we have log or sin or exp or cos or tan
		{
			string function = temp.substr(0 , 3);
			no_char+=3;
			char special = temp[no_char];
			no_char++;
			int getno = atoi(temp.substr(no_char , length - no_char).c_str());
			if(special=='#') {renew.push_back(trigofmatrix(hash[getno] , function));}
			else renew.push_back(trigofmatrix(ds[getno] , function));
			no++;
		}

		else if ( temp[0]=='s' && temp[1]=='q')
		{
			string function = temp.substr(0 , 4);
			no_char+=4;
			char special = temp[no_char];no_char++;
			int getno = atoi(temp.substr(no_char , length - no_char).c_str());
			if(special=='#') {renew.push_back(trigofmatrix(hash[getno] , function));}
			else renew.push_back(trigofmatrix(ds[getno] , function));
			no++;
		}
		else if (temp=="00") break;
		else if ((temp[0]=='#'&&temp[1]!='#') || (temp[0]=='$' && temp[1]!='$')) 
		{
			char special = temp[0];
			no_char++;
			int getno = atoi(temp.substr(no_char , length-no_char).c_str());
			if(special=='#') renew.push_back(hash[getno]);
			else renew.push_back(ds[getno]);
			no++;
		}
		else if(temp==".+" || temp==".-" ||temp==".*" ||temp=="./") {no++;continue;}
		else if (checkchar(temp[0])||temp[0]=='-')    /* here we have a number so store it as 1*1 matrix */
		{
			CMatrix c ;
			c.nrows=1;c.ncols=1;
			c.type=false;
			c.pp_rows=new double *[1];
			c.pp_rows[0]=new double [1];
			float rty = atof(temp.c_str());
			c.pp_rows[0][0] =  rty ;
			renew.push_back ( c );
			no++;
		}
		else if ( (temp[0] >= 65 && temp[0] <= 90) || (temp[0] >= 97 && temp[0] <= 122))  /* can be no or matrix */
		{
			if(check_for_var(temp)!= -1) /* therefore its a no*/
			{
				CMatrix c ; 
				c.nrows=1;c.ncols=1;c.pp_rows=new double*[1];
				c.pp_rows[0][0] = variables[check_for_var(temp)].value;
				renew.push_back(c);
				no++;
			}
			else    /* therefore its a matrix */
			{
				renew.push_back(v[check(temp)]);
				renew[renew.size()-1].type=true;
				no++;
			}
		}
		else if (temp=="$$")
		{
			if(renew.size()==2) /* we have an operation .* or /*/
			{
				hash.push_back(cal_vectors(renew , a[no-2] ));
				renew.erase ( renew.begin() , renew.begin()+2 ) ;
				no++;
			}
			else  /*  we have only one element we want to store into hash */
			{
				hash.push_back(renew[renew.size()-1]);
				renew.erase(renew.begin() , renew.begin()+1 );
				no++;
			}
		}
		else if ( temp=="##")
		{
			 if(renew.size()==2) /* we have an operation .* or /*/
			{
				ds.push_back(cal_vectors(renew , a[no-2] ));
				renew.erase ( renew.begin() , renew.begin()+2 ) ;
				hash.erase(hash.begin() , hash.begin()+hash.size() );
				no++;
			}
			 else
			 {
				 ds.push_back(renew[renew.size()-1]);
				 hash.erase(hash.begin() , hash.begin()+hash.size() );
				 no++;
			 }
		}
		else
		{
			no++;
			continue;
		}
	}	

	if ( renew.size() == 2 )
	{
		return cal_vectors(renew , a[no-2] );
	}
	else
		return renew[renew.size() - 1 ] ;
}






 



