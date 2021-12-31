/*
 * lps25hb.c
 *
 *  Created on: Dec 20, 2021
 *      Author: Niki
 */

#include "../lps25hb/lps25hb.h"

#include "i2c.h"

uint8_t addres = 0xB8;

uint8_t lps25hb_read_byte(uint8_t reg_addr)
{
	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, addres, 0));
}

void lps25hb_write_byte(uint8_t reg_addr, uint8_t value)
{
	i2c_master_write(value, reg_addr, addres, 0);
}

void lps25hb_readArray(uint8_t * data, uint8_t reg, uint8_t length)
{
	i2c_master_read(data, length, reg, addres, 1);
}

float lps25hb_get_pre(){
	uint8_t press_out_H;
	uint8_t press_out_XL;
	uint8_t press_out_L;
	float cal;

	press_out_H = lps25hb_read_byte(LPS25HB_PRESS_OUT_H);
	press_out_XL = lps25hb_read_byte(LPS25HB_PRESS_OUT_XL);
	press_out_L = lps25hb_read_byte(LPS25HB_PRESS_OUT_L);
  /*Vypocet*/
	cal = (press_out_H << 16 | press_out_L << 8 | press_out_XL)/4096.0;

	return cal;
}

uint8_t lps25hb_init(void)
{

	uint8_t status = 1;

	//LIS3MDL_ACC_ON;

	LL_mDelay(100);

	uint8_t val = lps25hb_read_byte(LPS25HB_WHO_AM_I_ADDRES);

	if(val == LPS25HB_WHO_AM_I_VALUE)
	{
		status = 1;
	}
	else			//if the device is not found on one address, try another one
	{
		addres = LPS25HB_DEVICE_ADDRESS_1;
		val = lps25hb_read_byte(LPS25HB_WHO_AM_I_ADDRES);
		if(val == LPS25HB_WHO_AM_I_VALUE)
		{
			status = 1;
		}
		else
		{
			status = 0;
		}
	}

	lps25hb_write_byte(LPS25HB_ADDRESS_CTRL1, 0x90);
	LL_mDelay(100);
	return status;
}
