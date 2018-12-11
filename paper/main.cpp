#include <iostream>
#include <windows.h>
#include <ctime>
#include "Constant.h"
#include "BaseFun.h"
#include "MultiTask.h"
using namespace std;

int GetRMSETask()
{
	double r2[] = { 0.1,0.3,0.5,0.7,0.9,1.0,-1 };
	double lambda[] = { 0,1,2,4,6,8,10,15,30,50, 100, 150, -1 };
	char szTestType[] = "P"/*"SMAP"*/;
	int nRow = GetSchLen(lambda), nCol = GetSchLen(r2);
	double n, k, v, a, a1, a2, a3, c, omega, c_star, tau1, tau2, tau_star;
	// Set n,k,a1,a2,a3
	k = 5;
	n = 20;
	a = (k - 2) / (n - k + 2);
	a1 = 0 * a;
	a2 = 30 * a;
	a3 = 30 * a;
	// Set FilePath
	char szTestFileName[] = "F:/W/paper/result/rmse_1.csv";
	// Do task
	v = n - k;
	c = a1 + a2;
	omega = a1 / c;
	c_star = a3;
	tau1 = (a - a1)*a3*v / (a2 - a + a1) / k;
	tau2 = -v / (4 * k)*(2 * a3 - a - a1 - a2) + 1 / (4 * k*k)*sqrt(v*v*k*k*(2 * a3 - a - a1 - a2)*(2 * a3 - a - a1 - a2) + 8 * v*v*k*k*a3*(a + a1));
	tau_star = (a - a1 - a2) > 0 ? tau2 : min(tau1, tau2);
	tau_star *= 1;
	cout << "tau1=" << tau1 << ", tau2=" << tau2 << ", tau_star=" << tau_star << endl;
	cout << "a=" << a << ", a1=" << a1 / a << "x, a2=" << a2 / a << "x, a3=" << a3 / a << "x" << endl;
	ESTIMATOR_PARAM PE{
		c,
		c_star,
		omega,
		tau_star,
		0,
		0,
		k,
		n
	};
	ReturnMseTable(PE, szTestType, lambda, r2, 4, true, szTestFileName);
	return 0;
}


int GetMinimaxTuaTask()
{
	double n, k, v, dR2, a, a1, a2, a3, c, omega, c_star;
	// Set n,k,dR2,a1,a2,a3
	k = 5;
	n = 20;
	dR2 = 1.0;
	a = (k - 2) / (n - k + 2);
	a1 = 0 * a;
	a2 = 30 * a;
	a3 = 30 * a;
	// Do task
	v = n - k;
	c = a1 + a2;
	omega = a1 / c;
	c_star = a3;
	/*tau1 = (a - a1)*a3*v / (a2 - a + a1) / k;
	tau2 = -v / (4 * k)*(2 * a3 - a - a1 - a2) + 1 / (4 * k*k)*sqrt(v*v*k*k*(2 * a3 - a - a1 - a2)*(2 * a3 - a - a1 - a2) + 8 * v*v*k*k*a3*(a + a1));
	tau_star = (a - a1 - a2) > 0 ? tau2 : min(tau1, tau2);*/
	ESTIMATOR_PARAM PE{
		c,
		c_star,
		omega,
		0,
		0,
		dR2,
		k,
		n
	};
	cout << GetMinmaxTau(&PE) << endl;
	return 0;
}

int main()
{
	double nTime = 0;
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBeginTime);
	GetMinimaxTuaTask();
	QueryPerformanceCounter(&nEndTime);
	nTime = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
	cout << "Spent: " << nTime * 1000 << "ms" << endl; 
	system("pause");
	return 0;
}