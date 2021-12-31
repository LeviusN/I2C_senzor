/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "display.h"
#include <string.h>
#include <math.h>
#include "main.h"
#include "gpio.h"
#include "tim.h"
#include "i2c.h"
#include "../hts221/hts221.h"
#include "../lps25hb/lps25hb.h"

void SystemClock_Config(void);

uint8_t checkButtonState(GPIO_TypeDef* GPIO_PORT, uint8_t GPIO_PIN);
uint8_t switch_state = 0;
uint8_t button = 0;

char stringS[] = "DATA00000";
extern char data[4];
int lengS = 0;
int i = 0;
int direction = 1;

float temperature;
float humidity;
float pressure;
float height;

char temValue[5];
char humValue[2];
char barValue[8];
char altValue[7];

extern uint64_t disp_time;
uint64_t saved_time;
double num_to_display = 10;

int main(void)
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  SystemClock_Config();

  MX_GPIO_Init();
  MX_I2C1_Init();

  setSegments();
  setDigits();

  LL_mDelay(2000);

  resetDigits();
  resetSegments();

  MX_TIM2_Init();

  hts221_init();

  lps25hb_init();

  lengS = strlen(stringS);
  while (1)
  {
	  /*Getting sensor data*/
	  temperature = hts221_get_te();
	  LL_mDelay(50);
	  humidity = hts221_get_hu();
	  pressure = lps25hb_get_pre();
	  height = ((powf((101325/(pressure*100)),1/5.257)-1)*(temperature+273.15))/0.0065;

	  char tempS[] = "T_";
	  char humS[] = "H_";
	  char barS[] = "B_";
	  char heightS[] = "A_";

      /*Changing state*/
	  switch(button){
	  case 0:
		  sprintf(temValue, "%.1f", temperature);
		  strcat(tempS, temValue);
		  memset(stringS, 0, 10);
		  strcat(stringS, tempS);
		  lengS = strlen(stringS)-1;
		  break;
	  case 1:
		  sprintf(humValue, "%.0f", humidity);
		  strcat(humS, humValue);
		  memset(stringS, 0, 10);
		  strcat(stringS, humS);
		  lengS = strlen(stringS);
		  break;
	  case 2:
		  sprintf(barValue, "%.2f", pressure);
		  strcat(barS, barValue);
		  memset(stringS, 0, 10);
		  strcat(stringS, barS);
		  lengS = strlen(stringS)-1;
		  break;
	  case 3:;
		  sprintf(altValue, "%.1f", height);
		  strcat(heightS, altValue);
		  memset(stringS, 0, 10);
		  strcat(stringS, heightS);
		  lengS = strlen(stringS)-1;
		  break;
	  default:
		  break;
	  }

	  /*Display*/
		switch(direction){
		case 1:
				  if(disp_time > (saved_time + 1000)){
						  data[3] = stringS[i];
						  data[2] = stringS[i+1];
						  data[1] = stringS[i+2];
						  data[0] = stringS[i+3];
						  i++;
						  saved_time = disp_time;
						  if(i >= (lengS-4)){
							  direction = 0;
							  i=0;
						  }
				  }
				  break;
		case 0:
			  if(disp_time > (saved_time + 1000)){
					  data[3] = stringS[i];
					  data[2] = stringS[i+1];
					  data[1] = stringS[i+2];
					  data[0] = stringS[i+3];
					  i--;
					  saved_time = disp_time;
					  if(i <= (lengS-4)){
						  direction = 1;
						  i=0;
					  }
			  }
			  break;
	   default:
			  break;
      }

  }
  }


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  Error_Handler();  
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  
  }
  LL_Init1msTick(8000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SYSTICK_EnableIT();
  LL_SetSystemCoreClock(8000000);
}

/* USER CODE BEGIN 4 */
uint8_t checkButtonState(GPIO_TypeDef* GPIO_PORT, uint8_t GPIO_PIN)
{
	uint8_t state = 0;
	state = LL_GPIO_IsInputPinSet(GPIO_PORT_BUTTON, LL_GPIO_PIN_3);

	switch(state){
	case 1:
		button++;
		if(button>3){
			button = 0;
		}
		return 1;
		break;
	case 0:
		return 0;
		break;
	}

}

void EXTI3_IRQHandler(void)
{
	if(checkButtonState(GPIO_PORT_BUTTON, GPIO_PIN_BUTTON))
			{
				switch_state ^= 1;
				LL_mDelay(200);
			}
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_3) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_3);
   }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
