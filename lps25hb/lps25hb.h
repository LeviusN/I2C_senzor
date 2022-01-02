/*
 * lps25hb.h
 *
 *  Created on: Dec 20, 2021
 *      Author: Niki
 */

#ifndef LPS25HB_H_
#define LPS25HB_H_

#include "main.h"
#include "i2c.h"

#define 	LPS25HB_DEVICE_ADDRESS_0				0xB8
#define 	LPS25HB_DEVICE_ADDRESS_1				0xBA

#define 	LPS25HB_WHO_AM_I_VALUE					0xB1
#define 	LPS25HB_WHO_AM_I_ADDRES					0x0F

#define 	LPS25HB_ADDRESS_CTRL1					0x10
#define 	LPS25HB_ADDRESS_CTRL1_VAL				0x90
#define 	LPS25HB_RES_CONF						0x10

#define 	LPS25HB_STATUS_REG					    0x27

#define 	LPS25HB_PRESS_OUT_H					    0x2A
#define 	LPS25HB_PRESS_OUT_L					    0x29
#define 	LPS25HB_PRESS_OUT_XL					0x28

uint8_t lps25hb_init(void);
float lps25hb_get_pre();
uint8_t lps25hb_read_byte(uint8_t reg_addr);
void lps25hb_write_byte(uint8_t reg_addr, uint8_t value);

#endif /* LPS25HB_H_ */
