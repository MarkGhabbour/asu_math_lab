#include"CParser.h"
#include"CMatrix.h"

vector <CVariables> vars;
vector <CMatrix> mats;

void CParser::take_input()
{
	while(1)
	{
		 CMatrix::print=1;	//this is the static variable of the class CMatrix::print
		int open_brakets=0,closed_brakets=0,equal_sign=0;
		string input="";
		do
		{
			string h;
			getline(cin,h);
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
		}
	}
    


	if(mode==2)
	{
		mats.push_back(CMatrix(h,"#"+to_string(temp_counter++)));
		return mats[mats.size()-1].name;
	}
	else return h;

}
