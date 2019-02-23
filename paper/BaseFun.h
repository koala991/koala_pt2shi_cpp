#pragma once
double FunW(double lambda, double i);
inline double FunInteBase(double t, TASK_PARAM P, double i, double j);
double FunInte(TASK_PARAM P, double i, double j);
double FunG(TASK_PARAM P, double i, double j);
double FunHJ(TASK_PARAM *P, int type);
double FunPT2(ESTIMATOR_PARAM E);
double LFunW(double lambda, double i);
double LFunG(TASK_PARAM *pT, double i, double j);
