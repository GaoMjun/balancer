#include "i2c.h"

//1.gpio config
void i2c_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | 	//SDA
							  GPIO_Pin_11; 	//SCL
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

static void delay(void)
{
    volatile int i = 5;
    while (i)
        i--;
}

/*
//2.i2c start
void i2c_start(void)
{
	setSDA_OUT();
	//1.init satrt set sda and scl high
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
	DelayUs(5);

	//2.start condition
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	DelayUs(5);
}

//2.i2c stop
void i2c_stop(void)
{
	setSDA_OUT();
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
	DelayUs(5);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
}

//3.i2c get ack
uint8_t i2c_get_ack(void)
{
	uint8_t tmp;
	setSDA_IN();
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	tmp = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8);
	DelayUs(5);
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
	DelayUs(5);
	return tmp;
}

//4.i2c send ack
void i2c_send_ack(void)
{
	setSDA_OUT();
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	DelayUs(5);
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
	DelayUs(5);
}

//5.i2c write a byte
void i2c_write_byte(uint8_t data)
{
	int8_t i;
	setSDA_OUT();
	for(i=7; i>=0; i--)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_11);
		DelayUs(5);
		GPIO_WriteBit(GPIOA,GPIO_Pin_8,data & (1 << i));
		GPIO_SetBits(GPIOA, GPIO_Pin_11);
		DelayUs(5);
	}
	i2c_get_ack();
}

//6.i2c read a byte
uint8_t i2c_read_byte(void)
{
	int8_t i;
	unsigned char data;
	setSDA_IN();
	for(i=7; i>=0; i--)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_11);
		DelayUs(5);
		data = (data << 1) | GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8);
		GPIO_SetBits(GPIOA, GPIO_Pin_11);
		DelayUs(5);
	}
	i2c_send_ack();
	return data;
}

//7.i2c read n byte
void i2c_read(uint8_t devAddr, uint8_t *readBuffer, uint8_t len)
{
	uint8_t i;
	//1.start
	i2c_start();
	//2.send device address and read flag
	i2c_write_byte((devAddr << 1) | 1);
	//3.read data
	for(i=0; i<len; i++)
	{
		readBuffer[i] = i2c_read_byte();
	}
	//4.stop
	i2c_stop();
}

//8.i2c write n byte
void i2c_write(uint8_t devAddr, uint8_t *writeBuffer, uint8_t len)
{
	uint8_t i;
	//1.start
	i2c_start();
	//2.send device address and write flag
	i2c_write_byte((devAddr << 1) | 0);
	//3.write data
	for(i=0; i<len; i++)
	{
		i2c_write_byte(writeBuffer[i]);
	}
	//4.stop
	i2c_stop();
}
int mpu6050_i2c_read(uint8_t devAddr, uint8_t regAddr, uint8_t len, uint8_t *buffer)
{
	uint8_t i;
	//1.start
	i2c_start();
	
	//2.send device address and read flag
	i2c_write_byte((devAddr << 1) | 1);

	//3.send register address
	i2c_write_byte(regAddr);

	//4.send device address, read flag and wait ack
	i2c_write_byte((devAddr << 1) | 1);

	//5.start again
	i2c_start();

	//6.send device address and read flag
	i2c_write_byte((devAddr << 1) | 1);
	
	//7.read data
	for(i=0; i<len; i++)
	{
		buffer[i] = i2c_read_byte();
	}

	//8.stop
	i2c_stop();

	return 0;
}

int mpu6050_i2c_write(uint8_t devAddr, uint8_t regAddr, uint8_t len, uint8_t *data)
{
	uint8_t i;
	//1.start
	i2c_start();

	//2.send device address and write flag
	i2c_write_byte((devAddr << 1) | 0);

	//3.send register address
	i2c_write_byte(regAddr);

	//4.write data
	for(i=0; i<len; i++)
	{
		i2c_write_byte(data[i]);
	}

	//5.stop
	i2c_stop();

	return 0;
}

u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data){
  
 	u8 count = 0;
	i2c_start();
	i2c_write_byte(dev);	   //发送写命令
	i2c_get_ack();
	i2c_write_byte(reg);   //发送地址
    i2c_get_ack();	  
	for(count=0;count<length;count++){
		i2c_write_byte(data[count]); 
		i2c_get_ack(); 
	 }
	i2c_stop();//产生一个停止条件

    return 1; //status == 0;
}

unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data){
    return IICwriteBytes(dev, reg, 1, &data);
}

unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
{
	unsigned char res=0;
	
	i2c_start();	
	i2c_write_byte(I2C_Addr);	   //发送写命令
	res++;
	i2c_get_ack();
	i2c_write_byte(addr); res++;  //发送地址
	i2c_get_ack();	  
	//IIC_Stop();//产生一个停止条件	
	i2c_start();
	i2c_write_byte(I2C_Addr+1); res++;          //进入接收模式			   
	i2c_get_Ack();
	res=i2c_read_byte();	   
    i2c_stop();//产生一个停止条件

	return res;
}

u8 IICreadByte(u8 dev, u8 reg, u8 *data){
	*data=I2C_ReadOneByte(dev, reg);
    return 1;
}

u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
{

    u8 b;
    if (IICreadByte(dev, reg, &b) != 0) 
	{
        u8 mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
        data <<= (8 - length);
        data >>= (7 - bitStart);
        b &= mask;
        b |= data;
        return IICwriteByte(dev, reg, b);
    } else {
        return 0;
    }
}
*/



