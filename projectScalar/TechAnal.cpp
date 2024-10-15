#include "TechAnal.h"

TechAnal::TechAnal() {
	//all tools needed for Tech analysis
	ema, kLine = 0.0;
	emaTrend = false, overbought = false, oversold = false;
	rsi = 0.0;
	cCoin = "";
	sPair = "";
	oPair = "";
}

void TechAnal::coinMgmt(int itt) {
	int cItterator;
	coin.push_back("bitcoin"), Pair.push_back("XBTUSD"), olchPa.push_back("XXBTZUSD");
	coin.push_back("ethereum"), Pair.push_back("ETHUSD"), olchPa.push_back("XETHZUSD");
	coin.push_back("ripple"), Pair.push_back("XRPUSD"), olchPa.push_back("XXRPZUSD");
	coin.push_back("litecoin"), Pair.push_back("LTCUSD"), olchPa.push_back("XLTCZUSD");
	coin.push_back("cardano"), Pair.push_back("ADAUSD"), olchPa.push_back("ADAUSD");
	coin.push_back("dogecoin"), Pair.push_back("DOGEUSD"), olchPa.push_back("XDGUSD");

	cItterator = itt % 6;
	cCoin = coin[cItterator];
	sPair = Pair[cItterator];
	oPair = olchPa[cItterator];
}

// Helper Function to calculate the highest value in a given range
static double highest(vector<double>& data, int start, int end) {
	return *max_element(data.begin() + start, data.begin() + end);
}

// Helper Function to calculate the lowest value in a given range
static double lowest(vector<double>& data, int start, int end) {
	return *min_element(data.begin() + start, data.begin() + end);
}

// Helper Function
static double average_gain(const std::vector<double>& gains, int period) {
	double sum = std::accumulate(gains.begin(), gains.begin() + period, 0.0);
	return sum / period;
}

// Helper Function
static double average_loss(const std::vector<double>& losses, int period) {
	double sum = std::accumulate(losses.begin(), losses.begin() + period, 0.0);
	return sum / period;
}

// helper Calculate the Simple Moving Average (SMA) of a data vector
static vector<double> sma(const vector<double>& data, int period) {
	vector<double> sma_values(data.size() - period + 1, 0);

	for (size_t i = 0; i <= data.size() - period; ++i) {
		sma_values[i] = std::accumulate(data.begin() + i, data.begin() + i + period, 0.0) / period;
	}

	return sma_values;
}

//getter for currenrt price (JSON)
double TechAnal::getPrice() {
	//variables and json setup
	Response r;
	do {
		r = Get(Url{ "https://api.kraken.com/0/public/Ticker?pair=" + sPair });
		if (r.status_code != 200) {
			this_thread::sleep_for(seconds(62));
		}
	} while (r.status_code != 200);
	json output;
	stringstream(r.text) >> output;

	string pricee = output["result"][oPair]["c"][0];
	double price = stod(pricee);

	return(price);
}

//Gets ohlc for specified currency. Must be Ran for other funcions to work!
void TechAnal::ohlc() {
	vector<json> tempArr;
	vector<string> one, twoo, three, four;
	Response A;
	do {
		A = Get(Url{ "https://api.kraken.com/0/public/OHLC?pair=" + sPair + "&interval=30"});
		if (A.status_code != 200) {
			this_thread::sleep_for(seconds(62));
		}
	} while (A.status_code != 200);

	json olch;
	stringstream(A.text) >> olch;

	//cout << olch["result"]["XXBTZUSD"] << endl;

	for (auto it = olch["result"][oPair].begin(); it != olch["result"][oPair].end(); it++) {
		//std::cout << it.value() << std::endl;
		tempArr.push_back(it.value());
	}

	//gets olch for timeframe
	for (int j = 0; j < tempArr.size(); j++) {
		//turns json to string
		one.push_back((tempArr[j][1]));
		twoo.push_back((tempArr[j][2]));
		three.push_back((tempArr[j][3]));
		four.push_back((tempArr[j][4]));
	}

	//turns string into double
	for (int k = 0; k < three.size(); k++) {
		//turns string to double
		open.push_back(stod(one[k]));
		high.push_back(stod(twoo[k]));
		low.push_back(stod(three[k]));
		close.push_back(stod(four[k]));
	}
}

