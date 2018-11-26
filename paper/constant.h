#pragma once
#define INTE_BLOCK 250
struct p_task {
	double p;
	double q;
	double r;
	double alpha;
	double tau;
	double lambda1;
	double lambda2;
	double k1;
	double v1;
	//	double i;
	//	double j;
};
typedef struct p_task *Param;

struct p_estimator {
	double c;
	double c_star;
	double omega;
	double tau;
	double lambda1;
	double lambda2;
	double k1;
	double v1;
};