static int i2c_start(void);
static void i2c_stop(void);
//static int i2c_send_byte(unsigned char data);
static unsigned char i2c_receiver_byte(void);
static unsigned char i2c_receiver_byte_withAck(void);
static void i2c_send_NAck(void);
static void i2c_send_ack(void);
static int i2c_waitAck(void);


static int i2c_start(void)
{
	//1.
	GPIO_SetBits(I2C_PORT, I2C_SDA);
	delay();

	//2.
	GPIO_SetBits(I2C_PORT, I2C_SCL);
	delay();

	//3.
	if(!GPIO_ReadInputDataBit(I2C_PORT, I2C_SDA))
		return -1;

	//4.
	GPIO_ResetBits(I2C_PORT, I2C_SDA);
	delay();

	//5.
	GPIO_ResetBits(I2C_PORT, I2C_SCL);
	delay();

	//6.
	if(GPIO_ReadInputDataBit(I2C_PORT, I2C_SDA))
		return -2;

	//7.
	return 0;
}

static void i2c_stop(void)
{
	//1.
	GPIO_ResetBits(I2C_PORT, I2C_SDA);
	delay();

	//2.
	GPIO_SetBits(I2C_PORT, I2C_SCL);
	delay();

	//3.
	GPIO_SetBits(I2C_PORT, I2C_SDA);
	delay();
}

int i2c_send_byte(unsigned char data)
{
	int i;

	GPIO_ResetBits(I2C_PORT, I2C_SCL);

	for(i = 0; i < 8; i++)
	{
		if(data & 0x80)
			GPIO_SetBits(I2C_PORT ,I2C_SDA);
		else
			GPIO_ResetBits(I2C_PORT, I2C_SDA);

		data = data << 1;
		delay();

		GPIO_SetBits(I2C_PORT, I2C_SCL);
		delay();
		GPIO_ResetBits(I2C_PORT, I2C_SCL);
		delay();
	}

	GPIO_SetBits(I2C_PORT, I2C_SDA);
	delay();
	GPIO_SetBits(I2C_PORT, I2C_SCL);
	delay();

	if(GPIO_ReadInputDataBit(I2C_PORT, I2C_SDA))
	{
		GPIO_ResetBits(I2C_PORT, I2C_SCL);
		return -1;
	}
	else
	{
		GPIO_ResetBits(I2C_PORT, I2C_SCL);
		return 0;
	}
}

