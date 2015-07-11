#ifndef __I2C_H_
#define __I2C_H_

#include "stm32f10x.h"
#include "delay.h"

#include <stdio.h>

#define I2C_PORT	GPIOA
#define I2C_SDA 	GPIO_Pin_8
#define I2C_SCL 	GPIO_Pin_11

//#define delay()		DelayUs(5);

#define setSDA_IN()  {GPIOA->CRH &= 0XFFFFFFF0; GPIOA->CRH |= 8<<0;}
#define setSDA_OUT() {GPIOA->CRH &= 0XFFFFFFF0; GPIOA->CRH |= 3<<0;}

//void i2c_init(void);
//void i2c_start(void);
//void i2c_stop(void);
//uint8_t i2c_get_ack(void);
//void i2c_send_ack(void);
//void i2c_write_byte(uint8_t data);
//unsigned char i2c_read_byte(void);
////void i2c_read(uint8_t devAddr, uint8_t *readBuffer, uint8_t len);
////void i2c_write(uint8_t devAddr, uint8_t *writeBuffer, uint8_t len);
//int mpu6050_i2c_read(uint8_t devAddr, uint8_t regAddr, uint8_t len, uint8_t *buffer);
//int mpu6050_i2c_write(uint8_t devAddr, uint8_t regAddr, uint8_t len, uint8_t *data);
int i2c_send_byte(unsigned char data);

void i2c_init(void);
int i2c_read(unsigned char slave_addr,
			 unsigned char reg_addr,
			 unsigned char length,
			 unsigned char *data);
int i2c_write(unsigned char slave_addr,
			  unsigned char reg_addr,
			  unsigned char length,
			  unsigned char const *data);
unsigned char i2c_read_byte(unsigned slave_addr,
							unsigned reg_addr);

#endif
