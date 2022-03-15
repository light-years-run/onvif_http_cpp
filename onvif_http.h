#pragma once
#include "httplib.h"
#include <string>
using namespace httplib;
using namespace std;
class onvif_http
{
public:
	onvif_http(string& ip, int& port);
	void onvif_http_init(string& username, string password);
	string get_profiletoken();
	string get_snapshot_uri(string& profile);
	string get_snapshot_img(string& uri);
private:
	Client m_client;
	string m_ip;
	string m_snapReq;
};

