#include <iostream>
#include <algorithm>
#include <math.h>
#include <cstdlib>
#include <fstream>
#include <constant.h>
using namespace std;
extern int INTE_BLOCK;
double f_w(double lambda, double i)
{
	return exp(-lambda / 2) * pow(lambda / 2, i) / tgamma(i - 1);
}

double f_inte_0(double t, param P, double i, double j)
{
	return pow(t, P.k1 / 2 + i + P.p - 1) *
		pow(1 - t, P.v1 / 2 + j - 1 + P.q) /
		pow(t + P.alpha * (1 - t), P.r);
}

double array_simp_part(double arr[]) 
{
	return 1.0 / 6.0*(arr[0] + arr[2] + 4 * arr[1]);
}

void array_queue_on(double arr[], double a)
{
	arr[0] = arr[1];
	arr[1] = arr[2];
	arr[2] = a;
}

double f_inte(param P, double i, double j)
{	
	int x;
	double tmp[3];
	double result = 0.0, part_inte;
	double tau_star = P.k1 * P.tau / (P.v1 + P.k1 * P.tau);
	if (P.tau == 0) return 0.0;
	for (x = 0; x < INTE_BLOCK; x++)
	{	
		part_inte = f_inte_0(double(x) / double(INTE_BLOCK) * tau_star, P, i, j);
		if (x < 2)
		{
			tmp[x + 1] = part_inte;
		} else {
			array_queue_on(tmp, part_inte);
			result += double(2) / double(INTE_BLOCK) * array_simp_part(tmp);
		}
	}
	return result;
}

double f_G(param P, double i, double j)
{
	double g1, g2;
	g1 = tgamma((P.k1 + P.v1)/ 2 + i + j + P.p + P.q - P.r) /
		tgamma(P.k1 / 2 + i) / tgamma(P.v1 / 2 + j);
	g2 = f_inte(P, i, j);
	return g1 * g2;
}

//double f_H(param P)
//{
//	//assert pow(2, P.p + P.q - P.r)==2
//	2;
//	f_w(P.lambda1, i) * f_w(P.lambda2)
//}