#include "Trade.h"

#define API_URL "https://api.kraken.com"
#define GET_API_URL(path) API_URL path

Trade::Trade() : result(json::object()) {
	key = "00000000000000000000000000000000000";
	secret = "0000000000000000000000000000000000==";
	postData = "";
	timestamp = "";
	signature = "";
	path = "";
	pair = "", volume = "";
	baseUrl = GET_API_URL("/0/private/Balance");///0/private/OpenOrders

}

static std::vector<char> to_vector(const std::string& str)
{
	return std::vector<char>(str.begin(), str.end());
}

// helper function to decode a base64 string to a vector of bytes:
static std::string b64_decode(const std::vector<char>& data)
{
	// Create a BIO object to wrap the encoded string
	BIO* b64 = BIO_new(BIO_f_base64());
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	BIO* bio = BIO_new_mem_buf(data.data(), data.size());
	bio = BIO_push(b64, bio);

	// Create a buffer to hold the decoded string
	char buffer[512];
	int len;

	// Decode the string and write the result to the output buffer
	std::string decoded;
	while ((len = BIO_read(bio, buffer, sizeof(buffer))) > 0) {
		decoded.append(buffer, len);
	}

	// Clean up the BIO objects
	BIO_free_all(bio);

	return decoded;
}

//helper function to encode a vector of bytes to a base64 string:
static std::string b64_encode(const std::vector<char>& data)
{
	// Create a base64 BIO
	BIO* b64 = BIO_new(BIO_f_base64());
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

	// Create a memory BIO to store the output
	BIO* mem = BIO_new(BIO_s_mem());
	b64 = BIO_push(b64, mem);

	// Encode the input data
	BIO_write(b64, data.data(), data.size());
	BIO_flush(b64);

	// Get the encoded data from the memory BIO
	BUF_MEM* mem_buf = NULL;
	BIO_get_mem_ptr(b64, &mem_buf);

	// Copy the data into a std::string
	std::string output(mem_buf->data, mem_buf->length);

	// Clean up
	BIO_free_all(b64);

	return output;
}

// helper function to compute SHA256:
static std::vector<char> sha256(const std::vector<char>& str) {
	EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
	const EVP_MD* md = EVP_sha256();
	unsigned char hash[EVP_MAX_MD_SIZE];
	unsigned int hash_len;

	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, str.data(), str.size());
	EVP_DigestFinal_ex(mdctx, hash, &hash_len);

	EVP_MD_CTX_free(mdctx);

	return std::vector<char>(hash, hash + hash_len);
}

// helper function to hash with HMAC algorithm:
static std::vector<char> hmac_sha512(const std::vector<char>& data, const std::vector<char>& key) {
	// Set up the HMAC context
	HMAC_CTX* hmac_ctx = HMAC_CTX_new();
	HMAC_Init_ex(hmac_ctx, key.data(), key.size(), EVP_sha512(), NULL);

	// Update the HMAC with the data
	HMAC_Update(hmac_ctx, (const unsigned char*)data.data(), data.size());

	// Finalize the HMAC and get the result
	unsigned char hmac_result[EVP_MAX_MD_SIZE];
	unsigned int hmac_len;
	HMAC_Final(hmac_ctx, hmac_result, &hmac_len);

	std::vector<char> result(hmac_result, hmac_result + hmac_len);

	// Clean up the HMAC context
	HMAC_CTX_free(hmac_ctx);

	return result;
}

void Trade::TimeStamp() {
	//calculate timestamp
	const auto p1 = system_clock::now();
	const auto intTimestamp = duration_cast<milliseconds>(p1.time_since_epoch()).count();
	timestamp = to_string(intTimestamp);
}

void Trade::Authenticate() {

	//setting auth variables
	//path = "/0/private/Balance";
	//postData = "nonce=" + timestamp;// +"&asset=xbt";

	//sha for timestamp + postdata
	string S256 = timestamp + postData;
	std::vector<char> hash = sha256(to_vector(S256));

	std::vector<char> message;
	message.insert(message.end(), path.cbegin(), path.cend());
	message.insert(message.end(), hash.cbegin(), hash.cend());

	signature = b64_encode(hmac_sha512(message, to_vector(b64_decode(to_vector(secret)))));
}

static string removeZeros(string str) {
	int dotIndex = str.find('.');
	if (dotIndex != string::npos) {
		int lastNonZeroIndex = str.size() - 1;
		while (lastNonZeroIndex > dotIndex && str[lastNonZeroIndex] == '0') {
			lastNonZeroIndex--;
		}
		if (lastNonZeroIndex == dotIndex) {
			str.erase(dotIndex, 1);
		}
		else {
			str.erase(lastNonZeroIndex + 1, str.size() - lastNonZeroIndex - 1);
		}
	}
	return str;
}

void Trade::openTrades() {
	//setting auth variables
	TimeStamp();
	baseUrl = GET_API_URL("/0/private/OpenOrders");
	path = "/0/private/OpenOrders";
	postData = "nonce=" + timestamp + "&trades=true";
	Authenticate();
	request();
}

void Trade::getOderInfo(string pair, string volume) {
	this->pair = pair;
	this->volume = volume;
}


void Trade::addOrder(string ty, string oType, string tp) {

	if (result["result"]["open"].empty()) {
		tp = removeZeros(tp);
		TimeStamp();
		baseUrl = GET_API_URL("/0/private/AddOrder");
		path = "/0/private/AddOrder"; //PB
		postData = "nonce=" + timestamp + "&pair=" + pair + "&type=" + ty + "&ordertype=" + oType;
		postData = postData + "&volume=" + volume + "&leverage=3:1";
		postData = postData + "&close[ordertype]=take-profit" + "&close[price]=" + tp;
		Authenticate();
		request();
	}
	else {
		//PEC
		return;
	}

}

void Trade::cancelOrder(string id) {
	//https://api.kraken.com/0/private/CancelOrder
	TimeStamp();
	baseUrl = GET_API_URL("/0/private/CancelOrder");
	path = "/0/private/CancelOrder"; //PB
	postData = "nonce=" + timestamp + "&txid=" + id;
	Authenticate();
	request();
}

void Trade::status() {
	TimeStamp();
	baseUrl = GET_API_URL("/0/private/TradeBalance");
	path = "/0/private/TradeBalance";
	postData = "nonce=" + timestamp + "&asset=ZUSD";
	Authenticate();
	request();
}

json Trade::getResult() {
	return result;
}

string Trade::getResults() {
	string results = result.dump();
	return results;
}

void Trade::request() {
	//variables
	Response r;

	Header header;
	header.insert({ "API-Key", key });
	header.insert({ "API-Sign", signature });
	header.insert({ "User-Agent", "Kraken C++ API Client" });//*
	header.insert({ "Content-Type", "application/x-www-form-urlencoded" });


	r = Post(Url{ baseUrl }, Body{ postData }, Header{ header });

	json output;
	stringstream(r.text) >> output;
	result = output;
	//cout << result << endl;
}
