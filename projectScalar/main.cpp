//main new
#include <iostream>
#include <string>
#include "Trade.h"
#include "Strategy.h"
#include "Transaction.h"
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <thread>
#include <chrono>

using namespace std;
using namespace chrono;

int main() {
	//main variables
	//Trade trade;
	json tradeRes; //*
	string id = "";
	double dailyloss = 0.0;

	bool stop = false;
	int cIt = 0;

	while (stop != true) {
		//setup
		Strategy strat(cIt);
		Trade t1;
		t1.status();
		json result = t1.getResult(); //*
		string soma = result["result"]["c"];//*
		double oma = stod(soma);//*
		Transaction tr;

		if (oma <= 0 && dailyloss > -2.0) {//*
			if (strat.overbought()) {
				cout << "True ob\n";
				t1.getOderInfo(strat.get("pair"), strat.get("volume"));
				t1.addOrder("sell", "market", strat.get("tp"));
				tr.getReciept("Sell", strat.get("pair"), strat.get("pri"));
				cout << "Results: " << t1.getResults() << endl;
				tradeRes = t1.getResult();
				id = tradeRes["result"]["txid"][0];
				stop = false;//stop = true;
			}
			else if (strat.oversold()) {
				cout << "True os\n";
				t1.getOderInfo(strat.get("pair"), strat.get("volume"));
				t1.addOrder("buy", "market", strat.get("tp"));
				tr.getReciept("Buy", strat.get("pair"), strat.get("pri"));
				cout << "Results: " << t1.getResults() << endl;
				tradeRes = t1.getResult();
				id = tradeRes["result"]["txid"][0];
				stop = false;//stop = true;
			}
			else {
				cout << "Will Keep Looking...\n";
				cIt++;
				this_thread::sleep_for(seconds(60));
			}
		}
		else {
			string spl = result["result"]["n"];
			double pl = stod(spl);

			if (dailyloss <= -50.0) {
				cout << "Daily Stop Loss Activated\n";
				t1.cancelOrder(id);
				cout << "Results: " << t1.getResults() << endl;
				stop = true;
			}
			else if (pl <= -40.0) {
				dailyloss = dailyloss + pl;
				t1.cancelOrder(id);
				cout << "Stopped Out\n";
				cout << "Results: " << t1.getResults() << endl;
				this_thread::sleep_for(minutes(5));
				stop = false;
			}
			else {
				cout << "Trades Are Active...\n";
				this_thread::sleep_for(minutes(5));
				stop = false;
			}
		}
	}

}