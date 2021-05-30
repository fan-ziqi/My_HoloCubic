/*
 * ***********************************************
 * 简易太空人时钟代码 
 * 作者抖音号：d88880
 * 欢迎随意传播修改，但请保留本段注释，且不要用于商业目的。
 * ***********************************************
 * 程序硬件介绍 nodemcu + LCD屏幕（ST7789）
 * 
 * 自动根据当前IP获取相应天气
 * 代码有点烂凑合着看
 * 时间部分使用NTP同步阿里云服务器时间
 * 天气部分从中国天气网上扒下来的，具体是否能长期使用无法确定
 * 
 * 屏幕使用的中景园1.54寸240分辨率全彩屏幕，7、8、10针脚的屏应该都可以，具体请自己参考引脚定义
 * 搞废了别找我就好，有问题可以在抖音评论区留言
 * 
 */
#include "ArduinoJson.h"

#include <TimeLib.h>
#include <WiFi.h>
#include <HTTPClient.h>
//#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
//#include <ESP8266HTTPClient.h>
#define LCD_BL_PIN 5
#define LCD_BL_PWM_CHANNEL 0

//---------------修改此处""内的信息--------------------
const char ssid[] = "77's";        //WIFI名称 修改这2个就可以了
const char pass[] = "123456789";    //WIFI密码
//----------------------------------------------------

#include "font/ZdyLwFont_20.h"
#include "font/FxLED_32.h"

#include "img/pangzi/i0.h"
#include "img/pangzi/i1.h"
#include "img/pangzi/i2.h"
#include "img/pangzi/i3.h"
#include "img/pangzi/i4.h"
#include "img/pangzi/i5.h"
#include "img/pangzi/i6.h"
#include "img/pangzi/i7.h"
#include "img/pangzi/i8.h"
#include "img/pangzi/i9.h"

#include "img/temperature.h"
#include "img/humidity.h"
#include "img/watch_top.h"
#include "img/watch_bottom.h"
#include "img/misaka.h"

#include <TFT_eSPI.h> 
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  // 引脚请自行配置tft_espi库中的 User_Setup.h文件
TFT_eSprite clk = TFT_eSprite(&tft);



#include <TJpg_Decoder.h>

uint32_t targetTime = 0;   
byte omm = 99;
boolean initial = 1;
byte xcolon = 0;
unsigned int colour = 0;

uint16_t bgColor = 0x0000;
String cityCode = "101210101";  //天气城市代码

//NTP服务器
static const char ntpServerName[] = "ntp6.aliyun.com";
const int timeZone = 8;     //东八区


WiFiUDP Udp;
unsigned int localPort = 8000;
float duty=0;
time_t getNtpTime();
void digitalClockDisplay();
void printDigits(int digits);
String num2str(int digits);
void sendNTPpacket(IPAddress &address);


bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
  if ( y >= tft.height() ) return 0;
  tft.pushImage(x, y, w, h, bitmap);
  // Return 1 to decode next block
  return 1;
}

byte loadNum = 6;
void loading(byte delayTime){
  clk.setColorDepth(8);
  
  clk.createSprite(200, 50);
  clk.fillSprite(0x0000);

  clk.drawRoundRect(0,0,200,16,8,0xFFFF);
  clk.fillRoundRect(3,3,loadNum,10,5,0xFFFF);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_GREEN, 0x0000); 
  clk.drawString("Connecting to WiFi",100,40,2);
  clk.pushSprite(20,110);
  clk.deleteSprite();
  loadNum += 1;
  if(loadNum>=194){
    loadNum = 194;
  }
  delay(delayTime);
}

