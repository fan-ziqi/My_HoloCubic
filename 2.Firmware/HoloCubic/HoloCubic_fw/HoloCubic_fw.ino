#include "display.h"
#include "imu.h"
#include "network.h"
#include "sd_card.h"
#include "lv_port_indev.h"
#include "lv_cubic_gui.h"
#include "lv_demo_encoder.h"
#include "lv_port_fatfs.h"
#include <ESP32Time.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <WiFiMulti.h>

#include <TFT_eSPI.h>
extern TFT_eSPI tft;
TFT_eSprite clk = TFT_eSprite(&tft);

/*** Component objects **7*/
Display screen;
IMU mpu;
SdCard tf;
Network wifi;
ESP32Time rtc;
long int Time;
int tian;
int zero_flag;
long time1;
const char* AP_SSID  = "ESP32_Config"; //热点名称
const char* AP_PASS  = "12345678";  //密码
#define ROOT_HTML  "<!DOCTYPE html><html><head><title>WIFI Config by lwang</title><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><style type=\"text/css\">.input{display: block; margin-top: 10px;}.input span{width: 100px; float: left; float: left; height: 36px; line-height: 36px;}.input input{height: 30px;width: 200px;}.btn{width: 120px; height: 35px; background-color: #000000; border:0px; color:#ffffff; margin-top:15px; margin-left:100px;}</style><body><form method=\"GET\" action=\"connect\"><label class=\"input\"><span>WiFi SSID</span><input type=\"text\" name=\"ssid\"></label><label class=\"input\"><span>WiFi PASS</span><input type=\"text\"  name=\"pass\"></label><label class=\"input\"><span>City Name</span><input type=\"text\" name=\"cityname\"></label></label><input class=\"btn\" type=\"submit\" name=\"submit\" value=\"Submie\"></form></body></html>"
WebServer server(80);
TimerHandle_t xTimer_rest;
WiFiMulti wifiMulti;
uint8_t count_down;//自动配网倒计时s
uint8_t wifi_flag;
uint8_t resr_count_down = 100;//重启倒计时s
String  key = "SqTlWHkGy6Ig6XcZG"; //填写天气秘钥
String  cityname;		 //显示的城市
String  language = "zh-Hans";
String  unit = "c";
String Time_api = "http://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp";
int Timey;
int Timem;
int Timed;
int Timeh;
int Timemin;

char Timey1[8];
char Timem1[8];
char Timed1[8];
char Timeh1[8];
char Timemin1[8];
char zero[8];
char tiangqii[8];
char cityname1[10];

const char* Timey2 = Timey1;
const char* Timem2 = Timem1;
const char* Timed2 = Timed1;
const char* Timeh2 = Timeh1;
const char* Timemin2 = Timemin1;
const char* zeroo = zero;
const char* tianqq = tiangqii;



void restCallback(TimerHandle_t xTimer );
////####################################################################################################
//// 文件上传SD卡
////####################################################################################################
//void handleFileUpload(){//网络服务处理函数
//  upload_flag = true;//正在进行上传
//  if(server.uri() != "/") return;
//  HTTPUpload& upload = server.upload();
//  String filename;
//  char *file_sd;
//  if(upload.status == UPLOAD_FILE_START){//开启下载上传的文件
//    filename = upload.filename;
//    if(!filename.startsWith("/"))
//    {
//      filename = "/loge"+String(tft_num)+".jpg";//如果文件开头没有/则添加/ 并且对该图片添加计数尾缀
//      tft_num++;//文件数+1
//      EEPROM.write(20,tft_num);//将数据保存
//      EEPROM.commit();
//    }
//    Serial.print("handleFileUpload Name: "); Serial.println(filename);//打印文件名
//
//    SD.remove(filename);
//    fsUploadFile = SD.open(filename, "w");//将上传的文件保存
//    filename = String();
//  } else if(upload.status == UPLOAD_FILE_WRITE){
//    if(fsUploadFile)
//      fsUploadFile.write(upload.buf, upload.currentSize);//将上传文件写入SD卡
//  } else if(upload.status == UPLOAD_FILE_END){
//    if(fsUploadFile)
//      fsUploadFile.close();
//  }
//}
void UpdateWeather(void)
{
  int TianQi = wifi.getTianqi("https://api.seniverse.com/v3/weather/now.json?key=" + key + "&location=" + cityname + "&language=" + language + "&unit=" + unit); //如果要改城市这里也需要修改
  itoa(TianQi, tiangqii, 10);
  Serial.println(tian);
  lv_holo_cubic_gui(tianqq, Timey2, Timem2, Timed2, Timeh2, Timemin2, cityname.c_str(), 1, WeatherCode);
}