static unsigned char i2c_receiver_byte(void)
{
	unsigned char data;
	int i;

	GPIO_SetBits(I2C_PORT, I2C_SDA);
	GPIO_ResetBits(I2C_PORT, I2C_SCL);

	data = 0;

	for(i = 0; i < 8; i++)
	{
		GPIO_SetBits(I2C_PORT, I2C_SCL);
		delay();

		data = data << 1;

		if(GPIO_ReadInputDataBit(I2C_PORT, I2C_SDA))
			data = data | 0x01;

		GPIO_ResetBits(I2C_PORT, I2C_SCL);
		delay();
	}

	i2c_send_NAck();

	return data;
}

static unsigned char i2c_receiver_byte_withAck(void)
{
	int i;
	unsigned char data;

	GPIO_SetBits(I2C_PORT, I2C_SDA);
	GPIO_ResetBits(I2C_PORT, I2C_SCL);

	data = 0;

	for(i = 0; i < 8; i++)
	{
		GPIO_SetBits(I2C_PORT, I2C_SCL);
		delay();

		data = data << 1;

		if(GPIO_ReadInputDataBit(I2C_PORT, I2C_SDA))
			data = data | 0x01;

		GPIO_ResetBits(I2C_PORT, I2C_SCL);
		delay();
	}

	i2c_send_ack();

	return data;
}

static void i2c_send_NAck(void)
{
	GPIO_SetBits(I2C_PORT, I2C_SDA);
	delay();

	GPIO_SetBits(I2C_PORT, I2C_SCL);
	delay();

	GPIO_ResetBits(I2C_PORT, I2C_SCL);
	delay();
}

static void i2c_send_ack(void)
{
	GPIO_ResetBits(I2C_PORT, I2C_SDA);
	delay();

	GPIO_SetBits(I2C_PORT, I2C_SCL);
	delay();

	GPIO_ResetBits(I2C_PORT, I2C_SCL);
	delay();
}

static int i2c_waitAck(void)
{
//	GPIO_ResetBits(I2C_PORT, I2C_SCL);
//	delay();
//
// 	GPIO_SetBits(I2C_PORT, I2C_SDA);
//	delay();
//
//	GPIO_SetBits(I2C_PORT, I2C_SCL);
//	delay();
//
//	if(!GPIO_ReadInputDataBit(I2C_PORT, I2C_SDA))
//	{
//		GPIO_ResetBits(I2C_PORT, I2C_SCL);
//		return 0;
//	}
//	
//	GPIO_ResetBits(I2C_PORT, I2C_SCL);
//
	return 0;
}

int i2c_read(unsigned char slave_addr,
			 unsigned char reg_addr,
			 unsigned char length,
			 unsigned char *data)
{
	i2c_start();
	i2c_send_byte(slave_addr << 1 | 0x00);
	if(i2c_waitAck())
		return -1;
	i2c_send_byte(reg_addr);
	if(i2c_waitAck())
		return -2;

	i2c_start();
	i2c_send_byte(slave_addr << 1 | 0x01);
	if(i2c_waitAck())
		return -3;

	while(length)
	{
		if(length == 1)
			*data = i2c_receiver_byte();
		else
			*data = i2c_receiver_byte_withAck();

		data ++;
		length --;
	}

	i2c_stop();

	return 0;
}

unsigned char i2c_read_byte(unsigned slave_addr,
							unsigned reg_addr)
{
	int length = 1;
	unsigned char data[1] = {0};

	i2c_read(slave_addr, reg_addr, length, data);

	return data[0];
}

int i2c_write(unsigned char slave_addr,
			  unsigned char reg_addr,
			  unsigned char length,
			  unsigned char const *data)
{
	int i;

	i2c_start();
	i2c_send_byte(slave_addr << 1 | 0x00);
	if(i2c_waitAck())
		return -1;

	i2c_send_byte(reg_addr);
	if(i2c_waitAck())
		return -2;

	for(i = 0; i < length; i ++)
	{
		i2c_send_byte(data[i]);
		if(i2c_waitAck())
		return -3;
	}

	i2c_stop();

	return 0;
}




