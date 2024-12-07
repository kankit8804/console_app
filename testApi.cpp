
// Console app. This a demo how you can connect API to your C++ application Using CURL libraries. 
// I use Deribit test API "https://test.deribit.com/api" for teading in crypto currency.
// hear we can do trade on BTC and ETH. We can get market value of the crypto. 

#define CURL_STATICLIB
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <format>
#include "curl/curl.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"



using namespace std;
using namespace rapidjson;


string access_Token;
vector<string>orderIDArray;

static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp) {
	size_t realsize = size * nmemb;
	((std::string*)userp)->append((char*)contents, realsize);

	return realsize;
}

string parseOrderid(string json) {
	Document document;
	document.Parse(json.c_str());

	if (document.HasParseError())
		return "parse failed";

	if (document.HasMember("result")) {
		if (document["result"].HasMember("order")) {
			orderIDArray.push_back(document["result"]["order"]["order_id"].GetString());
		}
	}

	return "pass";
}

string parseString(string json) {
	Document document;
	document.Parse(json.c_str());
	if (document.HasParseError())
		return "parse failed";


	if (document.HasMember("result")) {
		if (document["result"].HasMember("access_token")) {
			access_Token = document["result"]["access_token"].GetString();
		}
	}

	return "pass";
}

string authenticate(const string& clientId, const string clientSecretKey) {
	curl_global_init(CURL_GLOBAL_ALL);
	CURL* curl = curl_easy_init();

	if (!curl)
		return "";

	string readBuffer;
	CURLcode res;


	struct curl_slist* headers = NULL;
	if (curl) {

		string authURL = "https://test.deribit.com/api/v2/public/auth?client_id=" + clientId + "&client_secret=" + clientSecretKey + "&grant_type=client_credentials";

		curl_easy_setopt(curl, CURLOPT_URL, authURL.c_str());
		
		char header_buffer[550] = {};
		snprintf(header_buffer, sizeof(header_buffer), "application/json");
		headers = curl_slist_append(headers, header_buffer);

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);


		res = curl_easy_perform(curl);
		

		if (res != CURLE_OK) {
			cout << endl << "Authentication failed________" << endl;
		}
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();

	string value = parseString(readBuffer);

	cout << value << "_________" << access_Token << endl;

	return "ACCESS_TOKEN";
}

string getOrderBook(string instrumentName) {
	curl_global_init(CURL_GLOBAL_ALL);
	CURL* curl = curl_easy_init();

	string readBuffer;
	CURLcode res;
	if (curl) {

		string url = "https://test.deribit.com/api/v2/public/get_order_book?instrument_name="+ instrumentName;

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);


		res = curl_easy_perform(curl);


		if (res != CURLE_OK) {
			cout << endl << "get_order_book failed________" << endl;
		}

		cout << readBuffer << endl<< endl ;

		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();

	return "Order_Book";
}

string placeOrder(string tokenName, string instrumentName, int amount, int price, string type) {
	curl_global_init(CURL_GLOBAL_ALL);
	CURL* curl = curl_easy_init();

	string readBuffer;
	CURLcode res;
	struct curl_slist* headers1 = NULL;

	if (curl) {
		string url;
		if (type == "limit") {
			 url = "https://test.deribit.com/api/v2/private/buy?amount=" + to_string(amount) + "&instrument_name=" + instrumentName + "&price=" + to_string(price) + "&type=" + type;
		}
		else {
			url = "https://test.deribit.com/api/v2/private/buy?amount=" + to_string(amount) + "&instrument_name=" + instrumentName + "&type=" + type;
		}

		

		cout << url << endl << endl;
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

		string accessToken = "Authorization: Bearer " + access_Token;

		cout << accessToken << endl << endl;
		headers1 = curl_slist_append(headers1, accessToken.c_str());

		headers1 = curl_slist_append(headers1, "Content-Type: application/json");

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers1);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

		res = curl_easy_perform(curl);


		if (res != CURLE_OK) {
			cout << endl << "Place_Order failed________" << endl;
		}

		cout << readBuffer << endl << endl;

		curl_easy_cleanup(curl);

	}
	curl_global_cleanup();

	parseOrderid(readBuffer);


	return "Place_order";

}

