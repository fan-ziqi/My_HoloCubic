#include "imu.h"

void IMU::init()
{
	Wire.begin(IMU_I2C_SDA, IMU_I2C_SCL);
	Wire.setClock(400000);
	while (!imu.testConnection())
		;
	imu.initialize();
	active_info.active = UNKNOWN;
}

boolean image_flag = 0;

Imu_Active* IMU::update(int interval)
{
	imu.getMotion6(&(active_info.ax), &(active_info.ay), &(active_info.az), 
				&(active_info.gx), &(active_info.gy), &(active_info.gz));

	if (millis() - last_update_time > interval)
	{
		if      (active_info.ay >  3000 && !active_info.isValid)
		{
			encoder_diff--;
			active_info.isValid = 1;
			active_info.active = TURN_LEFT;
		}
		else if (active_info.ay < -3000 && !active_info.isValid)
		{
			encoder_diff++;
			active_info.isValid = 1;
			active_info.active = TURN_RIGHT;
		}
		else
		{
			active_info.isValid = 0;
		}

		// //x轴实时检测，6050交互
		// if (active_info.ax >  5000 && !active_info.isValid)
		// {
		// 	active_info.isValid = 1;
		// 	active_info.active = GO_FORWORD;
		// 	encoder_state = LV_INDEV_STATE_PR;
		// }
		// else if (active_info.ax < -1000 && !active_info.isValid)
		// {
		// 	active_info.isValid = 1;
		// 	active_info.active = RETURN;
		// 	encoder_state = LV_INDEV_STATE_REL;
		// }
		// else
		// {
		// 	active_info.isValid = 0;
		// }

		//x轴翻转延迟->图片显示
		if      (active_info.ax >  5000 && !active_info.isValid)
		{
			delay(1000);
			imu.getMotion6(&(active_info.ax), &(active_info.ay), &(active_info.az), 
						&(active_info.gx), &(active_info.gy), &(active_info.gz));
			if (active_info.ax > 5000)
			{
				image_flag = 1;
				active_info.isValid = 1;
				active_info.active = GO_FORWORD;
			}
		}
		else if (active_info.ax < -5000 && !active_info.isValid)
		{
			delay(1000);
			imu.getMotion6(&(active_info.ax), &(active_info.ay), &(active_info.az), 
						&(active_info.gx), &(active_info.gy), &(active_info.gz));
			if (active_info.ax < -5000)
			{
				image_flag = 0;
				active_info.isValid = 1;
				active_info.active = RETURN;
			}
		}
		last_update_time = millis();
	}
	return &active_info;
}

// Imu_Active* IMU::update(int interval)
// {
// 	imu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

// 	//Serial.print(gx);
// 	//Serial.print(" ");
// 	//Serial.print(gy);
// 	//Serial.print(" ");
// 	//Serial.print(gz);
// 	//Serial.println(" ");

// 	if (millis() - last_update_time > interval)
// 	{
// 		if (ay > 3000 && flag)
// 		{
// 			encoder_diff--;
// 			flag = 0;
// 		}
// 		else if (ay < -3000 && flag)
// 		{
// 			encoder_diff++;
// 			flag = 0;
// 		}
// 		else
// 		{
// 			flag = 1;
// 		}

// 		if (ax > 10000)
// 		{
// 			encoder_state = LV_INDEV_STATE_PR;
// 		}
// 		else
// 		{
// 			encoder_state = LV_INDEV_STATE_REL;
// 		}

// 		last_update_time = millis();
// 	}
// }
