#include <iostream>
#include <fstream>
#include <iomanip>
#include <windows.h>
#include <cmath>
#include "Constant.h"
#include "BaseFun.h"
using namespace std;

HANDLE hmutex;
struct MULTI_PARAM {
	int m_nLenR2;
	int m_nLenL;
	int *m_nID;
	int *m_nDone;
	double *m_dScheLam;
	double *m_dScheR2;
	double *m_dMse;
	ESTIMATOR_PARAM *m_pE;
};

int GetSchLen(double sch[])
{
	int i;
	for (i = 0; sch[i] >= 0; i++);
	return i;
}

void ChangeEstimatorType(ESTIMATOR_PARAM *pE, char type)
{
	switch (type)
	{
	case 'S':
		pE->m_dC = (pE->m_dK1 - 2) / (pE->m_dV1 + 2);
		pE->m_dCStar = 0;
		pE->m_dOmega = 0;
		pE->m_dTau = INF;
		break;
	case 'M':
		pE->m_dC = 1 / pE->m_dV1;
		pE->m_dCStar = 1 / pE->m_dV1;
		pE->m_dOmega = 0;
		pE->m_dTau = INF;
		break;
	case 'A':
		pE->m_dC = pE->m_dK1 / pE->m_dV1;
		pE->m_dCStar = pE->m_dK1 / pE->m_dV1;
		pE->m_dOmega = 0;
		pE->m_dTau = INF;
		break;
	case 'O':
		pE->m_dC = 0;
		pE->m_dCStar = 0;
		pE->m_dOmega = 0;
		pE->m_dTau = INF;
		break;
	case 'P':
		break;
	default:
		break;
	}
}

void DoEvalTask(MULTI_PARAM *M)
{
	int id;
	while (true)
	{
		WaitForSingleObject(hmutex, INFINITE);
		if (*(M->m_nID) >= M->m_nLenR2*M->m_nLenL) {
			ReleaseMutex(hmutex);
			break;
		}
		id = *(M->m_nID);
		*(M->m_nID) += 1;
		ESTIMATOR_PARAM E = *(M->m_pE);
		E.m_dLambda = *(M->m_dScheLam + id / M->m_nLenR2);
		E.m_dR2 = *(M->m_dScheR2 + id % M->m_nLenR2);
		ReleaseMutex(hmutex);
		*(M->m_dMse + id) = FunPT2(E);
		WaitForSingleObject(hmutex, INFINITE);
		*(M->m_nDone) += 1;
		ReleaseMutex(hmutex);
	}
}

double *EvalMseTable(ESTIMATOR_PARAM E, char type, double sch_l[], double sch_r[],int nThread)
{   // can not use end()-begin() in function, so use guard. Must be set in begining.
	// 1D array to store result, format n_lambda*n_r2. 
	int nL = GetSchLen(sch_l), nR = GetSchLen(sch_r);
	int task_id = 0, nCount = 0;
	double *pdMse = new double[nL*nR];
	ChangeEstimatorType(&E, type);
	MULTI_PARAM *pM = new MULTI_PARAM();
	pM->m_nLenL = nL;
	pM->m_nLenR2 = nR;
	pM->m_dScheLam = sch_l;
	pM->m_dScheR2 = sch_r;
	pM->m_pE = &E;
	pM->m_dMse = pdMse;
	pM->m_nID = &task_id;
	pM->m_nDone = &nCount;
	hmutex = CreateMutex(NULL, false, NULL);
	for (int i = 0; i < nThread; i++) {
		HANDLE h1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DoEvalTask, pM, 1, 0);
		ResumeThread(h1);
		CloseHandle(h1);
	}
	while (nCount < nL*nR)
	{
		Sleep(100);
		printf("Estimator: %c, %d / %d tasks is done!\r", type, task_id, nL*nR);
	}
	cout << endl;
	delete pM;
	pM = NULL;
	return pdMse;
}

