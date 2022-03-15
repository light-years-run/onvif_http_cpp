#include "onvif_http.h"

onvif_http::onvif_http(string& ip, int& port) :m_ip(ip),
m_client(ip, port)
{
}

void onvif_http::onvif_http_init(string& username, string password)
{
	m_client.set_digest_auth(username.c_str(), password.c_str());
}

string onvif_http::get_profiletoken()
{
	string reqBody = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
		"<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\">\n"
		"  <s:Body xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\n"
		"    <GetProfiles xmlns=\"http://www.onvif.org/ver20/media/wsdl\" />\n"
		"  </s:Body>\n"
		"</s:Envelope>";
	string m_profile = "";
	auto r = m_client.Post("/onvif/device_service", reqBody.c_str(), reqBody.length(), "Content-Type:text/xml");
	if (r)
	{
		if (r->status == 200)
		{
			string body = r->body;
			//<trt20:Profiles fixed="true" token="protoken_ch0001">
			//parser string find token
			int startPos = body.find("token=\"");
			if (startPos != string::npos)
			{
				startPos += 7;
				int endPos = body.find_first_of("\"", startPos);
				if (endPos != string::npos)
				{
					m_profile = body.substr(startPos, endPos - startPos);
				}
			}
		}
		else
			cout << r->body << endl;
	}
	else
		cout << "get profile error ..." << endl;

	m_snapReq = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
		"<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\" xmlns:tt=\"http://www.onvif.org/ver10/schema\">\n"
		"  <soap:Body>\n"
		"    <trt:GetSnapshotUri>\n"
		"      <trt:ProfileToken>";
	m_snapReq += m_profile;
	m_snapReq += "</trt:ProfileToken>\n"
		"    </trt:GetSnapshotUri>\n"
		"  </soap:Body>\n"
		"</soap:Envelope>";

	return m_profile;
}

string onvif_http::get_snapshot_uri(string& profile)
{
	if (profile == "")
		return "";
	string uri = "";
	auto r = m_client.Post("/onvif/device_service", m_snapReq.c_str(), m_snapReq.length(), "Content-Type:text/xml");
	if (r)
	{
		if (r->status == 200)
		{
			string body = r->body;
			//<tt:Uri>http://192.168.31.101/jpeg/trade_ftp/pic_type00_01.jpg</tt:Uri>
			//parser string find uri
			int startPos = body.find("tt:Uri");
			if (startPos != string::npos)
			{
				startPos += 7;
				int endPos = body.find_first_of("<", startPos);
				if (endPos != string::npos)
				{
					uri = body.substr(startPos, endPos - startPos);
				}
			}
		}
		else
			cout << r->body << endl;
	}
	else
		cout << "get GetSnapshotUri error ..." << endl;
	return uri;
}

string onvif_http::get_snapshot_img(string& uri)
{
	if (uri == "")
		return "";
	int pos = uri.find(m_ip);
	if (pos != string::npos)
	{
		uri = uri.substr(pos + m_ip.length());
	}
	string body = "";
	//½âÎöurl£ºhttp://192.168.31.101/jpeg/trade_ftp/pic_type00_01.jpg
	auto r = m_client.Get(uri.c_str());
	if (r)
	{
		if (r->status == 200)
		{
			body = r->body;
		}
		else
			cout << r->body << endl;
	}
	else
		cout << "get Image error ..." << uri << endl;
	return body;
}
