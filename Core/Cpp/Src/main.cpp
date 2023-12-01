/*
 * main.cpp
 *
 *  Created on: Dec 1, 2023
 *      Author: muhammad.inamulhaq
 */

#include <stm32l4xx.h>
#include <string>
#include <algorithm>

#include "ICM20948.h"


extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;
char buf[50] = { 0 };

#ifdef __cplusplus
extern "C" {
#endif

void flushUart(void) {
	HAL_UART_Transmit(&huart2, (uint8_t*) buf, sizeof(buf), HAL_MAX_DELAY);
	std::fill(std::begin(buf), std::end(buf), 0);
}

void mainCpp(void) {

	ICM20948 imu1 (0x68);
	ICM20948 imu2 (0x69);

//	std::fill(std::begin(buf), std::end(buf), 0);
//	sprintf(buf, "mainCPP\r\n");
//	flushUart();
//
//	HAL_StatusTypeDef ret;
//	ret = HAL_I2C_IsDeviceReady(&hi2c1, 0x68 << 1, 2, 2);
//	if (ret == HAL_OK) {
//		sprintf(buf, "HAL_OK\r\n");
//	}
//	else {
//		sprintf(buf, "HAL_OK\r\n");
//	}
//	flushUart();

	while (1) {

	}
}

#ifdef __cplusplus
}
#endif

