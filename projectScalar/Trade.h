/*Trade.h*/
#pragma warning(disable : 4996)
#include <iostream>
#include <iomanip>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <chrono>
#include <cmath>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <curl/curl.h>

using namespace std;
using namespace cpr;
using namespace chrono;
using json = nlohmann::json;

class Trade
{
private:
	string key;
	string secret;
	string timestamp;
	string signature;
	string baseUrl;
	string postData;
	string path;
	json result;
	string pair, volume;

public:
	Trade();
	void TimeStamp();
	void Authenticate();
	void request();
	void openTrades();
	json getResult();
	string getResults();
	void getOderInfo(string pair, string volume);
	void addOrder(string ty, string oType, string tp);
	void status();
	void cancelOrder(string id);
};