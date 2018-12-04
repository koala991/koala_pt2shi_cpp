#include <iostream>
#include <fstream>
#include <windows.h>
#include <ctime>
#include "Constant.h"
#include "BaseFun.h"
#include "MultiTask.h"

using namespace std;

void test1() {
	// test on constant set
	cout << INTE_BLOCK << "\n" << CRIT_VALUE << endl;
}
void test2()
{// test ofstream outf
	ofstream outf;
	outf.open("test.txt");
	outf << INTE_BLOCK << "\n" << CRIT_VALUE << endl;
}

void test12()
{// test on eval_mse_table
	double lambda[] = { 0,1,2,4,6,8,10,15,30,50,100,-1 };
	double r2[] = { 0.1,0.3,0.5,0.7,0.9,1.0,-1 };
	//double lambda[] = { 0,0.1,0.2,0.4,0.8,1,2,4,6,8,10,15,30,50,100,-1};
	//double r2[] = { 0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,-1};
	int nRow = GetSchLen(lambda), nCol = GetSchLen(r2);
	ESTIMATOR_PARAM PE{
		90 / 17.0,
		90 / 17.0,
		0,
		0,
		0,
		0,
		5,
		20
	};
	double *arr = EvalMseTable(PE, 'P', lambda, r2, 1);
	for (int tag = 0; tag < nRow*nCol; tag++)
	{
		cout << *(arr + tag);
		if ((tag+1) % nCol) 
		{
			cout << " ";
		}
		else 
		{
			cout << "\n";
		}
	}
	cout << endl;
}


void test13()
{// test on ReturnMseTable
	//const int lens = 202;
	//double r2[] = { 0.3,-1 };
	//double lambda[lens];
	//for (int i = 0; i < lens;i++) lambda[i] = i * 0.5;
	//lambda[lens - 1] = -1;
	double r2[] = { 0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,-1};
	double lambda[] = { 0,1,2,4,6,8,10,15,30,50,100,-1};
	char szTestType[] = "SP"/*"SMAP"*/;
	char szTestFileName[] = "F:/W/paper/result/tautmp2.csv";
	int nRow = GetSchLen(lambda), nCol = GetSchLen(r2);
	ESTIMATOR_PARAM PE{
		90 / 17.0,
		90 / 17.0,
		0,
		INF,
		0,
		0.3,
		5,
		20
	};
	ReturnMseTable(PE, szTestType, lambda, r2, 4, true, szTestFileName);
}


void test14() 
{
	ESTIMATOR_PARAM PE{
		90 / 17.0,
		90 / 17.0,
		0,
		1.68487,
		0,
		0.3,
		5,
		20
	};
	cout << GetMaxRegret(&PE, 1) << endl; 
}



void test15()
{
	ESTIMATOR_PARAM PE{
		90 / 17.0,
		90 / 17.0,
		0,
		1.68487,
		0,
		0.9,
		5,
		20
	};
	cout << GetMinmaxTau(&PE) << endl;
}


void test16()
{
	double lambda[] = { 0,1,2,4,6,8,10,15,30,50,100,-1 };
	double r2[] = { 0.1,0.3,0.5,0.7,0.9,1.0,-1 };
	//double r2[] = { 1,-1 };
	//double lambda[] = {100,-1};
	char szTestType[] = "P"/*"SMAP"*/;
	char szTestFileName[] = "C:\\Users\\xtkoa\\Desktop\\tautmp1.csv";
	int nRow = GetSchLen(lambda), nCol = GetSchLen(r2);
	ESTIMATOR_PARAM PE{
		90 / 17.0,
		90 / 17.0,
		0,
		2,
		0,
		0,
		5,
		20
	};
	ReturnMseTable(PE, szTestType, lambda, r2, 4, true, szTestFileName);
}

void test17()
{
	TASK_PARAM T = {
	0,
	2,
	1,
	0,
	1.839775,
	50,
	50,
	5,
	15
	};
	cout << FunHJ(&T, 0) << endl;
}


int main()
{	
	double nTime = 0;
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBeginTime);
	test13();
	QueryPerformanceCounter(&nEndTime);
	nTime = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
	cout << "Spent: " << nTime * 1000 << "ms" << endl;
	system("pause");
	return 0;
}
