/*
 * hts.c
 *
 *  Created on: Dec 20, 2021
 *      Author: Niki
 */

#include "hts221.h"

#include "i2c.h"

//uint8_t addres_temp = 0xBEU;
uint8_t addres1 = HTS221_DEVICE_ADDRESS_1;

uint8_t hts221_read_byte(uint8_t reg_addr)
{
	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, addres1, 0));
}

void hts221_write_byte(uint8_t reg_addr, uint8_t cal)
{
	i2c_master_write(cal, reg_addr, addres1, 0);
}

void hts221_readArray(uint8_t * data, uint8_t reg, uint8_t length)
{
	i2c_master_read(data, length, reg, addres1, 1);
}


int8_t hts221_get_temp()
{
	uint8_t temp[2];
	hts221_readArray(temp, HTS221_ADDRESS_TEMP, 2);

	return (((int16_t)((temp[1] << 8) | temp[0])) >> 3)  + 25;
}

float hts221_get_te(){
	int16_t t0_o;
	int16_t t1_o;
	int16_t t_o;
	int16_t t0_degC_v;
	int16_t t1_degC_v;
	uint8_t data[4];
	uint8_t temp;
	float cal;

	temp = hts221_read_byte(HTS221_T1_T0);
	hts221_readArray(data, HTS221_T0_DEGC_X8, 2);

	t1_degC_v = ((((uint16_t)(temp & 0x0C)) << 6) | ((uint16_t)data[1]))>>3;
	t0_degC_v = ((((uint16_t)(temp & 0x03)) << 8) | ((uint16_t)data[0]))>>3;

	hts221_readArray(data, HTS221_TEMP_OUT_REG, 2);
	t_o = (((uint16_t)data[1])<<8) | (uint16_t)data[0];

	hts221_readArray(data, HTS221_T0_OUT, 4);
	t1_o = (((uint16_t)data[3])<<8) | (uint16_t)data[2];
	t0_o = (((uint16_t)data[1])<<8) | (uint16_t)data[0];
	/*Vypocet*/
	cal = ((((int32_t)(t_o - t0_o))*((int32_t)(t1_degC_v - t0_degC_v)*10))/(t1_o - t0_o) + t0_degC_v*10)/10;

	return cal;
}

uint16_t hts221_get_hu(){
	int16_t h0_t0_o;
	int16_t h1_t0_o;
	int16_t h_t_o;
	int16_t h0_rh;
	int16_t h1_rh;
	uint8_t data[2];
	uint16_t cal=0;

	hts221_readArray(data, HTS221_H0_RH_X2, 2);
	h0_rh = data[0]>>1;
	h1_rh = data[1]>>1;

	hts221_readArray(data, HTS221_HR_OUT_REG, 2);
	h_t_o = (((uint16_t)data[1])<<8) | (uint16_t)data[0];

	hts221_readArray(data, HTS221_H0_T0_OUT, 2);
	h0_t0_o = (((uint16_t)data[1])<<8) | (uint16_t)data[0];

	hts221_readArray(data, HTS221_H1_T0_OUT, 2);
	h1_t0_o = (((uint16_t)data[1])<<8) | (uint16_t)data[0];

	/*Vypocet*/
	cal = ((int32_t)(h_t_o - h0_t0_o)) * ((int32_t)(h1_rh - h0_rh)*10)/(h1_t0_o - h0_t0_o) + h0_rh*10;

	cal=cal/10;
	return cal;
}

uint8_t hts221_init(void)
{

	uint8_t status = 1;

	//LIS3MDL_ACC_ON;

	LL_mDelay(100);

	uint8_t val = hts221_read_byte(HTS221_WHO_AM_I_ADDRES);

	if(val == HTS221_WHO_AM_I_VALUE)
	{
		status = 1;
	}
	else			//if the device is not found on one address, try another one
	{
		//addres_temp = addres1;
		val = hts221_read_byte(HTS221_WHO_AM_I_ADDRES);
		if(val == HTS221_WHO_AM_I_VALUE)
		{
			status = 1;
		}
		else
		{
			status = 0;
			//return status;
		}
	}

	hts221_write_byte(HTS221_ADDRESS_CTRL1, 0x81);

	return status;
}


