#include "Strategy.h"

Strategy::Strategy(int it) {
	tool.coinMgmt(it);
	tool.ohlc();
	tool.calcEMA();
	tool.stoch();
	tool.calc_rsi();
	price = 0.0;
	pip = 0.0;
	price = tool.getPrice();
	coin = tool.getS("coin");
	accDiff = 0.0;
	pair = "";
	volume = "";
	sl = "", tp = "";

	if (coin == "bitcoin") {
		accDiff = 200.0;
		pip = 2.0;
		pair = "XBTUSD";
		volume = "0.005";
	}
	else if (coin == "ethereum") {
		pip = 0.1;
		accDiff = 15.0;
		pair = "ETHUSD";
		volume = "0.07";
	}
	else if (coin == "ripple") {
		accDiff = 0.0050;
		pip = 0.0001;
		pair = "XRPUSD";
		volume = "300";
	}
	else if (coin == "litecoin") {
		accDiff = 1;
		pip = 0.01;
		pair = "LTCUSD";
		volume = "1.5";
	}
	else if (coin == "cardano") {
		accDiff = 0.0050;
		pip = 0.00004;
		pair = "ADAUSD";
		volume = "375";
	}
	else {
		accDiff = 0.0010;
		pip = 0.00001;
		pair = "DOGEUSD";
		volume = "1900";
	}
}

void Strategy::calcTakeProfitP(string type) {
	double ttp = pip * 300;
	if (type == "buy") {
		double result = (price + ttp);
		tp = to_string(result);
	}
	else {
		double result = (price - ttp);
		tp = to_string(result);
	}
}

string Strategy::get(string which) {
	if (which == "pair") {
		return pair;
	}
	else if (which == "volume") {
		return volume;
	}
	else if (which == "sl") {
		return sl;
	}
	else if (which == "pri") {
		return to_string(price);
	}
	else {
		return tp;
	}
}

bool Strategy::overbought() {
	bool trend = tool.getB("trend");
	bool overbought = tool.getB("ovb");
	bool oversold = tool.getB("ovs");
	double rsi = tool.getD("rsi");
	double kLine = tool.getD("stoch");
	double ema = tool.getD("ema");
	double heavy = (price - ema);

	if (kLine >= 70 && rsi >= 60 && trend == true) {
		if (oversold && heavy >= (pip * 200)) {
			calcTakeProfitP("sell");
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}


bool Strategy::oversold() {
	bool trend = tool.getB("trend");
	bool overbought = tool.getB("ovb");
	bool oversold = tool.getB("ovs");
	double rsi = tool.getD("rsi");
	double kLine = tool.getD("stoch");
	double ema = tool.getD("ema");
	double heavy = (ema - price);

	if (kLine <= 30 && rsi <= 45 && trend == false) {
		if (overbought && heavy >= (pip * 200)) {
			calcTakeProfitP("buy");
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}
