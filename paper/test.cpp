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
		15
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
	const int lens = 201;
	double r2[] = { 1.0,-1 };
	//double r2[] = { 0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,-1};
	double lambda[] = { 0,1,2,4,6,8,10,15,30,50,100,-1};
	//double lambda[lens];
	//for (int i = 0; i < lens;i++) lambda[i] = i * 0.5;
	//lambda[lens - 1] = -1;
	char szTestType[] = "P"/*"SMAP"*/;
	char szTestFileName[] = "C:\\Users\\xtkoa\\Desktop\\tautmp.csv";
	int nRow = GetSchLen(lambda), nCol = GetSchLen(r2);
	ESTIMATOR_PARAM PE{
		90 / 17.0,
		90 / 17.0,
		0,
		100,
		0,
		0,
		5,
		15
	};
	ReturnMseTable(PE, szTestType, lambda, r2, 4, true, szTestFileName);
}


void test14() 
{
	ESTIMATOR_PARAM PE{
		90 / 17.0,
		90 / 17.0,
		0,
		1,
		0,
		1,
		5,
		15
	};
	cout << GetMaxRegret(&PE, 0) << endl; 
}

int main()
{	
	double nTime = 0;
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBeginTime);
	test14();
	QueryPerformanceCounter(&nEndTime);
	nTime = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
	cout << "Spent: " << nTime * 1000 << "ms" << endl;
	system("pause");
	return 0;
}
