#include <iostream>
#include <algorithm>
#include <math.h>
#include <cstdlib>
#include "Constant.h"
using namespace std;
double FunW(double lambda, double i)
{
	double p1 = exp(-lambda / 2), p2 = pow(lambda / 2, i), p3 = tgamma(i + 1);
	return p2 / p3 * p1;
}

inline double FunInteBase(double t, TASK_PARAM *P, double i, double j)
{
	double p1 = pow(t, P->m_dK1 / 2 + i + P->m_dP - 1);
	double p2 = pow(1 - t, P->m_dV1 / 2 + j - 1 + P->m_dQ);
	double p3 = pow(t + P->m_dAlpha * (1 - t), P->m_dR);
	return p3 ? p1 * p2 / p3 : 0;
}

double FunInte(TASK_PARAM *P, double i, double j)
{
	// int weights[INTE_BLOCK + 1];
	// 1 4 2 4 2 ... 4 2 4 2 1
	// INTE_BLOCK must be even
	int p_id, p_nums = INTE_BLOCK * 2;
	double y_value, sum_res = 0, weight, sep;
	if (P->m_dTau == 0) return 0.0;
	sep = (P->m_dTau == INF ? 1 : P->m_dK1 * P->m_dTau / (P->m_dV1 + P->m_dK1 * P->m_dTau)) / double(p_nums);
	for (p_id = 0; p_id <= p_nums; p_id++)
	{
		y_value = FunInteBase(double(p_id) * sep, P, i, j);
		weight = p_id % 2 ? 4 : 2 - (p_id == 0 || p_id == p_nums);
		sum_res += sep / 3 * weight * y_value;
		//cout << y_value << " - " << weight << endl;
	}
	return sum_res;
}


double FunG(TASK_PARAM *pT, double i, double j)
{
	double g1, g2;
	g1 = tgamma((pT->m_dK1 + pT->m_dV1)/ 2 + i + j + pT->m_dP + pT->m_dQ - pT->m_dR) /
		tgamma(pT->m_dK1 / 2 + i) / tgamma(pT->m_dV1 / 2 + j);
	g2 = FunInte(pT, i, j);
	return g1 * g2;
}

double FunHJ(TASK_PARAM *P, int type)
{
	//assert pow(2, P.p + P.q - P.r)==2
	int i = 0, j = 0;
	double tmp = 0, tmpi = 0, tmpj = 0, tmps = 0, sum_res = 0;
	while (i < MAX_STEP)
	{
		tmpj = 0;
		for (j = 0; j < MAX_STEP; j++) 
		{
			tmp = FunW(P->m_dLambda1, i) * FunW(P->m_dLambda2, j) * FunG(P, i + type, j);
			if (tmp <= CRIT_VALUE && tmp <= tmpj) break;
			tmpj = tmp;
			sum_res += tmp;
		}
		//cout << "Setp:" << i << ", res = " << sum_res - tmps << endl;
		if (sum_res - tmps < CRIT_VALUE && sum_res - tmps <= tmpi) break;
		tmpi = sum_res - tmps;
		tmps = sum_res;
		i++;
	}
	if (i == MAX_STEP || j == MAX_STEP) cout << "Warn: Reach MAX_STEP" << endl;
	return sum_res * (type? P->m_dLambda1: 2);
}

double FunPT2(ESTIMATOR_PARAM E)
{
	double a, a1, a2, a3, sum_res = 0, tmp;
	a = (E.m_dK1 - 2) / (E.m_dN - E.m_dK1 + 2);
	//a = 0;//OLS
	a1 = E.m_dC * E.m_dOmega;
	a2 = E.m_dC - a1;
	a3 = E.m_dCStar;
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
	TASK_PARAM PT;
	PT.m_dLambda1 = E.m_dLambda * E.m_dR2;
	PT.m_dLambda2 = E.m_dLambda - PT.m_dLambda1;
	PT.m_dK1 = E.m_dK1;
	PT.m_dV1 = E.m_dN - E.m_dK1;
	for (int i = 0; i < 12; i++) {
		PT.m_dP = PARAM_LIST[i][1];
		PT.m_dQ = PARAM_LIST[i][2];
		PT.m_dR = PARAM_LIST[i][3];
		PT.m_dAlpha = PARAM_LIST[i][4] < 0 ? a3 : 0;
		PT.m_dTau = PARAM_LIST[i][5] < 0 ? INF : E.m_dTau;
		tmp = FunHJ(&PT, (int)PARAM_LIST[i][0]);
		//cout << "Step: " << i << ", tmp = " << tmp << ", head = " << head[i] << endl;
		sum_res += tmp * head[i];
	}
	return sum_res + E.m_dLambda;
}


