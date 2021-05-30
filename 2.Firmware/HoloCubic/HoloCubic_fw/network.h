#ifndef NETWORK_H
#define NETWORK_H

#include <WiFi.h>
#include <HTTPClient.h>

extern int WeatherCode;
 
class Network
{
private:
	 
public:
	void init(String ssid, String password);
	long int getTimestamp(String url);
	unsigned int getTianqi(String url);
};

#endif
