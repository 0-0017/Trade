#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <cpr/cpr.h>
#include <chrono>
#include <nlohmann/json.hpp>

using namespace std;
using namespace cpr;
using namespace chrono;
using json = nlohmann::json;

class Transaction
{
private:
	string id;
	string database;
	string text;
	string requestBody;
	string block;
	string timestamp;

public:
	Transaction();
	void timeStamp();
	void getReciept(string ty, string pa, string pr);
	void body();
	void requests();
};


