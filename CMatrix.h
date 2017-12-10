#ifndef Matrix_m
#define Matrix_m

#include<iostream>
#include<string>
#include<vector>

using namespace std;

class CMatrix
{

private:       
	bool type;
	int nrows, ncols;
	double** pp_rows;
public:
	friend CMatrix cal_vectors ( vector<CMatrix> renew, string op );  /* phase 2 */
	friend CMatrix calculatemat(string a[],int &n);          /* phase 2 */
	static int  print;
	bool get_type() ;
	CMatrix(int r, int c);
	CMatrix();
	void destroy_matrix();
	void copy_matrix(const CMatrix &m);
	CMatrix(const CMatrix &m);
	~CMatrix();
	CMatrix& operator=(const CMatrix& m);
	void set_element(int a, int b, double value);
	CMatrix  operator+(CMatrix &m);
	CMatrix operator-(CMatrix &m);
	CMatrix  operator*(CMatrix &m);
	CMatrix  transpose();
	CMatrix get_cofactor(int r,int c);
	double get_determinant();
	CMatrix operator/(CMatrix &m);
	CMatrix operator/(double d);
	CMatrix operator*(double d);
	CMatrix operator+(double d);
	CMatrix operator-(double d);
	CMatrix operator-();
	CMatrix num_sub_mat(double d);
	CMatrix num_div_mat(double d);
	void print_matrix(string name);

	bool check_singularity();
	CMatrix(int r,int c,string type);
	CMatrix inv();
	double get_determinant_LU();

	friend void create_matrix(string&s);
	friend int check(string);
	friend void dop(string&s);
	friend void  detect_instruction(string&s);
};

class CVariables
{
private:
	float value;
	string name;
public:
	friend int check_for_var(string name_to_check );
	friend CMatrix calculatemat(string a[],int &n);
	friend CMatrix cal_vectors ( vector<CMatrix>renew , string op );
};


#endif