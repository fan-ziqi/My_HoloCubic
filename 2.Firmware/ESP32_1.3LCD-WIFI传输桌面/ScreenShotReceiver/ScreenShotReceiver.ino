#include <esp_wifi.h>

#include "TCPReceiver.h"

#define LCD_BL_PIN 5
#define LCD_BL_PWM_CHANNEL 0

static LGFX lcd;
static TCPReceiver recv;

void setBackLight(float duty)
{
	duty = constrain(duty, 0, 1);
	duty = 1 - duty;
	ledcWrite(LCD_BL_PWM_CHANNEL, (int)(duty * 255));
}

void setup(void)
{
  Serial.begin(115200);
  Serial.flush();

  lcd.begin();
  ledcSetup(LCD_BL_PWM_CHANNEL, 5000, 8);
	ledcAttachPin(LCD_BL_PIN, LCD_BL_PWM_CHANNEL);
  setBackLight(0.9);
//lcd.setColorDepth(24);
  lcd.setRotation(6);
  

  lcd.setFont(&fonts::Font2);

  Serial.println("WiFi begin.");
  lcd.println("WiFi begin.");
  // 記憶しているAPへ接続試行
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin();

  // 接続できるまで10秒待機
  for (int i = 0; WiFi.status() != WL_CONNECTED && i < 100; i++) { delay(100); }

  // 接続できない場合はSmartConfigを起動
  // https://itunes.apple.com/app/id1071176700
  // https://play.google.com/store/apps/details?id=com.cmmakerclub.iot.esptouch
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("SmartConfig start.");
    lcd.println("SmartConfig start.");
    WiFi.mode(WIFI_MODE_APSTA);
    WiFi.beginSmartConfig();

    while (WiFi.status() != WL_CONNECTED) {
      delay(100);
    }
    WiFi.stopSmartConfig();
    WiFi.mode(WIFI_MODE_STA);
  }

  Serial.println(String("IP:") + WiFi.localIP().toString());
  lcd.println(WiFi.localIP().toString());

  recv.setup( &lcd );
}

void loop(void)
{
  recv.loop();
}
