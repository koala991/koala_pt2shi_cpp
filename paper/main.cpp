#include <iostream>
#include <fstream>
#include <windows.h>
#include <constant.h>
#include <fun0.h>

using namespace std;
extern double INF;
extern double CRIT_VALUE;

//void test0() {
//	// test on switch arr 
//	double arr[3] = { 0, 1, 2 };
//	int i = 0;
//	array_queue_on(arr, 6);
//	while (i < 3)
//	{
//		cout << arr[i++] << endl;
//	}
//}

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

void test3()
{// test f_H
	p_task PP{
		2,
		1,
		1,
		0,
		0.5,
		9,
		1,
		5,
		15
	};
	for (int i = 0; i < 100; i++) 
	{
		cout << f_J(PP) << endl;
	}
}

void test4() {
	// ols
	p_estimator PE{
		0,
		0,
		1,
		10000,
		3,
		7,
		5,
		15
	};
	cout << f_pt2(PE) << endl;
}

void test5() {
	// sr
	p_estimator PE{
		3.0/17.0,
		0,
		1,
		10000,
		1,
		9,
		5,
		15
	};
	cout << f_pt2(PE) << endl;
}

void test6()
{
	double ols, sr;
	p_estimator PE{
	3.0 / 17.0,
	0,
	1,
	10000,
	0,
	0,
	5,
	15
	};
	sr = f_pt2(PE);
	PE.c = 0;
	ols = f_pt2(PE);
	printf("ols:%f\nsr:%f\nratio:%f", ols, sr, sr / ols);
}
int main()
{
	test6();
	system("pause");
	return 0;
}
