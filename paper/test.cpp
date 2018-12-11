#include <iostream>
#include <fstream>
#include <windows.h>
#include <ctime>
#include "Constant.h"
#include "BaseFun.h"
#include "MultiTask.h"

using namespace std;

void test2()
{// test ofstream outf
	ofstream outf;
	outf.open("test.txt");
	outf << INTE_BLOCK << "\n" << CRIT_VALUE << endl;
}

void test13()
{// test on ReturnMseTable
	//const int lens = 202;
	//double r2[] = { 1.0,-1 };
	//double lambda[lens];
	//for (int i = 0; i < lens;i++) lambda[i] = i * 0.05;
	//lambda[lens - 1] = -1;
	double r2[] = { 0.1,0.3,0.5,0.7,0.9,1.0,-1};
	double lambda[] = { 0,1,2,4,6,8,10,15,30,50, 100, 150, -1};
	char szTestType[] = "P"/*"SMAP"*/;
	char szTestFileName[] = "F:/W/paper/result/tautmp2.csv";
	int nRow = GetSchLen(lambda), nCol = GetSchLen(r2);
	double n = 20, k=5;
	double v = n - k;
	double a = (k - 2) / (n - k + 2);
	double a1, a2, a3, c, omega, c_star, tau1, tau2, tau_star;
	a1 = 0*a;
	a2 = 30*a;
	a3 = 30*a;
	c = a1 + a2;
	omega = a1 / c;
	c_star = a3;
	tau1 = (a - a1)*a3*v / (a2-a+a1)/k;
	tau2 = -v / (4 * k)*(2 * a3 - a - a1 - a2) + 1 / (4 * k*k)*sqrt(v*v*k*k*(2*a3 - a - a1 - a2)*(2*a3 - a - a1 - a2) + 8 * v*v*k*k*a3*(a + a1));
	tau_star = (a - a1 - a2) > 0 ? tau2 : min(tau1, tau2);
	tau_star *= 1;
	cout << "tau1=" << tau1 << ", tau2=" << tau2 << ", tau_star=" << tau_star << endl;
	cout <<"a="<<a<< ", a1=" << a1/a << "x, a2=" << a2/a << "x, a3=" << a3/a << "x" <<endl;
	ESTIMATOR_PARAM PE{
		c,
		c_star,
		omega,
		INF,
//		tau_star,
		0,
		0.3,
		k,
		n
	};
	ReturnMseTable(PE, szTestType, lambda, r2, 4, true, szTestFileName);
}


void test15()
{
	double n = 20, k = 5;
	double v = n - k;
	double a = (k - 2) / (n - k + 2);
	double a1, a2, a3, c, omega, c_star, tau1, tau2, tau_star;
	a1 = 0 * a;
	a2 = 100 * a;
	a3 = 100 * a;
	c = a1 + a2;
	omega = a1 / c;
	c_star = a3;
	tau1 = (a - a1)*a3*v / (a2 - a + a1) / k;
	tau2 = -v / (4 * k)*(2 * a3 - a - a1 - a2) + 1 / (4 * k*k)*sqrt(v*v*k*k*(2 * a3 - a - a1 - a2)*(2 * a3 - a - a1 - a2) + 8 * v*v*k*k*a3*(a + a1));
	tau_star = (a - a1 - a2) > 0 ? tau2 : min(tau1, tau2);
	cout << "tau1=" << tau1 << ", tau2=" << tau2 << ", tau_star=" << tau_star << endl;
	cout << "a=" << a << ", a1=" << a1 / a << "x, a2=" << a2 / a << "x, a3=" << a3 / a << "x" << endl;
	ESTIMATOR_PARAM PE{
		c,
		c_star,
		omega,
		0,
		0,
		0.7,
		k,
		n
	};
	cout << GetMinmaxTau(&PE) << endl;
}

void test16()
{
	ofstream fout;
	double n, k;
	double v;
	double zk[] = { 3,4,5,6,8,10,12 };
	double a;
	double a1, a2, a3, c, omega, c_star, tau1, tau2, tau_star;
	fout.open("F:/W/paper/result/minimaxR.txt");
	for (n = 15; n < 35; n += 5) 
	{
		for (int i = 0; i < 7; i++)
		{
			k = zk[i];
			v = n - k;
			a = (k - 2) / (n - k + 2);
			a1 = 0 * a;
			a2 = 30 * a;
			a3 = 30 * a;
			c = a1 + a2;
			omega = a1 / c;
			c_star = a3;

			ESTIMATOR_PARAM PE{
				c,
				c_star,
				omega,
				0,
				0,
				1,
				k,
				n
			};
			cout << "i="<<i<<", n="<<n<<" Started!" <<endl;
			fout << GetMinmaxTau(&PE) << "\t";
			cout << "i=" << i << ", n=" << n << " Completed!" << endl;
		}
		fout << "\n";
	}

}

//
//int main()
//{	
//	double nTime = 0;
//	LARGE_INTEGER nFreq;
//	LARGE_INTEGER nBeginTime;
//	LARGE_INTEGER nEndTime;
//	QueryPerformanceFrequency(&nFreq);
//	QueryPerformanceCounter(&nBeginTime);
//	test16();
//	QueryPerformanceCounter(&nEndTime);
//	nTime = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
//	cout << "Spent: " << nTime * 1000 << "ms" << endl;
//	system("pause");
//	return 0;
//}
