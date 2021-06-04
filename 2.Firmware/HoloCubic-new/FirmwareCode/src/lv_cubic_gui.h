#ifndef LV_CUBIC_GUI_H
#define LV_CUBIC_GUI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"

	//extern lv_img_dsc_t screen_buffer;

	extern lv_obj_t *scr[4];

	void display_photo(const char *file_name);
	void display_weather(const char *cityname, const char * temperature, int weathercode);
	void display_time(const char *date, const char *time);
	void display_hardware(const char *info);
	void display_init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
