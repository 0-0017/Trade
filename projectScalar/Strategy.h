#pragma once
#include "TechAnal.h"
#include <iostream>
#include <string>


using namespace std;

class Strategy
{
private:
	TechAnal tool;
	string coin;
	double price, accDiff, pip;
	string pair, volume, sl, tp;

public:
	Strategy(int it);
	void calcTakeProfitP(string type);
	string get(string which);
	bool overbought();
	bool oversold();
};
