#include <iostream>
#include <fstream>
#include <constant.h>
using namespace std;
extern double INF;

void save_mse_table(p_estimator E, char type, double sch_l[], double sch_r[])
{
	// s->sr; m-mmse; a->ammse;p->pt2shi
	// save tmp .txt file named s.txt or m.txt
	switch (type) 
	{
	case 's':
		E.c = (E.k1 - 2) / (E.v1 + 2);
		E.c_star = 0;
		E.omega = 0;
		E.tau = INF;
	case 'm':
		;
	default:
		;
	}

		
}