void setup()
{
  Serial.begin(9600);

  ledcSetup(LCD_BL_PWM_CHANNEL, 5000, 8);
  ledcAttachPin(LCD_BL_PIN, LCD_BL_PWM_CHANNEL);
  duty = constrain(0.5, 0, 1);          //LCD亮度，大了小心电源芯片发热
  duty = 1 - duty;
  ledcWrite(LCD_BL_PWM_CHANNEL, (int)(duty * 255));
  tft.begin(); /* TFT init */
  tft.setRotation(4); /* mirror */
//  tft.init();
//  tft.setRotation(0);
  tft.fillScreen(0x0000);
  tft.setTextColor(TFT_BLACK, bgColor);

  targetTime = millis() + 1000; 

  Serial.print("正在连接WIFI ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    for(byte n=0;n<10;n++){ //每500毫秒检测一次状态
      loading(50);
    }
  }
  while(loadNum < 194){ //让动画走完
    loading(1);
  }

  Serial.print("本地IP： ");
  Serial.println(WiFi.localIP());
  //Serial.println("启动UDP");
  Udp.begin(localPort);
  //Serial.print("端口号: ");
  //Serial.println(Udp.localPort());
  //Serial.println("等待同步...");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);

  
  TJpgDec.setJpgScale(1);
  TJpgDec.setSwapBytes(true);
  TJpgDec.setCallback(tft_output);

  TJpgDec.drawJpg(0,0,misaka, sizeof(misaka));
  delay(1000); 
  
  TJpgDec.drawJpg(0,0,watchtop, sizeof(watchtop));
  TJpgDec.drawJpg(0,220,watchbottom, sizeof(watchbottom));

  
  //绘制一个视口
  tft.setViewport(0, 20, 240, 200);
  tft.fillScreen(0x0000);
  tft.fillRoundRect(0,0,240,200,5,bgColor);//实心圆角矩形
  //tft.resetViewport();

  //绘制线框
  tft.drawFastHLine(0,34,240,TFT_WHITE);
  
  tft.drawFastVLine(150,0,34,TFT_WHITE);
  
  tft.drawFastHLine(0,166,240,TFT_WHITE);
  
  tft.drawFastVLine(60,166,34,TFT_WHITE);
  tft.drawFastVLine(160,166,34,TFT_WHITE);

  getCityCode();  //获取城市代码
  
  TJpgDec.drawJpg(161,171,temperature, sizeof(temperature));  //温度图标
  TJpgDec.drawJpg(159,130,humidity, sizeof(humidity));  //湿度图标
  
}

time_t prevDisplay = 0; // 显示时间
unsigned long weaterTime = 0;

void loop(){
  
  if (now() != prevDisplay) {
    prevDisplay = now();
    digitalClockDisplay();
  }

  
  if(millis() - weaterTime > 300000){ //5分钟更新一次天气
    weaterTime = millis();
    getCityWeater();
  }
  scrollBanner();
  imgAnim();
}


// 发送HTTP请求并且将服务器响应通过串口输出
void getCityCode(){
 String URL = "http://wgeo.weather.com.cn/ip/?_="+String(now());
  //创建 HTTPClient 对象
  HTTPClient httpClient;
 
  //配置请求地址。此处也可以不使用端口号和PATH而单纯的
  httpClient.begin(URL); 
  
  //设置请求头中的User-Agent
  httpClient.setUserAgent("Mozilla/5.0 (iPhone; CPU iPhone OS 11_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/11.0 Mobile/15A372 Safari/604.1");
  httpClient.addHeader("Referer", "http://www.weather.com.cn/");
 
  //启动连接并发送HTTP请求
  int httpCode = httpClient.GET();
  Serial.print("Send GET request to URL: ");
  Serial.println(URL);
  
  //如果服务器响应OK则从服务器获取响应体信息并通过串口输出
  if (httpCode == HTTP_CODE_OK) {
    String str = httpClient.getString();
    
    int aa = str.indexOf("id=");
    if(aa>-1){
      //  cityCode = str.substring(aa+4,aa+4+9);
       Serial.println(cityCode); 
       getCityWeater();
    }else{
      Serial.println("获取城市代码失败");  
    }
    
    
  } else {
    Serial.println("请求城市代码错误：");
    Serial.println(httpCode);
  }
 
  //关闭ESP8266与服务器连接
  httpClient.end();
}



// 获取城市天气
void getCityWeater(){
 String URL = "http://d1.weather.com.cn/weather_index/" + cityCode + ".html?_="+String(now());
  //创建 HTTPClient 对象
  HTTPClient httpClient;
  
  httpClient.begin(URL); 
  
  //设置请求头中的User-Agent
  httpClient.setUserAgent("Mozilla/5.0 (iPhone; CPU iPhone OS 11_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/11.0 Mobile/15A372 Safari/604.1");
  httpClient.addHeader("Referer", "http://www.weather.com.cn/");
 
  //启动连接并发送HTTP请求
  int httpCode = httpClient.GET();
  Serial.println("正在获取天气数据");
  Serial.println(URL);
  
  //如果服务器响应OK则从服务器获取响应体信息并通过串口输出
  if (httpCode == HTTP_CODE_OK) {

    String str = httpClient.getString();
    int indexStart = str.indexOf("weatherinfo\":");
    int indexEnd = str.indexOf("};var alarmDZ");

    String jsonCityDZ = str.substring(indexStart+13,indexEnd);
    Serial.println(jsonCityDZ);

    indexStart = str.indexOf("dataSK =");
    indexEnd = str.indexOf(";var dataZS");
    String jsonDataSK = str.substring(indexStart+8,indexEnd);
    Serial.println(jsonDataSK);

    
    indexStart = str.indexOf("\"f\":[");
    indexEnd = str.indexOf(",{\"fa");
    String jsonFC = str.substring(indexStart+5,indexEnd);
    Serial.println(jsonFC);
    
    weaterData(&jsonCityDZ,&jsonDataSK,&jsonFC);
    Serial.println("获取成功");
    
  } else {
    Serial.println("请求城市天气错误：");
    Serial.print(httpCode);
  }
 
  //关闭ESP8266与服务器连接
  httpClient.end();
}