void ReturnMseTable(ESTIMATOR_PARAM E, char szScheT[], double dScheL[], double dScheR2[], int nThread, bool bSave, char szFileName[])
{
	int nT = strlen(szScheT), nL = GetSchLen(dScheL), nR = GetSchLen(dScheR2);
	double *pdTmp, *pdOls;
	ofstream fout;
	if (bSave) fout.open(szFileName);
	pdOls = EvalMseTable(E, 'O', dScheL, dScheR2, nThread);
	for (int i = 0; i < nT; i++)
	{
		pdTmp = EvalMseTable(E, szScheT[i], dScheL, dScheR2, nThread);
		//printf("Relative MSE table of %c is:\n", szScheT[i]);
		cout << ">>> MSE of estimator:"<<szScheT[i];
		if (szScheT[i] == 'P') cout << ",c=" << E.m_dC << ",c_star=" << E.m_dCStar << ",omega=" << E.m_dOmega << ",tau=" << E.m_dTau;
		cout << ",k1=" << E.m_dK1 << ",v1=" << E.m_dV1 << "\n";
		cout << "Lam\\R2";
		for (int j = 0; j < nR; j++) cout << "\t" << dScheR2[j];
		cout << "\n";
		for (int nTag = 0; nTag < nL*nR; nTag++)
		{
			if (!(nTag % nR)) cout << dScheL[nTag / nR] << "\t";
			cout << fixed << setprecision(5) << *(pdTmp + nTag) / *(pdOls + nTag);
			cout.unsetf(ios::fixed);
			cout << ((nTag + 1) % nR ? "\t" : "\n");
		}
		cout << "\n\n";
		if (bSave) 
		{
			fout << ">>> MSE of estimator:" << szScheT[i];
			if (szScheT[i] == 'P') fout << ",c=" << E.m_dC << ",c_star=" << E.m_dCStar << ",omega=" << E.m_dOmega << ",tau=" << E.m_dTau;
			fout << ",k1=" << E.m_dK1 << ",v1=" << E.m_dV1 << "\n";
			fout << "Lam\\R2";
			for (int j = 0; j < nR; j++) fout << "," << dScheR2[j];
			fout << "\n";
			for (int nTag = 0; nTag < nL*nR; nTag++)
			{
				if (!(nTag % nR)) fout << dScheL[nTag / nR] << ",";
				fout << *(pdTmp + nTag) / *(pdOls + nTag);
				fout << ((nTag+1) % nR ? "," : "\n");
			}
			fout << "\n\n";
		}
		delete pdTmp;
	}
}


double GetRegret(ESTIMATOR_PARAM *pE, int nType)
{
	ESTIMATOR_PARAM OE = *pE, CE = *pE;
	ChangeEstimatorType(&OE, 'O');
	CE.m_dTau = nType ? INF : 0;
	return ((FunPT2(*pE) - FunPT2(CE)) / FunPT2(OE));
}

//ESTIMATOR_PARAM *pCE
double GetMaxRegret(ESTIMATOR_PARAM *pE, int nType)
{	
	double dRangeX[2] = { 0, MAX_LAMBDA };
	double dRangeY[2];
	double dFourX[4], dFourY[4];
	double dMaxRegret = 0, dTmpMax = -1;
	bool bInc[3];
	int nInd, nStep = 0;
	pE->m_dLambda = 0; dRangeY[0] = GetRegret(pE, nType);
	pE->m_dLambda = MAX_LAMBDA; dRangeY[1] = GetRegret(pE, nType);
	while (abs(dRangeY[0] - dRangeY[1]) > CRIT_REGRET && abs(dRangeX[0] - dRangeX[1]) > CRIT_LAMBDA && nStep < MAX_STEP)
	{
		for (int i = 0; i < 4; i++) dFourX[i] = double(3 - i)*dRangeX[0] / 3 + double(i)*dRangeX[1] / 3;
		dFourY[0] = dRangeY[0], dFourY[3] = dRangeY[1];
		for (int i = 1; i < 3; i++)
		{
			pE->m_dLambda = dFourX[i];
			dFourY[i] = GetRegret(pE, nType);
		}
		for (int i = 0; i < 3; i++) bInc[i] = dFourY[i + 1] > dFourY[i];
		for (nInd = 3; nInd > 0 && !bInc[--nInd];);
		dRangeX[0] = dFourX[nInd], dRangeY[0] = dFourY[nInd];
		for (nInd = 0; nInd < 3 && bInc[nInd];nInd++);
		dRangeX[1] = dFourX[min(nInd + 1, 3)], dRangeY[1] = dFourY[min(nInd + 1, 3)];
		nStep++;
		cout << "Range of lambda is [" << dRangeX[0] << "," << dRangeX[1] << "]\n";
		cout << "Range of Regret is [" << dRangeY[0] << "," << dRangeY[1] << "]\n";
	}
	cout << "Stop at " << nStep << " turns.\n";
	cout << "Range of lambda is [" << dRangeX[0] << "," << dRangeX[1] << "]\n";
	cout << "Range of Regret is [" << dRangeY[0] << "," << dRangeY[1] << "]\n";
	return max(dRangeY[0], dRangeY[1]);
}

double GetMinmaxTau()
{
	return 0;
}