void UpdateTime_RTC(void)
{
  Timey = rtc.getYear();
  Timem = rtc.getMonth() + 1;
  Timed = rtc.getDay();
  Timeh = rtc.getHour(true) + 8;
  Timemin = rtc.getMinute();
  itoa(Timey, Timey1, 10);
  itoa(Timem, Timem1, 10);
  itoa(Timed, Timed1, 10);
  itoa(Timeh, Timeh1, 10);
  itoa(Timemin, Timemin1, 10);
  if (Timemin < 10)
  {
    strcpy(zero, "0");
    strcat(zero,Timemin1);
    strcpy(Timemin1,zero);
  }
  lv_holo_cubic_gui(tianqq, Timey2, Timem2, Timed2, Timeh2, Timemin2, cityname.c_str(), 2, WeatherCode);
}
void ap_jiance(boolean result)
{
  if (result)
  {
    IPAddress myIP = WiFi.softAPIP();

    //打印相关信息

    Serial.println("");

    Serial.print("Soft-AP IP address = ");

    Serial.println(myIP);

    Serial.println(String("MAC address = ")  + WiFi.softAPmacAddress().c_str());

    Serial.println("waiting ...");

    xTimer_rest = xTimerCreate("xTimer_rest", 1000 / portTICK_PERIOD_MS, pdTRUE, ( void * ) 0, restCallback);

    xTimerStart( xTimer_rest, 0 );  //开启定时器


  } else {  //开启热点失败

    Serial.println("WiFiAP Failed");

    delay(3000);

    ESP.restart();  //复位esp32

  }
  if (MDNS.begin("esp32")) {

    Serial.println("MDNS responder started");
  }
}
void wifi_cun(String ssid, String pass, String cityname)
{
  tf.deleteFile("/wifi.txt");
  tf.writeFile("/wifi.txt", ssid.c_str(), pass.c_str(), cityname.c_str());
  String ssid_txt = tf.readFileLine("/wifi.txt", 1);    // line-1 for WiFi ssid
  String password_txt = tf.readFileLine("/wifi.txt", 2);  // line-2 for WiFi password
  cityname = tf.readFileLine("/wifi.txt", 3);  // line-3 for tianqi
  Serial.println(ssid_txt);
  Serial.println(password_txt);
  Serial.println(cityname);
  tian = wifi.getTianqi("https://api.seniverse.com/v3/weather/now.json?key=" + key + "&location=" + cityname + "&language=" + language + "&unit=" + unit);
  Serial.println(tian);
  Time = wifi.getTimestamp(Time_api); //nowapi时间API
  Serial.println(Time);
  rtc.setTime(Time);

  char tiangqi[8];
  /*About Time Start*/

  Timey = rtc.getYear();
  Timem = rtc.getMonth() + 1;
  Timed = rtc.getDay();
  Timeh = rtc.getHour(true) + 8;
  Timemin = rtc.getMinute();
  itoa(Timey, Timey1, 10);
  itoa(Timem, Timem1, 10);
  itoa(Timed, Timed1, 10);
  itoa(Timeh, Timeh1, 10);
  itoa(Timemin, Timemin1, 10);
  if (Timemin < 10)
  {
    strcpy(zero, "0");
    strcat(zero,Timemin1);
    strcpy(Timemin1,zero);
  }
  itoa(tian, tiangqi, 10);
  const char* tianq = tiangqi;

  Serial.println("-------------------------------------");
  Serial.println(tianq);
  Serial.println(Timey2);
  Serial.println(Timem2);
  Serial.println(Timed2);
  Serial.println(Timeh2);
  Serial.println(Timemin2);
  Serial.println(cityname);
  Serial.println(0);
  Serial.println(WeatherCode);
  Serial.println("-------------------------------------");
  lv_holo_cubic_gui(tianq, Timey2, Timem2, Timed2, Timeh2, Timemin2, cityname.c_str(), 0, WeatherCode);
}
void wifi_getdata()
{
  tian = wifi.getTianqi("https://api.seniverse.com/v3/weather/now.json?key=" + key + "&location=" + cityname + "&language=" + language + "&unit=" + unit);
  Serial.println(tian);
  Time = wifi.getTimestamp(Time_api); //nowapi时间API
  Serial.println(Time);
  rtc.setTime(Time);
  char tiangqi[8];
  /*About Time Start*/
  Timey = rtc.getYear();
  Timem = rtc.getMonth() + 1;
  Timed = rtc.getDay();
  Timeh = rtc.getHour(true) + 8;
  Timemin = rtc.getMinute();
  itoa(Timey, Timey1, 10);
  itoa(Timem, Timem1, 10);
  itoa(Timed, Timed1, 10);
  itoa(Timeh, Timeh1, 10);
  itoa(Timemin, Timemin1, 10);
  itoa(tian, tiangqi, 10);
  if (Timemin < 10)
  {
    strcpy(zero, "0");
    strcat(zero,Timemin1);
    strcpy(Timemin1,zero);
  }
  const char* tianq = tiangqi;
  lv_holo_cubic_gui(tianq, Timey2, Timem2, Timed2, Timeh2, Timemin2, cityname.c_str(), 0, WeatherCode);
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


void ap_peizhi()
{
  //首页

  server.on("/", []() {

    server.send(200, "text/html", ROOT_HTML);

  });
  //连接
  server.on("/connect", []() {


    server.send(200, "text/html", "<html><body><h1>successd,conning...</h1></body></html>");


    WiFi.softAPdisconnect(true);

    //获取输入的WIFI账户和密码

    String ssid = server.arg("ssid");

    String pass = server.arg("pass");

    cityname = server.arg("cityname");

    Serial.println("WiFi Connect SSID:" + ssid + "  PASS:" + pass);

    //设置为STA模式并连接WIFI

    WiFi.mode(WIFI_STA);

    WiFi.begin(ssid.c_str(), pass.c_str());


    resr_count_down = 100;

    xTimerStop(xTimer_rest, 0);


    uint8_t Connect_time = 0; //用于连接计时，如果长时间连接不成功，复位设备


    while (WiFi.status() != WL_CONNECTED) 
    {  //等待WIFI连接成功

      delay(500);
      Connect_time ++;

      if (Connect_time > 80) {  //长时间连接不上，复位设备

        Serial.println("Connection timeout, check input is correct or try again later!");

        delay(3000);

        ESP.restart();
      }
    }


    wifi_cun(ssid, pass, cityname);
    wifi_flag = 0;
    count_down = 0;
  });
  server.begin();
}
extern int File_num;//相册文件数量
void setup()
{
  Serial.begin(115200);

  /*** Init screen ***/
  screen.init();
  screen.setBackLight(0.6);//0.1

  /*** Init IMU as input device ***/
  lv_port_indev_init();
  mpu.init();

  /*** Init micro SD-Card ***/
  tf.init();
  lv_fs_if_init();
  String ssid = tf.readFileLine("/wifi.txt", 1);		// line-1 for WiFi ssid
  String password = tf.readFileLine("/wifi.txt", 2);	// line-2 for WiFi password
  cityname   = tf.readFileLine("/wifi.txt", 3);  // line-2 for WiFi password
  Serial.println(ssid);
  Serial.println(password);
  Serial.println(cityname);
  tf.listDir("/image", 250);

  /*** Read WiFi info in SD-Card, then scan & connect WiFi ***/
  if (count_down <= 10)
  {
    wifi.init(ssid, password);
    Serial.println("正在自动配网");
    Serial.println(wifi_flag);

    while (WiFi.status() != WL_CONNECTED) 
    {  //等待WIFI连接成功
      for(byte n=0;n<10;n++)
      { //每500毫秒检测一次状态
        loading(50);
      }
    }
    while(loadNum < 194)
    { //让动画走完
      loading(1);
    }

    if (wifi_flag == 0)
    {
      wifi_getdata();
    }
  }
  if (wifi_flag == 1) {
    WiFi.mode(WIFI_AP);//配置为AP模式
    boolean result = WiFi.softAP(AP_SSID, AP_PASS);//开启WIFI热点
    ap_jiance(result);
    ap_peizhi();
  }
  time1 = millis();
}

int flag = 0;
float fade = 0;
extern int tmp;
extern boolean image_flag;
int image_flag_num = 0;
int page = 0;
long previousMillis1 = 0;
long previousMillis2 = 0;
long Weatherinterval = 900000;
long Timeinterval = 10000;
unsigned long previousMillis = 0;
boolean delayState = false;
boolean doDelayMillisTime(long interval , boolean state);
void loop()
{
  server.handleClient();
  // run this as often as possible ��
  while (WiFi.status() == WL_CONNECTED) {  //等待WIFI连接成功

    //WIFI已连接
    screen.routine();

    mpu.update(500);

    delay(10);
    if (image_flag == 1)
    {
      xiangce(File_num);
      lv_scr_load_anim(src, LV_SCR_LOAD_ANIM_NONE , 100, 0  , false);
      if (doDelayMillisTime(3000 , false) == true) {
        image_flag_num++;
        delayState = false;
      }

      if (image_flag_num >= File_num)
      {
        image_flag_num = 0;
      }
      page = 0;
      tmp = 1;
    }
    else
    {
      image_flag_num = 0;
      if (tmp == 1 & page == 0)
      {
        lv_scr_load_anim(scr2, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 300, false);
        page = 1;
        tmp = 0;
      }
      if (tmp == 1 & page == 1)
      {
        lv_scr_load_anim(scr3, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 300, false);
        page = 2;
        tmp = 0;
      }
      if (tmp == 1 & page == 2)
      {
        lv_scr_load_anim(scr1, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 300, false);
        page = 0;
        tmp = 0;
      }
      if (tmp == 2 & page == 0)
      {
        lv_scr_load_anim(scr3, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 300, false);
        page = 2;
        tmp = 0;
      }
      if (tmp == 2 & page == 1)
      {
        lv_scr_load_anim(scr1, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 300, false);
        page = 0;
        tmp = 0;
      }
      if (tmp == 2 & page == 2)
      {
        lv_scr_load_anim(scr2, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 300, false);
        page = 1;
        tmp = 0;
      }

      if (millis() - time1 > 5000 && !flag)
      {
        flag = 1;
        lv_scr_load_anim(scr1, LV_SCR_LOAD_ANIM_NONE, 10, 0, false);
      }

      if (millis() - previousMillis1 >= Weatherinterval)
      {
        previousMillis1 = millis();
        UpdateWeather();

      }

      if (millis() - previousMillis2 >= Timeinterval)
      {
        previousMillis2 = millis();
        UpdateTime_RTC();
      }
    }
    delay(500);
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    if (wifi_flag == 0)
    {
      Serial.println("成功复位");
      ESP.restart();
    }
  }


}
boolean doDelayMillisTime(long interval , boolean state) {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    state = !state;
  }
  return state;
}
void restCallback(TimerHandle_t xTimer ) {  //长时间不访问WIFI Config 将复位设备

  resr_count_down --;

  Serial.print("resr_count_down: ");

  Serial.println(resr_count_down);

  if (resr_count_down < 1) {

    ESP.restart();

  }

}
