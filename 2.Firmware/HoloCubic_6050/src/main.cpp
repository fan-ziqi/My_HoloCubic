#include <Arduino.h>
#include "display.h"
#include "imu.h"
#include "ambient.h"
#include "network.h"
#include "sd_card.h"
#include "lv_port_indev.h"
#include "lv_port_fatfs.h"
#include "lv_cubic_gui.h"
#include "gui_guider.h"
#include "lv_demo_encoder.h"

/*** Component objects ***/
Display screen;
IMU mpu;
SdCard tf;
Network wifi;

lv_ui guider_ui;

void setup()
{
    Serial.begin(115200);

    /*** Init screen ***/
    screen.init();
    screen.setBackLight(0.8);

    /*** Init IMU as input device ***/
    lv_port_indev_init();
    mpu.init();

    /*** Init micro SD-Card ***/
    tf.init();
    lv_fs_if_init();

    String ssid = tf.readFileLine("/wifi.txt", 1);        // line-1 for WiFi ssid
    String password = tf.readFileLine("/wifi.txt", 2);    // line-2 for WiFi password

    /*** Inflate GUI objects ***/
    // lv_holo_cubic_gui();
    // setup_ui(&guider_ui);
    lv_demo_encoder();

    /*** Read WiFi info from SD-Card, then scan & connect WiFi ***/
#if 0
    wifi.init(ssid, password);

    // Change to your BiliBili UID
    Serial.println(wifi.getBilibiliFans("20259914"));
#endif
}

int frame_id = 0;
char buf[100];

void loop()
{
    // run this as often as possible
    screen.routine();

    // 200 means update IMU data every 200ms
    mpu.update(200);

    // int len = sprintf(buf, "S:/Scenes/Holo3D/frame%03d.bin", frame_id++);
    // buf[len] = 0;
    // lv_img_set_src(guider_ui.scenes_canvas, buf);
    // Serial.println(buf);

    // if (frame_id == 138) frame_id = 0;

    //delay(10);
}