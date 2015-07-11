#include "mpu6050.h"

/* Data requested by client. */
#define PRINT_ACCEL     (0x01)
#define PRINT_GYRO      (0x02)
#define PRINT_QUAT      (0x04)

#define ACCEL_ON        (0x01)
#define GYRO_ON         (0x02)

#define MOTION          (0)
#define NO_MOTION       (1)

/* Starting sampling rate. */
#define DEFAULT_MPU_HZ  (100)

#define FLASH_SIZE      (512)
#define FLASH_MEM_START ((void*)0x1800)

static signed char gyro_orientation[9] = {-1, 0, 0,
                                           0,-1, 0,
                                           0, 0, 1};


#define q30  1073741824.0f
float q0=0.0f,q1=0.0f,q2=0.0f,q3=0.0f;

static void run_self_test(void);
static unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx);
static unsigned short inv_row_2_scale(const signed char *row);

#define MPU6050_ADDRESS		0x68
#define MPU6050_ID 			0x68
#define WHO_AM_I			0x75
#define PWR_MGMT_1			0x6B
#define SMPLRT_DIV			0x19
#define CONFIG				0x1A
#define GYRO_CONFIG			0x1B
#define ACCEL_CONFIG		0x1C
#define INT_PIN_CFG			0x37
#define INT_ENABLE			0x38
#define USER_CTRL			0x6A

int mpu6050_init(void)
{
	int i;
	unsigned char id[1]={0};
    unsigned char check[1] = {0};

	const uint8_t param[][2] = 
    {
        {PWR_MGMT_1,	0x01  },
        {SMPLRT_DIV,	0x00  },
        {CONFIG,		0x03  },
        {GYRO_CONFIG,	0x18  },
        {ACCEL_CONFIG,	0x18  },
//        {INT_PIN_CFG,	0x32  },
//        {INT_ENABLE,	0     },
//        {USER_CTRL,		0x00  },
    };

    i2c_read(MPU6050_ADDRESS, WHO_AM_I, 1, id);
//    printf("0x%x \r\n", id[0]);
    if(id[0] != MPU6050_ID)
    {
    	printf("mpu6050 id error, system will reboot ... \r\n");
    	NVIC_SystemReset();
    }

    for(i=0; i < sizeof(param)/2; i++)
    {
        i2c_write(MPU6050_ADDRESS, param[i][0], 1, &(param[i][1]));
        i2c_read(MPU6050_ADDRESS, param[i][0], 1, check);
        if(check[0] != param[i][1])
            return -1;
    }

    return 0;
}

void dmp_init(void)
{
	if(!mpu_init())
	{
		//mpu_init();
		printf("mpu initialization complete......\r\n");

		//mpu_set_sensor
		if(!mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL))
			printf("mpu_set_sensor complete ......\r\n");

		//mpu_configure_fifo
		if(!mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL))
			printf("mpu_configure_fifo complete ......\r\n");
		
		//mpu_set_sample_rate
		if(!mpu_set_sample_rate(DEFAULT_MPU_HZ))
			printf("mpu_set_sample_rate complete ......\r\n");
		
		//dmp_load_motion_driver_firmvare
		if(!dmp_load_motion_driver_firmware())
			printf("dmp_load_motion_driver_firmware complete ......\r\n");
		
		//dmp_set_orientation
		if(!dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation)))
			printf("dmp_set_orientation complete ......\r\n");
		
		//dmp_enable_feature
		if(!dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | 
								DMP_FEATURE_TAP |
		    					DMP_FEATURE_ANDROID_ORIENT | 
								DMP_FEATURE_SEND_RAW_ACCEL | 
								DMP_FEATURE_SEND_CAL_GYRO |
		    					DMP_FEATURE_GYRO_CAL))
			printf("dmp_enable_feature complete ......\r\n");
	
		//dmp_set_fifo_rate
		if(!dmp_set_fifo_rate(DEFAULT_MPU_HZ))
			printf("dmp_set_fifo_rate complete ......\r\n");

		run_self_test();

		if(!mpu_set_dmp_state(1))
			printf("mpu_set_dmp_state complete ......\r\n");
	}
}

void read_dmp(void)
{
	// test dmp
     unsigned long sensor_timestamp;
     short gyro[3], accel[3], sensors;
     unsigned char more;
     long quat[4];
	 float Yaw = 0.0, Roll = 0.0, Pitch = 0.0;
	 dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,
                &more);	 
	 /* Gyro and accel data are written to the FIFO by the DMP in chip
     * frame and hardware units. This behavior is convenient because it
     * keeps the gyro and accel outputs of dmp_read_fifo and
     * mpu_read_fifo consistent.
     */
	 /*     if (sensors & INV_XYZ_GYRO )
       send_packet(PACKET_TYPE_GYRO, gyro);
     if (sensors & INV_XYZ_ACCEL)
        send_packet(PACKET_TYPE_ACCEL, accel); */
     /* Unlike gyro and accel, quaternions are written to the FIFO in
     * the body frame, q30. The orientation is set by the scalar passed
     * to dmp_set_orientation during initialization.
     */
     if (sensors & INV_WXYZ_QUAT )
	 {
//	     printf("in Calculating quaternion steps.....\r\n");
	 	 q0=quat[0] / q30;
		 q1=quat[1] / q30;
		 q2=quat[2] / q30;
		 q3=quat[3] / q30;
		 Pitch  = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
  		 Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
		 Yaw = 	atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;
	 }
//	 printf("q1:%f  q2:%f  q3:%f  q4:%f \r\n", q0, q1, q2, q3);
	printf("Pitch:%f  Roll:%f  Yaw:%f \r\n", Pitch, Roll, Yaw);		
}

static unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}

static unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx)
{
    unsigned short scalar;

    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;


    return scalar;
}

static void run_self_test(void)
{
    int result;
//    char test_packet[4] = {0};
    long gyro[3], accel[3];

    result = mpu_run_self_test(gyro, accel);
    if (result == 0x7) {
        /* Test passed. We can trust the gyro data here, so let's push it down
         * to the DMP.
         */
        float sens;
        unsigned short accel_sens;
        mpu_get_gyro_sens(&sens);
        gyro[0] = (long)(gyro[0] * sens);
        gyro[1] = (long)(gyro[1] * sens);
        gyro[2] = (long)(gyro[2] * sens);
        dmp_set_gyro_bias(gyro);
        mpu_get_accel_sens(&accel_sens);
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        dmp_set_accel_bias(accel);
		printf("setting bias succesfully ......\r\n");
    }
	else
	{
		printf("bias has not been modified ......\r\n");
	}

    /* Report results. */
//    test_packet[0] = 't';
//    test_packet[1] = result;
//    send_packet(PACKET_TYPE_MISC, test_packet);
}

int Read_Temperature(void)
{	   
	float Temp = 0.0;
	uint8_t temp_H[1] = {0};
	uint8_t temp_L[1] = {0};

	i2c_read(0x68, 0x41, 1, temp_H);
	i2c_read(0x68, 0x42, 1, temp_L);

	Temp = (temp_H[0] << 8) + temp_L[0];
	
	if(Temp > 32768) 
		Temp -= 65536;

	Temp = Temp / 340 + 36.53;
	printf("Temperature:%f  ", Temp);
	return (int)Temp;
}
