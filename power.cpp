#include<math.h>
//////////////////////////////////////////////////////////////////////////
cMatrix cMatrix::operator^ (int a)
{
	cMatrix y(nrows, ncols);
	cMatrix x[100];
	x[0] = *this;
	int n = 1;
	for (n = 1;pow(2, n) <= a;n++)
	{
		x[n] = x[n - 1] * x[n - 1];

	}
	n--;
	int diff = a - pow(2, n);
	if (diff == 0)
	{
		return x[n];
	}
	else {
		y = x[n];

		while (diff > 0) {

			if (diff >= pow(2, n))
			{
				diff = diff - pow(2, n);
				//	cout << diff<<"xxxxxxx";
				y = y*x[n];


			}
			else {
				--n;
				//cout << n<<endl;
			}
		}
		return y;
	}



}
