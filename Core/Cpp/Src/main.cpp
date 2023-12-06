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
#include <stm32l476xx.h>

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;

uint8_t rawAccelGyro[12] = { 0 };
char debugBuf[512] = { 0 };

#ifdef __cplusplus
extern "C" {
#endif

void clearBuffer(void) {
	std::fill(std::begin(debugBuf), std::end(debugBuf), 0);
}
void uartFlush(void) {
	HAL_UART_Transmit(&huart2, (uint8_t*) debugBuf, sizeof(debugBuf),
			HAL_MAX_DELAY);
	clearBuffer();
}


float accelGyro[6];

void mainCpp(void) {
	clearBuffer();
	sprintf(debugBuf, "\r\n\n\n___ BEGIN ___\r\n");
	uartFlush();
	ICM20948 imu(0x69);

	ACCEL_CONFIG config;
	config.accel_dlpfcfg = DLPF_4;
	config.accel_fs_sel = ACCEL_FS_RANGE_4G;
	config.accel_fchoice = ENABLE;

	while (1) {
		imu.readAccelGyroRaw(nullptr);
		HAL_Delay(500);

	}
}

#ifdef __cplusplus
}
#endif

