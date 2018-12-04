//#include <iostream>
//#include <windows.h>
//#include <ctime>
//#include "Constant.h"
//#include "BaseFun.h"
//#include "MultiTask.h"
//using namespace std;
//
//int main()
//{
//	double lambda[] = { 0,1,2,4,6,8,10,15,30,50,100,-1 };
//	double r2[] = { 0.1,0.3,0.5,0.7,0.9,1.0,-1 };
//	ESTIMATOR_PARAM PE{
//	90 / 17.0,
//	90 / 17.0,
//	0,
//	0.578,
//	0,
//	0,
//	5,
//	20
//	};
//	double nTime = 0;
//	LARGE_INTEGER nFreq;
//	LARGE_INTEGER nBeginTime;
//	LARGE_INTEGER nEndTime;
//	QueryPerformanceFrequency(&nFreq);
//	QueryPerformanceCounter(&nBeginTime);
//	ReturnMseTable(PE, (char *)"SMAP", lambda, r2, 4, true, (char *)"demo.csv");
//	QueryPerformanceCounter(&nEndTime);
//	nTime = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
//	cout << "Spent: " << nTime * 1000 << "ms" << endl; 
//	system("pause");
//	return 0;
//}