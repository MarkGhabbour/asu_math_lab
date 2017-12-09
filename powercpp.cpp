cMatrix cMatrix::power(int n)
{
	/*	if (n == 2)
	{

	matrix x(nR, nC);

	for (int i = 0; i < nR; i++)
	{
	for (int j = 0; j < nC; j++)
	{


	for (int k = 0; k < nR; k++)
	{
	x.values[i][j] += values[i][k] * values[k][j];
	}
	}

	}
	return x;
	}
	else if (n == 1)
	{
	return *this;
	}
	else if (n % 2 == 0)
	{
	return (power(n / 2)*power(n / 2));
	}
	else if (n % 2 == 1)
	{
	matrix y(nR, nC);
	for (int i = 0; i < nR; i++)
	{
	for (int j = 0; j < nC; j++)
	{
	y.values[i][j] = values[i][j];
	}
	}

	return (power(n / 2)*power(n / 2)*y);
	}*/
	/////////////////////////////////////////////////////////
	cMatrix y(nR, nC);
	for (int i = 0; i < nR; i++)
	{
		for (int j = 0; j < nC; j++)
		{
			y.values[i][j] = values[i][j];
		}
	}


	for (int i = 1;i < n;i++)
	{
		cMatrix x(nR, nC);

		for (int i = 0; i < nR; i++)
		{
			for (int j = 0; j < nC; j++)
			{


				for (int k = 0; k < nR; k++)
				{
					x.values[i][j] += values[i][k] * values[k][j];
				}
			}

		}

		y.copy(x);


	} return y;
}
////////////////////////////////////////////////////////////////
cMatrix cMatrix::operator^ (int n)
{
	power(n);
	return *this;
}