/*
 * hts.h
 *
 *  Created on: Dec 20, 2021
 *      Author: Niki
 */

#ifndef HTS221_H_//
#define HTS221_H_//

#include "main.h"
#include "i2c.h"

#define 	HTS221_DEVICE_ADDRESS_1					0xBE
#define 	HTS221_DEVICE_ADDRESS_0					0xBF

#define 	HTS221_WHO_AM_I_ADDRES					0x0F
#define 	HTS221_WHO_AM_I_VALUE					0xBC

#define 	HTS221_ADDRESS_CTRL1					0x20

#define 	HTS221_H0_RH_X2							0x30
#define 	HTS221_HR_OUT_REG						0x28
#define 	HTS221_H0_T0_OUT						0x36
#define 	HTS221_H1_T0_OUT						0x3A

#define 	HTS221_T1_T0							0x35
#define 	HTS221_T0_OUT							0x3C
#define 	HTS221_TEMP_OUT_REG						0x2A
#define 	HTS221_T0_DEGC_X8						0x32

#define 	HTS221_ADDRESS_TEMP					    0x20

uint8_t hts221_init(void);
float hts221_get_te();
uint16_t hts221_get_hu();
uint8_t hts221_read_byte(uint8_t reg_addr);
void hts221_write_byte(uint8_t reg_addr, uint8_t value);
int8_t hts221_get_temp();

#endif
