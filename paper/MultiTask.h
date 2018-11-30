#pragma once
int GetSchLen(double sch[]);
double *EvalMseTable(ESTIMATOR_PARAM E, char type, double sch_l[], double sch_r[], int nThread);
void ReturnMseTable(ESTIMATOR_PARAM E, char szScheT[], double dScheL[], double dScheR2[], int nThread, bool bSave, char szFileName[]);
double GetMaxRegret(ESTIMATOR_PARAM *pE, int nType);