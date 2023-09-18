/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define POWER1_ON()  HAL_GPIO_WritePin(POWER1_GPIO_Port, POWER1_Pin, GPIO_PIN_SET);
#define POWER1_OFF()  HAL_GPIO_WritePin(POWER1_GPIO_Port, POWER1_Pin, GPIO_PIN_RESET);

#define POWER2_ON()  HAL_GPIO_WritePin(POWER2_GPIO_Port, POWER2_Pin, GPIO_PIN_SET);
#define POWER2_OFF()  HAL_GPIO_WritePin(POWER2_GPIO_Port, POWER2_Pin, GPIO_PIN_RESET);

#define POWER3_ON()  HAL_GPIO_WritePin(POWER3_GPIO_Port, POWER3_Pin, GPIO_PIN_SET);
#define POWER3_OFF()  HAL_GPIO_WritePin(POWER3_GPIO_Port, POWER3_Pin, GPIO_PIN_RESET);

#define POWER4_ON()  HAL_GPIO_WritePin(POWER4_GPIO_Port, POWER4_Pin, GPIO_PIN_SET);
#define POWER4_OFF()  HAL_GPIO_WritePin(POWER4_GPIO_Port, POWER4_Pin, GPIO_PIN_RESET);
/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