String scrollText[7];
//int scrollTextWidth = 0;
//天气信息写到屏幕上
void weaterData(String *cityDZ,String *dataSK,String *dataFC){
  
  DynamicJsonDocument doc(512);
  deserializeJson(doc, *dataSK);
  JsonObject sk = doc.as<JsonObject>();

  //TFT_eSprite clkb = TFT_eSprite(&tft);
  
  /***绘制相关文字***/
  clk.setColorDepth(8);
  clk.loadFont(ZdyLwFont_20);
  
  //温度
  clk.createSprite(54, 32); 
  clk.fillSprite(bgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_WHITE, bgColor); 
  clk.drawString(sk["temp"].as<String>()+"℃",27,16);
  clk.pushSprite(185,168);
  clk.deleteSprite();

  //城市名称
  clk.createSprite(88, 32); 
  clk.fillSprite(bgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_WHITE, bgColor); 
  clk.drawString(sk["cityname"].as<String>(),44,16);
  clk.pushSprite(151,1);
  clk.deleteSprite();
  
  //PM2.5空气指数
  uint16_t pm25BgColor = tft.color565(156,202,127);//优
  String aqiTxt = "优";
  int pm25V = sk["aqi"];
  if(pm25V>200){
    pm25BgColor = tft.color565(136,11,32);//重度
    aqiTxt = "重度";
  }else if(pm25V>150){
    pm25BgColor = tft.color565(186,55,121);//中度
    aqiTxt = "中度";
  }else if(pm25V>100){
    pm25BgColor = tft.color565(242,159,57);//轻
    aqiTxt = "轻度";
  }else if(pm25V>50){
    pm25BgColor = tft.color565(247,219,100);//良
    aqiTxt = "良";
  }
  clk.createSprite(50, 24); 
  clk.fillSprite(bgColor);
  clk.fillRoundRect(0,0,50,24,4,pm25BgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(0x0000); 
  clk.drawString(aqiTxt,25,13);
  clk.pushSprite(5,130);
  clk.deleteSprite();

  //湿度
  clk.createSprite(56, 24); 
  clk.fillSprite(bgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_WHITE, bgColor); 
  clk.drawString(sk["SD"].as<String>(),28,13);
  //clk.drawString("100%",28,13);
  clk.pushSprite(180,130);
  clk.deleteSprite();

  
  scrollText[0] = "实时天气 "+sk["weather"].as<String>();
  scrollText[1] = "空气质量 "+aqiTxt;
  scrollText[2] = "风向 "+sk["WD"].as<String>()+sk["WS"].as<String>();
  
  //左上角滚动字幕
  //解析第二段JSON
  deserializeJson(doc, *cityDZ);
  JsonObject dz = doc.as<JsonObject>();
  //Serial.println(sk["ws"].as<String>());
  //横向滚动方式
  //String aa = "今日天气:" + dz["weather"].as<String>() + "，温度:最低" + dz["tempn"].as<String>() + "，最高" + dz["temp"].as<String>() + " 空气质量:" + aqiTxt + "，风向:" + dz["wd"].as<String>() + dz["ws"].as<String>();
  //scrollTextWidth = clk.textWidth(scrollText);
  //Serial.println(aa);
  scrollText[3] = "今日"+dz["weather"].as<String>();
  
  deserializeJson(doc, *dataFC);
  JsonObject fc = doc.as<JsonObject>();
  
  scrollText[4] = "最低温度"+fc["fd"].as<String>()+"℃";
  scrollText[5] = "最高温度"+fc["fc"].as<String>()+"℃";
  // scrollText[6] = "我们在一起     天了";
  
  //Serial.println(scrollText[0]);
  
  clk.unloadFont();

}

int currentIndex = 0;
int prevTime = 0;
TFT_eSprite clkb = TFT_eSprite(&tft);

void scrollBanner(){
  if(millis() - prevTime > 2500){ //2.5秒切换一次

    if(scrollText[currentIndex]){
  
      clkb.setColorDepth(8);
      clkb.loadFont(ZdyLwFont_20);
      
      for(int pos = 24; pos>0 ; pos--){
        scrollTxt(pos);
      }
      
      clkb.deleteSprite();
      clkb.unloadFont();
  
      if(currentIndex>=6){
        currentIndex = 0;  //回第一个
      }else{
        currentIndex += 1;  //准备切换到下一个  
      }
      //Serial.println(currentIndex);
      
    }
    prevTime = millis();
  }
}

void scrollTxt(int pos){
    clkb.createSprite(148, 24); 
    clkb.fillSprite(bgColor);
    clkb.setTextWrap(false);
    clkb.setTextDatum(CC_DATUM);
    clkb.setTextColor(TFT_WHITE, bgColor); 
    clkb.drawString(scrollText[currentIndex],74,pos+12);
    clkb.pushSprite(2,4);
}

void imgAnim(){
  int x=80,y=94,dt=30;//瘦子版dt=10毫秒 胖子30较为合适

  TJpgDec.drawJpg(x,y,i0, sizeof(i0));
  delay(dt);
  TJpgDec.drawJpg(x,y,i1, sizeof(i1));
  delay(dt);
  TJpgDec.drawJpg(x,y,i2, sizeof(i2));
  delay(dt);
  TJpgDec.drawJpg(x,y,i3, sizeof(i3));
  delay(dt);  
  TJpgDec.drawJpg(x,y,i4, sizeof(i4));
  delay(dt);  
  TJpgDec.drawJpg(x,y,i5, sizeof(i5));
  delay(dt);  
  TJpgDec.drawJpg(x,y,i6, sizeof(i6));
  delay(dt);  
  TJpgDec.drawJpg(x,y,i7, sizeof(i7));
  delay(dt);  
  TJpgDec.drawJpg(x,y,i8, sizeof(i8));
  delay(dt);  
  TJpgDec.drawJpg(x,y,i9, sizeof(i9));
  delay(dt);  

}

void digitalClockDisplay()
{
  
  clk.setColorDepth(8);

  /***中间时间区***/
  //时分
  clk.createSprite(140, 48);
  clk.fillSprite(bgColor);
  //clk.loadFont(FxLED_48);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_WHITE, bgColor);
  clk.drawString(hourMinute(),70,24,7); //绘制时和分
  //clk.unloadFont();
  clk.pushSprite(28,40);
  clk.deleteSprite();
  
  //秒
  clk.createSprite(40, 32);
  clk.fillSprite(bgColor);
  
  clk.loadFont(FxLED_32);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_WHITE, bgColor); 
  clk.drawString(num2str(second()),20,16);
  
  clk.unloadFont();
  clk.pushSprite(170,60);
  clk.deleteSprite();
  /***中间时间区***/

  /***底部***/
  clk.loadFont(ZdyLwFont_20);
  clk.createSprite(58, 32);
  clk.fillSprite(bgColor);

  //星期
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_WHITE, bgColor);
  clk.drawString(week(),29,16);
  clk.pushSprite(1,168);
  clk.deleteSprite();
  
  //月日
  clk.createSprite(98, 32);
  clk.fillSprite(bgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_WHITE, bgColor);  
  clk.drawString(monthDay(),49,16);
  clk.pushSprite(61,168);
  clk.deleteSprite();
  
  
  clk.unloadFont();
  /***底部***/
}

//星期
String week(){
  String wk[7] = {"日","一","二","三","四","五","六"};
  String s = "周" + wk[weekday()-1];
  return s;
}

//月日
String monthDay(){
  String s = String(month());
  s = s + "月" + day() + "日";
  return s;
}
//时分
String hourMinute(){
  String s = num2str(hour());
  s = s + ":" + num2str(minute());
  return s;
}

String num2str(int digits)
{
  String s = "";
  if (digits < 10)
    s = s + "0";
  s = s + digits;
  return s;
}

void printDigits(int digits)
{
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP时间在消息的前48字节中
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  //Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  //Serial.print(ntpServerName);
  //Serial.print(": ");
  //Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      //Serial.println(secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR);
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // 无法获取时间时返回0
}

// 向NTP服务器发送请求
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
