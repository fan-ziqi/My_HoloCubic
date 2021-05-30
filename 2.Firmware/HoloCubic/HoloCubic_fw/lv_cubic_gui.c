/*********************
        INCLUDES
 *********************/
#include "lv_cubic_gui.h"
#include "images.h"
#include "lv_port_indev.h"
lv_obj_t* src;
lv_obj_t* img;
lv_obj_t* scr1;
lv_obj_t* scr2;
lv_obj_t* scr3;
//lv_obj_t* label;
lv_obj_t* labe2;
//lv_obj_t* labe3;
lv_obj_t* labe5;
lv_obj_t* labe7;
lv_obj_t* labe9;
lv_obj_t* labe10;
lv_obj_t* labe11;
lv_obj_t* labe12;
lv_obj_t* labe13;
lv_obj_t* labe14;
lv_font_declare(lv_font_montserrat_20);
lv_font_declare(lv_font_montserrat_24);
lv_font_declare(lv_font_montserrat_40);
extern int image_flag_num;
static lv_group_t* g;
static lv_obj_t* tv;
static lv_obj_t* t1;
static lv_obj_t* t2;
static lv_obj_t* t3;
extern int zero_flag;
void xiangce(int File_num)
{
  char buf[20];

  //第一页
  sprintf(buf, "S:/image/%d.bin", image_flag_num);
  lv_img_set_src(img, buf);
  lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);
}
void lv_holo_cubic_gui(const char* weachern, const char* Timey, const char* Timem, const char* Timed, const char* Timeh, const char* Timemin, const char* cityname2, int control, int weathercode)
{
  if (control == 0)
  {
    static lv_style_t default_style;
    static lv_style_t label_style;
    static lv_style_t label_style2;
    static lv_style_t label_style3;
    static lv_style_t label_style4;
    static lv_style_t label_style5;

    lv_style_init(&default_style);
    lv_style_set_bg_color(&default_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_bg_color(&default_style, LV_STATE_PRESSED, LV_COLOR_GRAY);
    lv_style_set_bg_color(&default_style, LV_STATE_FOCUSED, LV_COLOR_BLACK);
    lv_style_set_bg_color(&default_style, LV_STATE_FOCUSED | LV_STATE_PRESSED, lv_color_hex(0xf88));
    lv_obj_add_style(lv_scr_act(), LV_BTN_PART_MAIN, &default_style);

    src = lv_obj_create(NULL, NULL);
    img = lv_img_create(src, NULL);
    
    scr1 = lv_scr_act();//在第一页创建
    lv_obj_t* img1 = lv_img_create(scr1, NULL);
    switch (weathercode) {
      case 0:
        lv_img_set_src(img1, &Sunny);
        break;
      case 1:
        lv_img_set_src(img1, &Clear);
        break;
      case 2:
        lv_img_set_src(img1, "S:/Weather/2.bin");
        break;
      case 3:
        lv_img_set_src(img1,  "S:/Weather/3.bin");
        break;
      case 4:
        lv_img_set_src(img1, &Cloudy);
        break;
      case 5:
        lv_img_set_src(img1, &PartlyCloudy);
        break;
      case 6:
        lv_img_set_src(img1, &PartlyCloudy);
        break;
      case 7:
        lv_img_set_src(img1, &PartlyCloudy);
        break;
      case 8:
        lv_img_set_src(img1, &PartlyCloudy);
        break;
      case 9:
        lv_img_set_src(img1, &Overcast);
        break;
      case 10:
        lv_img_set_src(img1, &ModerateSnow);
        break;
      case 11:
        lv_img_set_src(img1, &Thundershower);
        break;
      case 12:
        lv_img_set_src(img1, "S:/Weather/12.bin");
        break;
      case 13:
        lv_img_set_src(img1, &LightRain);
        break;
      case 14:
        lv_img_set_src(img1, &ModerateRain);
        break;
      case 15:
        lv_img_set_src(img1, &HeavyRain);
        break;
      case 16:
        lv_img_set_src(img1, &HeavyRain);
        break;
      case 17:
        lv_img_set_src(img1, &HeavyRain);
        break;
      case 18:
        lv_img_set_src(img1, &HeavyRain);
        break;
      case 19:
        lv_img_set_src(img1, "S:/Weather/19.bin");
        break;
      case 20:
        lv_img_set_src(img1, &Sleet);
        break;
      case 21:
        lv_img_set_src(img1, &SnowFlurry);
        break;
      case 22:
        lv_img_set_src(img1, &LightSnow);
        break;
      case 23:
        lv_img_set_src(img1, &ModerateSnow);
        break;
      case 24:
        lv_img_set_src(img1, &HeavySnow);
        break;
      case 25:
        lv_img_set_src(img1, &HeavySnow);
        break;
      case 26:
        lv_img_set_src(img1, "S:/Weather/26.bin");
        break;
      case 27:
        lv_img_set_src(img1, "S:/Weather/27.bin");
        break;
      case 28:
        lv_img_set_src(img1, "S:/Weather/28.bin");
        break;
      case 29:
        lv_img_set_src(img1, "S:/Weather/29.bin");
        break;
      case 30:
        lv_img_set_src(img1, "S:/Weather/30.bin");
        break;
      case 31:
        lv_img_set_src(img1, "S:/Weather/31.bin");
        break;
      case 32:
        lv_img_set_src(img1, "S:/Weather/32.bin");
        break;
      case 33:
        lv_img_set_src(img1, "S:/Weather/33.bin");
        break;
      case 34:
        lv_img_set_src(img1, "S:/Weather/34.bin");
        break;
      case 35:
        lv_img_set_src(img1, "S:/Weather/35.bin");
        break;
      case 36:
        lv_img_set_src(img1, "S:/Weather/36.bin");
        break;
      case 37:
        lv_img_set_src(img1, "S:/Weather/37.bin");
        break;
      case 38:
        lv_img_set_src(img1, "S:/Weather/38.bin");
        break;
      case 99:
        lv_img_set_src(img1, "S:/Weather/99.bin");
        break;
      default:
        lv_img_set_src(img1, "S:/Weather/99.bin");
        break;
    }
    lv_obj_align(img1, NULL, LV_ALIGN_OUT_TOP_MID, 0, 160);





    lv_style_init(&label_style);
    lv_style_set_text_opa(&label_style, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_text_color(&label_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&label_style, LV_STATE_DEFAULT, &lv_font_montserrat_24);
    lv_obj_t* label = lv_label_create(scr1, NULL);
    lv_obj_add_style(label, LV_LABEL_PART_MAIN, &label_style);
    lv_label_set_text(label, cityname2);
    lv_obj_align(label, NULL, LV_ALIGN_OUT_BOTTOM_LEFT, 20, -70);


    lv_style_init(&label_style2);
    lv_style_set_text_opa(&label_style2, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_text_color(&label_style2, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&label_style2, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    labe2 = lv_label_create(scr1, NULL);
    lv_obj_add_style(labe2, LV_LABEL_PART_MAIN, &label_style2);
    lv_label_set_text(labe2, weachern);
    lv_obj_align(labe2, label, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

    lv_style_init(&label_style3);
    lv_style_set_text_opa(&label_style3, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_text_color(&label_style3, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&label_style3, LV_STATE_DEFAULT, &lv_font_montserrat_24);
    lv_obj_t* labe3 = lv_label_create(scr1, NULL);
    lv_obj_add_style(labe3, LV_LABEL_PART_MAIN, &label_style3);
    lv_label_set_text(labe3, "°C");
    lv_obj_align(labe3, labe2, LV_ALIGN_OUT_RIGHT_MID, 10, 0);


    // scr2 = lv_obj_create(NULL, NULL);//在第二页创建
    // lv_obj_add_style(scr2, LV_BTN_PART_MAIN, &default_style);

    // lv_obj_t* img4 = lv_img_create(scr2, NULL);
    // lv_img_set_src(img4, &logo);
    // lv_obj_align(img4, NULL, LV_ALIGN_OUT_TOP_MID, 0, 110);

    // lv_obj_t* img3 = lv_img_create(scr2, NULL);
    // lv_img_set_src(img3, &huoj);
    // lv_obj_align(img3, NULL, LV_ALIGN_OUT_BOTTOM_RIGHT, -15, -100);


    // lv_style_init(&label_style);
    // lv_style_set_text_opa(&label_style, LV_STATE_DEFAULT, LV_OPA_COVER);
    // lv_style_set_text_color(&label_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    // lv_style_set_text_font(&label_style, LV_STATE_DEFAULT, &lv_font_montserrat_24);
    // lv_obj_t* labe4 = lv_label_create(scr2, NULL);
    // lv_obj_add_style(labe4, LV_LABEL_PART_MAIN, &label_style);
    // lv_label_set_text(labe4, "- CWEIB -");
    // lv_obj_align(labe4, NULL, LV_ALIGN_OUT_BOTTOM_LEFT, 30, -95);

    /*About Time Start*/
    scr2 = lv_obj_create(NULL, NULL);
    lv_obj_add_style(scr2, LV_BTN_PART_MAIN, &default_style);
    lv_style_init(&label_style5);
    lv_style_set_text_opa(&label_style5, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_text_color(&label_style5, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&label_style5, LV_STATE_DEFAULT, &lv_font_montserrat_40);

    lv_obj_t* img3 = lv_img_create(scr2, NULL);
    lv_img_set_src(img3, &logo);
    lv_obj_align(img3, NULL, LV_ALIGN_OUT_TOP_MID, 0, 120);

    labe5 = lv_label_create(scr2, NULL);
    lv_obj_add_style(labe5, LV_LABEL_PART_MAIN, &label_style5);
    lv_label_set_text(labe5, Timey);
    lv_obj_align(labe5, NULL, LV_ALIGN_OUT_BOTTOM_LEFT, 30, -110);//45

    lv_obj_t* labe6 = lv_label_create(scr2, NULL);
    lv_obj_add_style(labe6, LV_LABEL_PART_MAIN, &label_style5);
    lv_label_set_text(labe6, "-");
    lv_obj_align(labe6, labe5, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    labe7 = lv_label_create(scr2, NULL);
    lv_obj_add_style(labe7, LV_LABEL_PART_MAIN, &label_style5);
    lv_label_set_text(labe7, Timem);
    lv_obj_align(labe7, labe6, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    lv_obj_t* labe8 = lv_label_create(scr2, NULL);
    lv_obj_add_style(labe8, LV_LABEL_PART_MAIN, &label_style5);
    lv_label_set_text(labe8, "-");
    lv_obj_align(labe8, labe7, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    labe9 = lv_label_create(scr2, NULL);
    lv_obj_add_style(labe9, LV_LABEL_PART_MAIN, &label_style5);
    lv_label_set_text(labe9, Timed);
    lv_obj_align(labe9, labe8, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    labe10 = lv_label_create(scr2, NULL);
    lv_obj_add_style(labe10, LV_LABEL_PART_MAIN, &label_style5);
    lv_label_set_text(labe10, Timeh);
    lv_obj_align(labe10, NULL, LV_ALIGN_OUT_BOTTOM_LEFT, 65, -60);//40, -60

    labe11= lv_label_create(scr2, NULL);
    lv_obj_add_style(labe11, LV_LABEL_PART_MAIN, &label_style5);
    lv_label_set_text(labe11, ":");
    lv_obj_align(labe11, labe10, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    labe13 = lv_label_create(scr2, NULL);
    lv_obj_add_style(labe13, LV_LABEL_PART_MAIN, &label_style5);
    lv_label_set_text(labe13, Timemin);
    lv_obj_align(labe13, labe11, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    /*About Time End*/

    scr3 = lv_obj_create(NULL, NULL);
    lv_obj_add_style(scr3, LV_BTN_PART_MAIN, &default_style);
    lv_style_init(&label_style4);
    lv_style_set_text_opa(&label_style4, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_text_color(&label_style4, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&label_style4, LV_STATE_DEFAULT, &lv_font_montserrat_20);

    lv_obj_t* labe14 = lv_label_create(scr3, NULL);
    lv_obj_add_style(labe14, LV_LABEL_PART_MAIN, &label_style4);
    lv_label_set_text_fmt(labe14, "CPU Temp: %d °C", 0);
    lv_obj_set_pos(labe14, 2, 30);

    lv_obj_t* labe15 = lv_label_create(scr3, NULL);
    lv_obj_add_style(labe15, LV_LABEL_PART_MAIN, &label_style4);
    lv_label_set_text_fmt(labe15, "CPU Usage: %d\%", 0);
    lv_obj_set_pos(labe15, 2, 60);

    lv_obj_t* labe16 = lv_label_create(scr3, NULL);
    lv_obj_add_style(labe16, LV_LABEL_PART_MAIN, &label_style4);
    lv_label_set_text_fmt(labe16, "Mem Usage: %dMB", 0);
    lv_obj_set_pos(labe16, 2, 90);

    lv_obj_t* labe17 = lv_label_create(scr3, NULL);
    lv_obj_add_style(labe17, LV_LABEL_PART_MAIN, &label_style4);
    lv_label_set_text_fmt(labe17, "Net Upload: %dKB/s", 0);
    lv_obj_set_pos(labe17, 2, 120);

    lv_obj_t* labe18 = lv_label_create(scr3, NULL);
    lv_obj_add_style(labe18, LV_LABEL_PART_MAIN, &label_style4);
    lv_label_set_text_fmt(labe18, "Net Download: %dKB/s", 0);
    lv_obj_set_pos(labe18, 2, 150);





  }
  if (control == 1)
  {
    lv_label_set_text(labe2, weachern);
  }
  if (control == 2)
  {
    lv_label_set_text(labe5, Timey);
    lv_label_set_text(labe7, Timem);
    lv_label_set_text(labe9, Timed);
    lv_label_set_text(labe10, Timeh);
    lv_obj_align(labe10, NULL, LV_ALIGN_OUT_BOTTOM_LEFT, 65, -60);
    lv_label_set_text(labe11, ":");
    lv_obj_align(labe11, labe10, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    lv_label_set_text(labe13, Timemin);
    lv_obj_align(labe13, labe11, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
  }
}
