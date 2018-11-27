#include <iostream>
#include <fstream>
#include <windows.h>
#include <ctime>
#include <constant.h>
#include <fun0.h>
#include <do_task_mul.h>

using namespace std;
extern double INF;
extern double CRIT_VALUE;

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
	6.0 / 14.0,
	0,
	1,
	10000,
	0.8,
	7.2,
	8,
	12
	};
	sr = f_pt2(PE);
	PE.c = 0;
	ols = f_pt2(PE);
	printf("ols:%f\nsr:%f\nratio:%f\n", ols, sr, sr / ols);
}

void test7()
{// test on speed
	time_t t1 = time(0);
	for (int i = 1; i < 1000; i++)
	{
		test6();
	}
	cout << "spent:" << time(0) - t1 << endl;
}

void test8()
{// test on mutl process
	double lambda[11] = { 0,1,2,4,6,8,10,15,30,50,100 };
	double r2[6] = { 0.1,0.3,0.5,0.7,0.9,1.0 };
	p_estimator PE{
		//3.0 / 17.0,
		0,
		0,
		1,
		INF,
		0,
		0,
		5,
		15
	};
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			PE.lambda1 = lambda[i] * r2[j];
			PE.lambda2 = lambda[i] - PE.lambda1;
			cout << f_pt2(PE) << " ";
		}
		cout << "\n";
	}
	cout << endl;
}

void test9()
{// test on mutl process
	double lambda[11] = { 0,1,2,4,6,8,10,15,30,50,100 };
	double r2[6] = { 0.1,0.3,0.5,0.7,0.9,1.0 };
	p_estimator PE{
		3.0 / 17.0,
		0,
		1,
		INF,
		0,
		0,
		5,
		15
	};
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			PE.lambda1 = lambda[i] * r2[j];
			PE.lambda2 = lambda[i] - PE.lambda1;
			cout << f_pt2(PE) << " ";
		}
		cout << "\n";
	}
	cout << endl;
}


struct p_mul {
	char tag;
	int nums;
};

void test10(p_mul *para)
{
	for (int i = 0; i < para->nums; i++) {
		cout << "Step " << i << ": " << (para->tag);
	}
	cout << endl;
}

void test11()
{// test on mul 
	p_mul *para = new p_mul();
	para->tag = 'a';
	para->nums = 3;
	cout << "Main Thread start!" << endl;
	HANDLE h = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)test10, para, 1, 0); //创建子线程
	ResumeThread(h);  //启动子线程
	system("pause");
	delete h; //回收子线程资源
}

void test11(double arr[]) 
{
	for (int i = 0; i < 7; i++)
	{
		cout << arr[i] << endl;
	}
}

void test12()
{// test on eval_mse_table
	double lambda[] = { 0,1,2,4,6,8,10,15,30,50,100,-1};
	double r2[] = { 0.1,0.3,0.5,0.7,0.9,1.0,-1};
	p_estimator PE{
		3.0 / 17.0,
		0,
		1,
		INF,
		0,
		0,
		5,
		15
	};
	double *arr = eval_mse_table(PE, 's', lambda, r2);
	for (int tag = 0; tag < 66; tag++)
	{
		cout << *(arr + tag);
		if ((tag+1) % 6) 
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
int main()
{	
	time_t t1 = time(0);
	test12();
	cout << "spent:" << time(0) - t1 << endl;
	system("pause");
	return 0;
}

