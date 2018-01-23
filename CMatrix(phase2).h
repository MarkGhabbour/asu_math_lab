#ifndef Matrix_m
#define Matrix_m

#include<iostream>
#include<string>
#include<vector>

using namespace std;

class CMatrix
{
	friend class CParser;
private:       
	// bool type;
	
	int nrows, ncols;
	double** pp_rows;
	string name;
public:
	static int  print;
	

	//constructors:
	CMatrix();
	CMatrix(int r, int c);

	//destructor:
	~CMatrix();
	void destroy_matrix();

	//copy constructor:
	CMatrix(const CMatrix &m);
	void copy_matrix(const CMatrix &m);
	
	//operators:
	CMatrix& operator=(const CMatrix& m);
	CMatrix  operator+(CMatrix &m);
	CMatrix operator-(CMatrix &m);
	CMatrix  operator*(CMatrix &m);
//--->	CMatrix   operator^ (int a);
	CMatrix operator/(CMatrix &m);
	CMatrix operator/(double d);
	CMatrix operator*(double d);
	CMatrix operator+(double d);
	CMatrix operator-(double d);
	CMatrix operator-();


	//another functions:
	void set_element(int a, int b, double value);
	CMatrix  transpose();
	CMatrix get_cofactor(int r,int c);
	double get_determinant();
	CMatrix num_sub_mat(double d);
	CMatrix num_div_mat(double d);
	CMatrix mat_pow_num(double d);
	void print_matrix(string name);
	bool check_singularity();
	CMatrix(int r,int c,string type);  //constructor to be used by determinant function
	CMatrix inv();
	double get_determinant_LU();


	/////////phase 2///////////////
	CMatrix(int r,int c,int mode,string name);  //1:zeros  2:ones  3:rand  4:eye
	CMatrix(string h,string name);
	  void print_mat();
};

class CVariables
{
	friend class CParser;
private:
	double value;
	string name;
public:
	CVariables(double value,string name)
	{
		this->name=name;
		this->value=value;
	}
	void print_var()
	{
		if(CMatrix::print)
		{
			cout<<name<<"="<<endl;
			cout<<"            "<<value<<endl;
		}
	}

	
};


#endif