void TechAnal::calcEMA() {
	size_t period = 50;
	double alpha = 2.0 / (period + 1);
	vector<double> EMA_values(close.size(), 0);

	// Initialize EMA with the simple moving average (SMA) of the first 50 data points
	double sum = 0;
	for (size_t i = 0; i < period; i++) {
		sum += close[i];
	}
	EMA_values[period - 1] = sum / period;

	// Calculate the remaining EMA values using the exponential smoothing formula
	for (size_t i = period; i < close.size(); i++) {
		EMA_values[i] = (close[i] - EMA_values[i - 1]) * alpha + EMA_values[i - 1];
	}


	//overbought / oversold
	ema = EMA_values[close.size() - 1];


	double price = getPrice();
	if (price > ema) {
		emaTrend = true;
	}
	else {
		emaTrend = false;
	}
}

void TechAnal::stoch() {
	//variables
	int period = 14;
	int k_slowing = 3;
	int d_slowing = 3;

	vector<double> K_values, D_values;

	for (size_t i = period - 1; i < high.size(); ++i) {
		double highest_high = highest(high, i - period + 1, i + 1);
		double lowest_low = lowest(low, i - period + 1, i + 1);
		double k_fast = 100 * (close[i] - lowest_low) / (highest_high - lowest_low);

		if (K_values.size() < k_slowing - 1) {
			K_values.push_back(k_fast);
		}
		else {
			double k_slow = (k_fast + K_values[K_values.size() - 1] * (k_slowing - 1)) / k_slowing;
			K_values.push_back(k_slow);

			if (D_values.size() < d_slowing - 1) {
				D_values.push_back(k_slow);
			}
			else {
				double d_slow = (k_slow + D_values[D_values.size() - 1] * (d_slowing - 1)) / d_slowing;
				D_values.push_back(d_slow);
			}
		}
	}

	//current values
	double currK = K_values[K_values.size() - 1];
	double currD = D_values[D_values.size() - 1];
	kLine = currK;

	if (currK > currD) {
		overbought = true;
	}
	else if (currD > currK)
	{
		oversold = true;
	}
	else {
		overbought, oversold = false;
	}
}

//RSI fully-functional -> data validation
void TechAnal::calc_rsi() {
	//variables
	int period = 14;
	vector<double> gains(close.size() - 1);
	vector<double> losses(close.size() - 1);

	// Calculate gains and losses for each price change
	for (size_t i = 1; i < close.size(); ++i) {
		double change = close[i] - close[i - 1];
		gains[i - 1] = std::max<double>(change, 0.0);
		losses[i - 1] = std::max<double>(-change, 0.0);
	}

	// Calculate initial average gain and loss
	double avg_gain = std::accumulate(gains.begin(), gains.begin() + period, 0.0) / period;
	double avg_loss = std::accumulate(losses.begin(), losses.begin() + period, 0.0) / period;

	vector<double> rsi_values(close.size() - period);

	// Calculate the initial RSI value
	double rs = (avg_loss == 0.0) ? 100 : (avg_gain / avg_loss);
	rsi_values[0] = 100 - (100 / (1 + rs));

	// Calculate RSI values for the rest of the data
	for (size_t i = period; i < gains.size(); ++i) {
		avg_gain = (avg_gain * (period - 1) + gains[i]) / period;
		avg_loss = (avg_loss * (period - 1) + losses[i]) / period;

		rs = (avg_loss == 0.0) ? 100 : (avg_gain / avg_loss);
		rsi_values[i - period + 1] = 100 - (100 / (1 + rs));
	}

	// Apply the Simple Moving Average (SMA) smoother to the RSI values
	vector<double> smoothed_rsi = sma(rsi_values, period);

	int curRsi = smoothed_rsi.size() - 1;
	rsi = smoothed_rsi[curRsi];
}

double TechAnal::getD(string which) {
	//colect data

	if (which == "rsi") {
		return rsi;
	}
	else if (which == "stoch") {
		return kLine;
	}
	else if (which == "ema") {
		return ema;
	}
	else {
		return 0;
	}
}

bool TechAnal::getB(string which) {
	if (which == "trend") {
		return emaTrend;
	}
	else if (which == "ovb") {
		return overbought;
	}
	else if (which == "ovs") {
		return oversold;
	}
	else {
		return false;
	}
}

string TechAnal::getS(string which) {
	if (which == "coin") {
		return cCoin;
	}
	else {
		return cCoin;
	}
}
