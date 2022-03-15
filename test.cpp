#include "onvif_http.h"
#include <stdio.h>
int main()
{
	string ip = "192.168.31.101";
	int port = 80;
	string username = "admin";
	string password = "admin";
	onvif_http onvif(ip, port);
	onvif.onvif_http_init(username, password);

	string token = onvif.get_profiletoken();
	if (token == "")
	{
		printf("can not get profiletoken\n");
		return -1;
	}
	int j = 0;
	time_t startTime = time(0);
	while (1)
	{
		string url = onvif.get_snapshot_uri(token);
		if (url == "")
		{
			printf("can not get snapShotUri\n");
			return -1;
		}
		string img = onvif.get_snapshot_img(url);
		j++;
		sleep(1);
		if (j > 10)
			break;
	}
	time_t endTime = time(0);
	printf("span Time %d\n", (endTime - startTime));
	return 0;
}