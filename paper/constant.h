#pragma once
constexpr auto INTE_BLOCK = 500;
constexpr auto MAX_STEP = 500;
constexpr auto MAX_LAMBDA = 50;
constexpr auto MAX_TAU = 50;
constexpr auto CRIT_REGRET = 1e-4;
constexpr auto CRIT_LAMBDA = 1e-6;
constexpr auto CRIT_TAU = 1e-4;
constexpr auto CRIT_VALUE = 1e-12;
constexpr auto INF = 1e10;
constexpr auto LEAST_CHANGE_RATE = 0.25;
constexpr auto VERBOSE = true;
//constexpr auto VERBOSE = false;
constexpr auto RESULT_PATH = "F:/W/paper/result/tmp.csv";
constexpr double PARAM_LIST[12][6] =
{
	{0,0,2,1,0,0},
	{0,1,2,2,-1,0},
	{0,0,1,0,0,0},
	{0,1,1,1,-1,0},
	{0,0,2,1,-1,0},
	{0,1,0,0,0,-1},
	{0,0,1,0,0,-1},
	{0,0,2,1,0,-1},
	{1,0,1,1,0,0},
	{1,0,1,1,-1,0},
	{1,0,0,0,0,-1},
	{1,0,1,1,0,-1}
};
struct TASK_PARAM {
	double m_dP;
	double m_dQ;
	double m_dR;
	double m_dAlpha;
	double m_dTau;
	double m_dLambda1;
	double m_dLambda2;
	double m_dK1;
	double m_dV1;
};

struct ESTIMATOR_PARAM {
	double m_dC;
	double m_dCStar;
	double m_dOmega;
	double m_dTau;
	double m_dLambda;
	double m_dR2;
	double m_dK1;
	double m_dN;
};