#include"CMatrix.h"

//vector <CMatrix> v;
//vector <CVariables>  variables;

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
				else if(mode==3) pp_rows[i][j]=rand(); //we need to adjust the range of the random number
				else if(mode==4) pp_rows[i][j]=((i==j)?1:0);
			}
		}
	  }

CMatrix::CMatrix(string h,string name)
	  {
		  this->name=name;
	//	  cout<<"name: "<<name<<endl;
		  int r=1,c=1,enough_cols=0;
		  for(int i=0;i<h.length();i++)
		  {
			  if((h[i]==' ' || h[i]==',')&& !enough_cols) c++;
			  else if(h[i]==';' || h[i]==13 )    //13:the ascii code of enter key
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
		  for(int i=0;i<=h.length();i++)	//i==h.length()!!!!!!
		  {
			  if((h[i]==' ')||(h[i]==',') ||((i==h.length())) )
			  {
				  pp_rows[r][c]=atof(el.c_str());
				  c++;
				  el="";
			  }
			  else if((h[i]==';')||(h[i]==13)) 
			  {
				 pp_rows[r][c]=atof(el.c_str()); 
				 c=0; 
			   	 r++; 
				 el="";
			  }
			  else el+=h[i];
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