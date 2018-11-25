#include <iostream>
#include <algorithm>
#include <math.h>
#include <cstdlib>
#include <fstream>
#include <constant.h>
using namespace std;
extern double INTE_BLOCK;
extern double CRIT_VALUE;
extern double INF;
extern int MAX_STEP;
extern double param_list[12][6];
double f_w(double lambda, double i)
{
	double p1 = exp(-lambda / 2), p2 = pow(lambda / 2, i), p3;
	p3 = i > 1 ? tgamma(i - 1) : 1;
	return p1 * p2 / p3;
}

double f_inte_0(double t, p_task P, double i, double j)
{
	double p1 = pow(t, P.k1 / 2 + i + P.p - 1);
	double p2 = pow(1 - t, P.v1 / 2 + j - 1 + P.q);
	double p3 = pow(t + P.alpha * (1 - t), P.r);
	return p1 * p2 / p3;
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

double f_inte(p_task P, double i, double j)
{	
	int x;
	double tmp[3];
	double result = 0.0, part_inte;
	double tau_star;
	if (P.tau == 0) return 0.0;
	//if (P.r == 0 && P.alpha == 0)
	//{
	//	return;
	//}
	//if (P.r == 1 && P.alpha == 0)
	//{
	//	return;
	//}
	tau_star = P.k1 * P.tau / (P.v1 + P.k1 * P.tau);
	for (x = 1; x < INTE_BLOCK; x++)
	{	
		part_inte = f_inte_0(double(x) / double(INTE_BLOCK) * tau_star, P, i, j);
		if (x < 3)
		{
			tmp[x] = part_inte;
		} else {
			array_queue_on(tmp, part_inte);
			result += double(2) / double(INTE_BLOCK) * array_simp_part(tmp);
		}
	}
	return result;
}

double f_G(p_task P, double i, double j)
{
	double g1, g2;
	g1 = tgamma((P.k1 + P.v1)/ 2 + i + j + P.p + P.q - P.r) /
		tgamma(P.k1 / 2 + i) / tgamma(P.v1 / 2 + j);
	g2 = f_inte(P, i, j);
	return g1 * g2;
}

double f_H(p_task P)
{
	//assert pow(2, P.p + P.q - P.r)==2
	int i = 0, j = 0;
	double tmp, tmpi = 0, tmpj, sum_res = 0;
	while (i < MAX_STEP) 
	{
		tmpj = 0;
		for (j = 0; j < MAX_STEP; j++) 
		{
			tmp = f_w(P.lambda1, i) * f_w(P.lambda2, j) * f_G(P, i, j);
			if (j == 0)
			{
				if (tmp <= CRIT_VALUE && tmp <= tmpi)
				{
					i = MAX_STEP;
					break;
				}
				tmpi = tmp;
			}
			if (tmp <= CRIT_VALUE && tmp <= tmpj) break;
			tmpj = tmp;
			sum_res += tmp;
		}
		//cout << "setp:" << i << ", res = " << sum_res << endl;
		i++;
	}
	return 2 * sum_res;
}



double f_J(p_task P)
{
	int i = 0, j = 0;
	double tmp, tmpi = 0, tmpj, sum_res = 0;
	while (i < MAX_STEP)
	{
		tmpj = 0;
		for (j = 0; j < MAX_STEP; j++)
		{
			tmp = f_w(P.lambda1, i) * f_w(P.lambda2, j) * f_G(P, i + 1, j);
			if (j == 0)
			{
				if (tmp <= CRIT_VALUE && tmp <= tmpi)
				{
					i = MAX_STEP;
					break;
				}
				tmpi = tmp;
			}
			if (tmp <= CRIT_VALUE && tmp <= tmpj) break;
			tmpj = tmp;
			sum_res += tmp;
		}
		//cout << "setp:" << i << ", res = " << sum_res << endl;
		i++;
	}
	return P.lambda1 * sum_res;
}

double f_pt2(p_estimator E)
{
	double a, a1, a2, a3, sum_res = 0, tmp;
	a = (E.k1 - 2) / (E.v1 + 2);
	a1 = E.c * E.omega;
	a2 = E.c - a1;
	a3 = E.c_star;
	double head[12] = {
		a1*a1 - a * a,
		a2*a2,
		2 * (a - a1),
		-2 * a2,
		2 * a1*a2,
		1,
		-2 * a,
		a*a,
		2 * (a1 - a),
		2 * a2,
		-2,
		2 * a
	};
	p_task PT;
	PT.lambda1 = E.lambda1;
	PT.lambda2 = E.lambda2;
	PT.k1 = E.k1;
	PT.v1 = E.v1;
	for (int i = 0; i < 12; i++) {
		PT.p = param_list[i][1];
		PT.q = param_list[i][2];
		PT.r = param_list[i][3];
		PT.alpha = param_list[i][4] < 0 ? a3 : 0;
		PT.tau = param_list[i][5] < 0 ? INF : E.tau;
		tmp = param_list[i][0] == 0 ? f_H(PT) : f_J(PT);
		cout << "step: " << i << ", tmp = " << tmp << ", head = " << head[i] << endl;
		sum_res += tmp * head[i];
	}
	return sum_res + E.lambda1 + E.lambda2;
}




