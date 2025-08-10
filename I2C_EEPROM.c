                                    /* i2c_eeprom.c */
#include <LPC21xx.h>
#include "types.h"
#include "I2C.h"
#include "delay.h"

void i2c_eeprom_write(u8 slaveAddr,u8 wBuffAddr,u8 dat)
{

  i2c_start();	//issue start event
  i2c_write(slaveAddr<<1); //slaveAddr + w
	i2c_write(wBuffAddr);//wBuffAddr
	i2c_write(dat);  //data  
	i2c_stop();//isuee stop event
	delay_ms(10);//delay for internal write operation
}			 

u8 i2c_eeprom_read(u8 slaveAddr,u8 rBuffAddr)
{
	u8 dat;
	
	i2c_start();	//issue start event
  i2c_write(slaveAddr<<1); //slaveAddr + w
	i2c_write(rBuffAddr);//rBuffAddr
	i2c_restart();//repeated start	
	i2c_write((slaveAddr<<1)|1); //slaveAddr + r
  dat=i2c_nack();	//not of ack
	i2c_stop();//isuee stop event
	return dat;
}

void i2c_eeprom_page_write(u8 slaveAddr,u8 wBuffStartAddr,u8 *p,u8 nBytes)
{
  u8 i;
  i2c_start();	
  i2c_write(slaveAddr<<1);    //slaveAddr + w
	i2c_write(wBuffStartAddr);  //wBuffStartAddr
	for(i=0;i<nBytes;i++)
	{
	   i2c_write(p[i]);             //wBuffAddr
	}
	i2c_stop();
	delay_ms(10);
}			 

void i2c_eeprom_seq_read(u8 slaveAddr,u8 rBuffStartAddr,u8 *p,u8 nBytes)
{
	u8 i;
	i2c_start();	
  i2c_write(slaveAddr<<1); //slaveAddr + w
	i2c_write(rBuffStartAddr);    //rBuffAddr
	i2c_restart();	
	i2c_write((slaveAddr<<1)|1); //slaveAddr + r
	for(i=0;i<nBytes-1;i++)
	{
	 p[i]=i2c_mack();	
	}
	p[i]=i2c_nack();
	i2c_stop();
}



