#include"CMatrix.h"
#include"CParser.h"

vector <CVariables> vars;
vector <CMatrix> mats;


int CMatrix::print;

CMatrix::CMatrix()
	{
		nrows = 0;
		ncols = 0;
		pp_rows = NULL;
	}

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

CMatrix::~CMatrix()
	{
		destroy_matrix();
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
		this->name=m.name;
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

CMatrix& CMatrix::operator=(const CMatrix& m) 
	{
		copy_matrix(m);
		return *this;
	}

CMatrix CMatrix::partial_mul(CMatrix &m)
{
	CMatrix x(this->nrows,this->ncols);
	x.type=true;
	for(int i=0;i<x.nrows;i++)
	{
		for(int j=0;j<x.ncols;j++)
		{
			x.set_element(i,j,this->pp_rows[i][j]*m.pp_rows[i][j]);
		}
	}
	return x;
}

CMatrix CMatrix::partial_div(CMatrix &m)
{
	CMatrix x(this->nrows,this->ncols);
	x.type=true;
	for(int i=0;i<x.nrows;i++)
	{
		for(int j=0;j<x.ncols;j++)
		{
			x.set_element(i,j,this->pp_rows[i][j]/m.pp_rows[i][j]);
		}
	}
	return x;
}

CMatrix CMatrix::operator+(CMatrix &m) 
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
		double a=m.get_determinant_LU();
		CMatrix x(m.nrows,m.ncols); int sign=1;
		for(int i=0;i<x.nrows;i++)
		{
			for(int j=0;j<x.ncols;j++)
			{
				if(i%2 != j%2) sign=-1; else sign=1;
				x.set_element(i,j,sign*m.get_cofactor(i,j).get_determinant_LU());
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


void CMatrix::set_element(int a, int b, double value)
	{
		pp_rows[a][b] = value;
	}  

CMatrix CMatrix::num_sub_mat(double d)  //A-4
	{
		return -*this + d;
	}

CMatrix CMatrix::num_div_mat(double d) // 5/A 
	{
		CMatrix r1(this->nrows, this->ncols);
		for (int i = 0; i<this->nrows; i++)
			for (int j = 0; j<this->ncols; j++)
				r1.set_element(i, j, 1.0 / this->pp_rows[i][j]);
		return r1*d;
	}

CMatrix CMatrix::mat_pow_num(double d)  //B^2
{
	CMatrix x(this->nrows,this->ncols);
	for(int i=0;i<nrows;i++)
		for(int j=0;j<ncols;j++)
			x.set_element(i,j,pow(pp_rows[i][j],d));
	return x;
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

CMatrix CMatrix::inv() // mark correction
{
	CMatrix m=*this;
	CMatrix x(m.nrows,m.ncols,"unity"); // creating a unity matrix   //this is a constructor needs to be completed i made a similar one can be removed just use it for now
	for(int i=0;i<m.nrows;i++)
	{
		for(int j=0;j<m.ncols;j++)
		{
			double a=m.pp_rows[i][i];  // the element in the main diagonal
			m.pp_rows[i][j]/=a;        // devide both the main matrix and the unity matrix by it to make the element in the main diagonal one
			x.pp_rows[i][j]/=a;
		}
		for(int k=0;k<m.nrows;k++)
		{
			if(k==i) continue;
			else 
			{
				for(int z=0;z<m.ncols;z++)    // looping through all elements except the main diagonal
				{
					double b=m.pp_rows[k][i]; // the corresponding element in each row we'll use it to zero the other elements under it
					m.pp_rows[k][z]+=-1*b*m.pp_rows[i][z];
					x.pp_rows[k][z]+=-1*b*x.pp_rows[i][z];
				}
			}
		}
	}
	//return the matrix that was a unity matrix
	return x;
}

double CMatrix::get_determinant_LU()
{
	CMatrix m=*this;
	double det=1;
	for(int i=0;i<m.nrows;i++)
	{
		double a=m.pp_rows[i][i]; // if the element we wish to divide by is zero so we'll find another row and replace it with it and multiply by -1
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
	return det;
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


/////////////////phase 2////////////////
CMatrix::CMatrix(int r,int c,int mode,string name)  //1:zeros  2:ones  3:rand  4:eye
    {
        this->nrows=r;
        this->ncols=c;
		this->name=name;
        pp_rows=new double*[r];
        for(int i=0;i<r;i++)
        {
            pp_rows[i]=new double[c];
			for(int j=0;j<c;j++)
			{
				if(mode==1) pp_rows[i][j]=0;
				else if(mode==2) pp_rows[i][j]=1;
				else if(mode==3) pp_rows[i][j]=rand()%10; //we need to adjust the range of the random number
				else if(mode==4) pp_rows[i][j]=((i==j)?1:0);
			}
		}
	  }

CMatrix::CMatrix(string h,string name)
	  {
		  this->name=name;
		  int r=1,c=1,enough_cols=0;
		  for(int i=0;i<h.length();i++)
		  {
			  if((h[i]==' ' || h[i]==',')&& !enough_cols) c++;
			  else if((h[i]==';' || h[i]==13)  && (i!=h.length()-1))    //13:the ascii code of enter key
			  {
				  r++;
				  enough_cols=1;
			  }
		  }
		//  int mode=1;
		  //if(name[1]=='#') mode=2;
		//  *this=CMatrix(r,c,mode,name);

		  //it's own initialization
		  this->nrows=r;
		  this->ncols=c;
		  this->pp_rows=new double*[r];
		  for(int i=0;i<r;i++)
		  {
			  this->pp_rows[i]=new double [c];
			 for(int j=0;j<c;j++)
			 {
				 pp_rows[i][j]=0;
			 }
		  }
		  r=0;c=0;
		  string el="";
		  string operations="+/*^()";
		  for(int i=0;i<h.length();i++)	
		  {
			  if((h[i]==' ')||(h[i]==',') ||((i== (h.length() -1) )) )
			  {
				  if(el.find_first_of(operations.c_str())!=string::npos || (el.rfind('-')>0))
				  {
					  pp_rows[r][c]=CParser::calculate(el , First);
				  }
				  else pp_rows[r][c]=atof(el.c_str());
				  c++;
				  el="";
			  }
			  else if(((h[i]==';')||(h[i]==13)) &&(i!=h.length()-1)) 
			  {
				 if(el.find_first_of(operations.c_str())!=string::npos || (el.rfind('-')>0))
				  {
					  pp_rows[r][c]=CParser::calculate(el , First);
				  }
				  else pp_rows[r][c]=atof(el.c_str());
				 c=0; 
			   	 r++; 
				 el="";
			  }
			  else el+=h[i];

			  if((i+1)== (h.length()-1))   //the solution for: i==h.length()!!!!!!
				  el+=h[i+1];
		  }
	  }

 void CMatrix::print_mat()   //we need also to print the name   //and activate the static variable print
	  {
		  if(CMatrix::print)
		  {
			  cout<<name<<" ="<<endl;
			  for(int i=0;i<nrows;i++)
			  {
				  for(int j=0;j<ncols;j++)
				  {
					  cout<<"		"<<pp_rows[i][j];
				  }
				  cout<<endl;
			  }
		  }
	  }



 CMatrix CMatrix::trigofmatrix (CMatrix &a , string type)
 {
	 CMatrix m(a.nrows,a.ncols);
	 for(int i=0;i<m.nrows;i++)
	 {
		 for(int j=0;j<m.ncols;j++)
		 {
			 if(type=="sin")
			 {
				 m.set_element(i,j,sin(a.pp_rows[i][j]));
			 }
			 else if(type=="cos")
			 {
				 m.set_element(i,j,cos(a.pp_rows[i][j]));
			 }
			 else if(type=="tan")
			 {
				 if(a.pp_rows[i][j]==1.5708) throw(2); // tan (90) 
				 m.set_element(i,j,tan(a.pp_rows[i][j]));
			 }
			 else if(type=="exp")
			 {
				 m.set_element(i,j,exp(a.pp_rows[i][j]));
			 }
			 else if(type=="log")  
			 {
				 if(a.pp_rows[i][j]<=0) throw(1) ; //log(0) or log(neg num)
				 m.set_element(i,j,log10(a.pp_rows[i][j]));
			 }
			 else if(type=="ln")
			 {
				 if(a.pp_rows[i][j]<=0) throw(1) ;
				 m.set_element(i,j,log(a.pp_rows[i][j]));
			 }
			 else if(type=="sqrt")
			 {
				 if(a.pp_rows[i][j]<0) throw 
				 m.set_element(i,j,sqrt(a.pp_rows[i][j]));
			 }
		 }
	 }
	 return m;
 }

 CMatrix CMatrix::cal_vectors ( vector<CMatrix>renew , string op )
{
	CMatrix result;
	if((op == ".*" )|| (op == "./") ||(op == ".+" )||(op == ".-" ) ) /* matrix .* matrix  OR no ./ matrix */
	{
		if(renew[0].type==true && renew[1].type ==true)  /* matrix .- matrix */
		{
			if( op == ".+" ) return renew[0]+renew[1] ;   /* should return renew[0] .+ renew[1] */
			else if ( op ==".-") return renew[0]-renew[1] ; 
			else if ( op == ".*" ) return renew[0].partial_mul(renew[1]) ;      // should return renew[0].partial_mul(renew[1])
			else return renew[0].partial_div(renew[1]);                         // should return renew[0].partial_div(renew[1])
		}
		else    // num .+ matrix 
		{
			float number = 0;
			if(!renew[0].type) {number = renew[0].pp_rows[0][0];result = renew[1];}
			else{ number = renew[1].pp_rows[0][0]; result = renew[0];}
			if( op == ".*" ) return result*number ;
			else if(op=="./") return result.num_div_mat(number);  
			else if ( op==".+") return result + number;
			else return result - number;
		}
	}
	else  /* number+number OR matrix + matrix */
	{
		if(renew[0].type && renew[1].type )  /* matrix + matrix */
		{
			if(op=="+"||op==".+") return renew[0] + renew[1];
			else if (op == "-"||op==".-") return renew[0] - renew[1];
			else if (op == "*") return renew[0] * renew[1];
			else if (op==".*") return renew[0].partial_mul(renew[1]) ; // should return renew[0].partial_mul(renew[1])
			else if (op=="./") return renew[0].partial_div(renew[1]) ; // should return renew[0].partial_div(renew[1])
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

 CMatrix CMatrix::calculatemat(string a[],int n)  /* Return result of matrix operation */
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
		else if(temp==".+" || temp==".-" ||temp==".*" ||temp=="./"||temp=="+"||temp=="-"||temp=="*"||temp=="/") {no++;continue;}
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
			if(CVariables::check_for_var(temp)!= -1) /* therefore its a no*/
			{
				CMatrix c ; 
				c.nrows=1;c.ncols=1;c.pp_rows=new double*[1];
				c.pp_rows[0][0] = vars[CVariables::check_for_var(temp)].value;
				renew.push_back(c);
				no++;
			}
			else    /* therefore its a matrix */
			{
				renew.push_back(mats[check(temp)]);
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

CMatrix CMatrix::calculate_expression(string s)
{
	//////// cout part /////////////
	//cout << endl << "  my_string = " << s << endl<<endl;
	
	string my_string_after_parentheses = CParser::handle_parentheses(s);
	string my_string_after_priorities =CParser:: handle_priorities(my_string_after_parentheses);

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


return result;

}


bool CMatrix::checkchar(char x)  
{
	if( x=='0' || x =='1' || x=='2'||x=='3'||x=='4'||x=='5'||x=='6'||x=='7'||x=='8'||x=='9'||x=='.')
	{
		return true;
	}
	else return false;
}

int CMatrix::check(string name)
{
	int s = 0; unsigned int i;
	for (i = 0; i < mats.size(); i++)
	{
		if (mats[i].name == name)
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




///////////////CVariables/////////////

CVariables::CVariables(double value,string name)
	{
		this->name=name;
		this->value=value;
	}
	void CVariables::print_var()
	{
		if(CMatrix::print)
		{
			cout<<name<<"="<<endl;
			cout<<"            "<<value<<endl;
		}
	}
int CVariables::check_for_var(string name_to_check )   /*function to check existance of variable like that of matrix*/
	{
		for(int i=0; i < vars.size() ; i++ )
		{
			if(vars[i].name == name_to_check)
				return i;
		}
		return -1;
	}