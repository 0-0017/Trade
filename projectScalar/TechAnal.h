#pragma once
#include <iostream>
#include <cpr/cpr.h>
#include <vector>
#include <nlohmann/json.hpp>
#include <algorithm>
#include <thread>
#include <string>
#include <cmath>
#include <numeric>


using namespace std;
using namespace cpr;
using namespace chrono;
using json = nlohmann::json;

class TechAnal
{
private:
	//Coin Management
	vector<string> coin, Pair, olchPa;
	string cCoin, sPair, oPair;

	//vectors for ohcl
	vector<double> open, low, close, high;

	//calc_rsi values
	double rsi;

	//EMA & stoch
	bool emaTrend;
	bool overbought, oversold;
	double ema, kLine;

public:
	TechAnal();
	void coinMgmt(int itt);
	double getPrice();
	void ohlc();
	void calcEMA();
	void stoch();
	void calc_rsi();
	double getD(string which);
	bool getB(string which);
	string getS(string which);
};