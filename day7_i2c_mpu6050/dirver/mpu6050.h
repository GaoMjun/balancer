#ifndef	__MPU6050_H_
#define __MPU6050_H_

#include "inv_mpu_dmp_motion_driver.h"
#include "inv_mpu.h"
#include "i2c.h"

#include <math.h>

int mpu6050_init(void);
void dmp_init(void);
void read_dmp(void);
int Read_Temperature(void);

#endif 
