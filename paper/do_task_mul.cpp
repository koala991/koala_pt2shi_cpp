#include <iostream>
#include <fstream>
#include <windows.h>
#include <constant.h>
#include <fun0.h>
using namespace std;
extern double INF;
HANDLE hmutex;


struct MultiPara {
	int nr;
	int nl;
	double *sch_l;
	double *sch_r;
	int *id;
	double *arr;
	p_estimator *pE;
};

int get_sch_length(double sch[])
{
	int i;
	for (i = 0; sch[i] >= 0; i++);
	return i;
}

void change_estimator_type(p_estimator *pE, char type)
{
	switch (type)
	{
	case 's':
		pE->c = (pE->k1 - 2) / (pE->v1 + 2);
		pE->c_star = 0;
		pE->omega = 0;
		pE->tau = INF;
		break;
	case 'm':
		pE->c = 1 / pE->v1;
		pE->c_star = 1 / pE->v1;
		pE->omega = 0;
		pE->tau = INF;
		break;
	case 'a':
		pE->c = pE->k1 / pE->v1;
		pE->c_star = pE->k1 / pE->v1;
		pE->omega = 0;
		pE->tau = INF;
		break;
	case 'o':
		pE->c = 0;
		pE->c_star = 0;
		pE->omega = 0;
		pE->tau = INF;
		break;
	case 'p':
		break;
	default:
		break;
	}
}

void mse_task(MultiPara *M)
{
	int id;
	while (true)
	{
		WaitForSingleObject(hmutex, INFINITE);
		if (*(M->id) >= M->nr*M->nl) {
			ReleaseMutex(hmutex);
			break;
		}
		id = *(M->id);
		*(M->id) += 1;
		p_estimator E = *(M->pE);
		E.lambda1 = *(M->sch_l + id / M->nr) * *(M->sch_r + id % M->nr);
		E.lambda2 = *(M->sch_l + id / M->nr) - E.lambda1;
		ReleaseMutex(hmutex);
		*(M->arr + id) = f_pt2(E);
		// sent a signal that task if finshed
		if (id == M->nr*M->nl - 1) *(M->id) += 1;
	}
}

//
//void mse_task_b(MultiPara *M)
//{
//	int id;
//	while (true)
//	{
//		WaitForSingleObject(hmutex, INFINITE);
//		if (*(M->id) >= M->nr*M->nl) {
//			ReleaseMutex(hmutex);
//			break;
//		}
//		id = *(M->id);
//		*(M->id) += 1;
//		p_estimator E = *(M->pE);
//		E.lambda1 = *(M->sch_l + id / M->nr) * *(M->sch_r + id % M->nr);
//		E.lambda2 = *(M->sch_l + id / M->nr) - E.lambda1;
//		ReleaseMutex(hmutex);
//		*(M->arr + id) = f_pt2(E);
//		// sent a signal that task if finshed
//		if (id == M->nr*M->nl - 1) *(M->id) += 1;
//	}
//
//
//}

double *eval_mse_table(p_estimator E, char type, double sch_l[], double sch_r[])
{   // can not use end()-begin() in function, so use guard. Must be set in begining.
	// 1D array to store result, format n_lambda*n_r2. 
	int n_l = get_sch_length(sch_l), n_r = get_sch_length(sch_r);
	int task_id = 0;
	double *mse = new double[n_l*n_r];
	change_estimator_type(&E, type);
	MultiPara *pM = new MultiPara();
	pM->nl = n_l;
	pM->nr = n_r;
	pM->sch_l = sch_l;
	pM->sch_r = sch_r;
	pM->pE = &E;
	pM->arr = mse;
	pM->id = &task_id;
	hmutex = CreateMutex(NULL, false, NULL);
	HANDLE h1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)mse_task, pM, 1, 0);
	ResumeThread(h1);
	CloseHandle(h1);
	HANDLE h2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)mse_task, pM, 1, 0);
	ResumeThread(h2);
	CloseHandle(h2);
	HANDLE h3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)mse_task, pM, 1, 0);
	ResumeThread(h3);
	CloseHandle(h3);
	HANDLE h4 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)mse_task, pM, 1, 0);
	ResumeThread(h4);
	CloseHandle(h4);
	while (task_id < n_l*n_r+1)
	{
		Sleep(500);
		printf("Estimator:%c, %d / %d tasks is done!\r", type, task_id, n_l*n_r);
	}
	Sleep(500);
	cout << endl;
	delete pM;
	pM = NULL;
	return mse;
}


void do_table_task(p_estimator E, char sch_t[], double sch_l[], double sch_r[], bool is_save, char _fileName[], bool verbose=true)
{
	;
}

