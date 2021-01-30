/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USART2_HC_05_TX_Pin GPIO_PIN_2
#define USART2_HC_05_TX_GPIO_Port GPIOA
#define USART2_HC_05_RX_Pin GPIO_PIN_3
#define USART2_HC_05_RX_GPIO_Port GPIOA
#define Analog_MQ135_1_Pin GPIO_PIN_5
#define Analog_MQ135_1_GPIO_Port GPIOA
#define ELECTRIC_BOILER_Pin GPIO_PIN_6
#define ELECTRIC_BOILER_GPIO_Port GPIOA
#define Analog_MQ135_2_____Pin GPIO_PIN_7
#define Analog_MQ135_2_____GPIO_Port GPIOA
#define USART3_TX_SIM900_Pin GPIO_PIN_10
#define USART3_TX_SIM900_GPIO_Port GPIOB
#define USART3_RX_SIM900_Pin GPIO_PIN_11
#define USART3_RX_SIM900_GPIO_Port GPIOB
#define VENT_BEE_IN_Pin GPIO_PIN_12
#define VENT_BEE_IN_GPIO_Port GPIOB
#define VENT_BEE_OUT_Pin GPIO_PIN_13
#define VENT_BEE_OUT_GPIO_Port GPIOB
#define VENT_BOILER_ROOM_Pin GPIO_PIN_14
#define VENT_BOILER_ROOM_GPIO_Port GPIOB
#define RESERVE_Pin GPIO_PIN_15
#define RESERVE_GPIO_Port GPIOB
#define USART1_DS18B20_Pin GPIO_PIN_9
#define USART1_DS18B20_GPIO_Port GPIOA
#define RESERVEA10_Pin GPIO_PIN_10
#define RESERVEA10_GPIO_Port GPIOA
#define RESERVEA11_Pin GPIO_PIN_11
#define RESERVEA11_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
