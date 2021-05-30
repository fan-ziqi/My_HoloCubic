#include "network.h"

int WeatherCode=0;
extern uint8_t count_down;
extern uint8_t wifi_flag;
void Network::init(String ssid, String password)
{
	Serial.println("scan start");
	int n = WiFi.scanNetworks();
	Serial.println("scan done");
	if (n == 0)
	{
		Serial.println("no networks found");
	}
	else
	{
		Serial.print(n);
		Serial.println(" networks found");
		for (int i = 0; i < n; ++i)
		{
			Serial.print(i + 1);
			Serial.print(": ");
			Serial.print(WiFi.SSID(i));
			Serial.print(" (");
			Serial.print(WiFi.RSSI(i));
			Serial.print(")");
			Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
			delay(10);
		}
	}
	Serial.println("");
	Serial.print("Connecting: ");
	Serial.print(ssid.c_str());
	Serial.print(" @");
	Serial.println(password.c_str());

	WiFi.begin(ssid.c_str(), password.c_str());
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
    count_down++;
		Serial.println(count_down);
   if(count_down>=10)
   {wifi_flag=1;break;}
	}
	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
}

long int Network::getTimestamp(String url)
{
	String Time = "";
  long long Time1;
	HTTPClient http;
	http.begin(url);

	// start connection and send HTTP headerFFF
	int httpCode = http.GET();

	// httpCode will be negative on error
	if (httpCode > 0)
	{
		// file found at server
		if (httpCode == HTTP_CODE_OK)
		{
      String payload = http.getString();
      int pos = (payload.indexOf("data"));
      Time = payload.substring(pos + 12, payload.length() - 6);
      Serial.println(payload.substring(pos + 12, payload.length() - 6));
		}
	}
	else
	{
		Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
	}
	http.end();
  
	return atoll(Time.c_str());
}

unsigned int Network::getTianqi(String url)
{
	String temperatureCount = "";
  String weatherCount= "";

	HTTPClient https;
	https.begin(url);

	// start connection and send HTTP headerFFF
	int httpCode = https.GET();

	// httpCode will be negative on error
	if (httpCode > 0)
	{
		// file found at server
		if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
		{
			String payload = https.getString();
			Serial.println(httpCode);
			Serial.println(payload);
      int pos1 = (payload.indexOf("code"));  //获取code位置
			int pos2 = (payload.indexOf("temperature")); //获取temperature位置     
      weatherCount= payload.substring(pos1 + 7, pos2 - 3);
			temperatureCount = payload.substring(pos2 + 14, payload.length() - 47);
      Serial.println("weather:"+weatherCount); //得到code的值
      Serial.println("temperature:"+temperatureCount); //得到temperature的值
      WeatherCode=atol(weatherCount.c_str());
		}
	}
	else
	{
		Serial.printf("[HTTP] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
	}
	https.end();

	return atol(temperatureCount.c_str());
}