string getEditOrder(int amount, string order_id, int price) {
	curl_global_init(CURL_GLOBAL_ALL);
	CURL* curl = curl_easy_init();

	string readBuffer;
	CURLcode res;
	struct curl_slist* headers1 = NULL;

	if (curl) {
		string url;

		url = "https://test.deribit.com/api/v2/private/buy?amount=" + to_string(amount) + "&order_id" + order_id + "&price=" + to_string(price);
		
		cout << url << endl << endl;
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

		string accessToken = "Authorization: Bearer " + access_Token;

		cout << accessToken << endl << endl;
		headers1 = curl_slist_append(headers1, accessToken.c_str());

		headers1 = curl_slist_append(headers1, "Content-Type: application/json");

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers1);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

		res = curl_easy_perform(curl);


		if (res != CURLE_OK) {
			cout << endl << "get EditOrder failed________" << endl;
		}

		cout << readBuffer << endl << endl;

		curl_easy_cleanup(curl);

	}
	curl_global_cleanup();

	parseOrderid(readBuffer);


	return "Place_order";
}

string  getPosition(string tokenName, string kind) {

	curl_global_init(CURL_GLOBAL_ALL);
	CURL* curl = curl_easy_init();

	string readBuffer;
	CURLcode res;
	struct curl_slist* headers1 = NULL;

	if (curl) {
		string url = "https://test.deribit.com/api/v2/private/get_positions?currency=" + tokenName + "&kind=" + kind;

		cout << url << endl << endl;
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

		string accessToken = "Authorization: Bearer " + access_Token;

		cout << accessToken << endl << endl;
		headers1 = curl_slist_append(headers1, accessToken.c_str());

		headers1 = curl_slist_append(headers1, "Content-Type: application/json");

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers1);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		
		res = curl_easy_perform(curl);

		if (res != CURLE_OK) {
			cout << endl << "get positions failed________" << endl;
		}

		cout << readBuffer << endl << endl;

		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
	return "str";
}

string getOrderCancel(string order_id) {
	curl_global_init(CURL_GLOBAL_ALL);
	CURL* curl = curl_easy_init();

	string readBuffer;
	CURLcode res;
	struct curl_slist* headers1 = NULL;

	if (curl) {
		string url = "https://test.deribit.com/api/v2/private/cancel?order_id=" + order_id;

		cout << url << endl << endl;
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

		string accessToken = "Authorization: Bearer " + access_Token;

		cout << accessToken << endl << endl;
		headers1 = curl_slist_append(headers1, accessToken.c_str());

		headers1 = curl_slist_append(headers1, "Content-Type: application/json");

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers1);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

		res = curl_easy_perform(curl);

		if (res != CURLE_OK) {
			cout << endl << "order Cancel failed________" << endl;
		}

		cout << readBuffer << endl << endl;

		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
	return "str";
}

string getAllOpenOrder() {
	curl_global_init(CURL_GLOBAL_ALL);
	CURL* curl = curl_easy_init();

	string readBuffer;
	CURLcode res;
	struct curl_slist* headers1 = NULL;

	if (curl) {
		string url = "https://test.deribit.com/api/v2/private/get_open_orders";

		cout << url << endl << endl;
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

		string accessToken = "Authorization: Bearer " + access_Token;

		cout << accessToken << endl << endl;
		headers1 = curl_slist_append(headers1, accessToken.c_str());

		headers1 = curl_slist_append(headers1, "Content-Type: application/json");

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers1);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

		res = curl_easy_perform(curl);

		if (res != CURLE_OK) {
			cout << endl << "order history failed________" << endl;
		}

		cout << readBuffer << endl << endl;

		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
	return "str";
}

string selectKind() {
	int opt;
	string kind;
	cout << "Please Select One Option" << endl;

	cout << "1. future" << endl << "2. option" << endl << "3. spot" << endl;
	cin >>opt;

	switch (opt) {
	case 1:
		{
			kind = "future";
			break;
		}
	case 2:
	{
		kind = "option";
		break;
	}
	case 3:
	{
		kind = "spot";
		break;
	}
	}

	return kind;

}

