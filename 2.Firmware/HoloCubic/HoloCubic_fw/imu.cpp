#include "imu.h"

void IMU::init()
{
	Wire.begin(IMU_I2C_SDA, IMU_I2C_SCL);
	Wire.setClock(400000);
	while (!imu.testConnection());
	imu.initialize();
}


int tmp = 0;
boolean image_flag;
void IMU::update(int interval)
{
	imu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

//ssS

	if (millis() - last_update_time > interval)
	{
		if (ay > 3000 && flag)
		{
			encoder_diff--;
			flag = 0;
			tmp = 1;
		}
		else if (ay < -3000 && flag)
		{
			encoder_diff++;
			flag = 0;
			tmp = 2;
		}
		else
		{
			flag = 1;
		}

		if (ax > 4000 && flag)
    	{
      		delay(2000);
          	if (ax > 6000 && flag)
      	{
          	image_flag=1;
          	flag = 0;
          	encoder_state = LV_INDEV_STATE_PR;
      	}
    }
    else if(ax<-3000 && flag )
    {
        delay(2000);
        if (ax<-4000  && flag)
      { 
        image_flag=0;
        flag = 0;
        encoder_state = LV_INDEV_STATE_REL; 
      }
    }
    else
    {
      flag = 1;
    }
		last_update_time = millis();
	}
}

int16_t IMU::getAccelX()
{
	return ax;
}

int16_t IMU::getAccelY()
{
	return ay;
}

int16_t IMU::getAccelZ()
{
	return az;
}

int16_t IMU::getGyroX()
{
	return gx;
}

int16_t IMU::getGyroY()
{
	return gy;
}

int16_t IMU::getGyroZ()
{
	return gz;
}
