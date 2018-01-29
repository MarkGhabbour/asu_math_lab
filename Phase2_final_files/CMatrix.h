#ifndef Matrix_m
#define Matrix_m

#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<math.h>

using namespace std;

class CMatrix
{
	friend class CParser;
private:       
	bool type;
	
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
	CMatrix   operator^ (int a);
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
	CMatrix partial_mul(CMatrix&m);
	CMatrix partial_div(CMatrix&m);
	void print_matrix(string name);
	bool check_singularity();
	CMatrix(int r,int c,string type);  //constructor to be used by determinant function
	CMatrix inv();
	double get_determinant_LU();


	/////////phase 2///////////////
	CMatrix(int r,int c,int mode,string name);  //1:zeros  2:ones  3:rand  4:eye
	CMatrix(string h,string name);
	  void print_mat();

	static CMatrix trigofmatrix (CMatrix &a , string type);
	static CMatrix cal_vectors ( vector<CMatrix>renew , string op );
	static CMatrix calculatemat(string a[],int n);
	static CMatrix calculate_expression(string s);
	static bool checkchar(char x);
	static int check(string name);
};

class CVariables
{
	friend class CParser;
	friend class CMatrix;
private:
	double value;
	string name;
public:
	CVariables(double value,string name);
	void print_var();
static	int check_for_var(string name_to_check );   /*function to check existance of variable like that of matrix*/
	
};




#endif