string selectToken() {
	int opt;
	cout << "Please Select One Token: " << endl;

	cout << "1. BTC" << endl << "2. ETH" << endl;
	cin >> opt;
	cout << endl;

	if (opt == 1) {
		return "BTC";
	}


	return "ETH";
}


string selectInstrumentName() {
	string instrumentName;
	cout << "Please Select One option" << endl;

	int opt;
	cout << endl << "1. BTC-PERPETUAL " << endl << "2. ETH-PERPETUAL" << endl;
	cin >> opt;
	if (opt == 1) {
		return  "BTC-PERPETUAL";
	}
	
	return "ETH-PERPETUAL";
}

int main() {

	
	cout << "WELCOME TO DERIBIT TEST PROJECT" << endl<<endl;

	string clientId = "gIEbnF7O";
	cout << "clientId: " << clientId << endl;

	string clientSecretKey="zBMM1J745_Bvf52M2XnjKgx811P7v40Z7lmdn_wqsLE";
	cout << "clientSecretKey: " << clientSecretKey << endl << endl;

	bool mExit = 0;
	while(!mExit){

		cout << "Please Select One Option " << endl;

		cout << "1: Authentication" << endl
			<< "2: Get OrderBook" << endl 
			<< "3: Place Order" << endl 
			<< "4: Cancel Order" << endl 
			<< "5: Modify order" << endl 
			<< "6: Get current Position" << endl
			<< "7. Get Open Order" << endl
			<< "8: Exit" << endl;

		int option;

		cin >> option;
		cout << endl;


		switch (option) {
		case 1:
		{
			authenticate(clientId, clientSecretKey);
			break;
		}
		case 2:
		{
			string instrument_name;
			
			instrument_name = selectInstrumentName();

			getOrderBook(instrument_name);
			break;
		}
		case 3:
		{
			string token;

			token = selectToken();

			string instrument_name = token + "-PERPETUAL";

			int amount;
			cout << "Please Enter the amount: ";
			cin >> amount;

			int market;
			cout << "Please select market type: " << endl << "1. limit" << endl << "2. market" << endl;
			cin >> market;
			cout << endl;
			
			int price = 0; 
			string type = " ";
			if (market == 1) {
				cout << endl << "Please Enter the Price: ";
				cin >> price;

				type = "limit";
			}
			else {
				type = "market";
			}

			placeOrder(token, instrument_name, amount, price, type);

			break;
		}
		case 4:
		{
			if (orderIDArray.size() <= 0) {
				cout << "No order available to cancel" << endl;
				break;
			}
			cout << "Which Order you want to cancel" << endl;
			for (int i = 0; i < orderIDArray.size(); i++) {
				cout << i+1 << ". " << orderIDArray[i] << endl;
			}

			int opt;
			cin >> opt;

			getOrderCancel(orderIDArray[opt-1]);

			break;
		}
		case 5:
		{
			if (orderIDArray.size() <= 0) {
				cout << "No order available to Edit" << endl;
				break;
			}
			cout << "Which Order you want to Edit" << endl;
			for (int i = 0; i < orderIDArray.size(); i++) {
				cout << i + 1 << ". " << orderIDArray[i] << endl;
			}
			int opt;
			cin >> opt;
			cout << endl << "Enter the amount: " << endl;
			int amount;
			cin >> amount;

			cout << endl << "Enter the price: " << endl;
			int price;
			cin >> price;

			getEditOrder(amount, orderIDArray[--opt], price);
			break;
		}
		case 6:
		{
			string tokenName = selectToken();

			string kind = selectKind();
			getPosition(tokenName, kind);
			break;
		}
		case 7:
		{


			getAllOpenOrder();
			break;
		}
		case 8:
		{
			cout << endl << "Exit" << endl << endl;
			mExit = true;
			break;
		}
		default:
			cout << "Wrong selction" << endl << endl;
			break;

		}


	}

	cout << "CLOSE____________" <<  endl;


	return 0;

}