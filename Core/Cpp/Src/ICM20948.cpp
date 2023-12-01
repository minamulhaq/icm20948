/*
 * ICM20948.cpp
 *
 *  Created on: Nov 28, 2023
 *      Author: muhammad.inamulhaq
 */

#include <string>
#include "ICM20948.h"
#include "icm20948_common.h"

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;

HAL_StatusTypeDef ret;
extern char debugBuf[50];

#ifdef __cplusplus
extern "C" {
#endif
  extern void clearBuffer(void);
#ifdef __cplusplus
}
#endif

void uartFlush(void) {
	HAL_UART_Transmit(&huart2, (uint8_t*) debugBuf, sizeof(debugBuf), HAL_MAX_DELAY);
	clearBuffer();
}

ICM20948::ICM20948(uint8_t address) :
		_i2cAddress(address << 1), _i2cAddressDebug(address), _currentBank(
				BANK0) {

	clearBuffer();
	if (isDeviceConnected() == HAL_OK) {
		sprintf(debugBuf, "I2C Device 0x%x detected\r\n", _i2cAddressDebug);
		uartFlush();
		HAL_Delay(300);
		whoAmI();
		reset();
	} else {
		sprintf(debugBuf, "I2C Device 0x%x is not detected\r\n", _i2cAddressDebug);
		uartFlush();
	}


}
HAL_StatusTypeDef ICM20948::isDeviceConnected() {
	return HAL_I2C_IsDeviceReady(&hi2c1, _i2cAddress, 2, 5);
}

uint8_t ICM20948::whoAmI(void) {
	switchUserBank(BANK0);

	uint8_t id = I2C_ReadByte(REGISTER_WHO_AM_I);
	if (id == WHOAMI) {
		sprintf(debugBuf, "Imu 0x%x\tid:%x\tconnected\r\n",
				_i2cAddress >> 1 & 0xFF, id & 0xFF);
	} else {
		sprintf(debugBuf, "Imu 0x%x is not connected\r\n",
				_i2cAddress >> 1 & 0xFF);
	}
	uartFlush();

	return id;
}

void ICM20948::switchUserBank(const USERBANK &newBank) {
	if (_currentBank != newBank) {
		_currentBank = newBank;
		I2C_writeTwoBytes(REGISTER_BANK_SEL, _currentBank << 4);

	}
}

/**
 * Resets the internal registers of IMU
 * Register PWR_MGMT_1 - Address 0x06 - Bank 0
 * Reset Value: 0x41
 */

void ICM20948::reset(void) {
	switchUserBank(BANK0);
	I2C_writeTwoBytes(REGISTER_PWR_MGMT_1, BIT_RESET);
	HAL_Delay(20);
	if (I2C_ReadByte(REGISTER_PWR_MGMT_1) == 0x41) {
		sprintf(debugBuf, "IMU 0x%x reset successful\r\n", _i2cAddressDebug);
	} else {
		sprintf(debugBuf, "IMU 0x%x reset unsuccessful\r\n", _i2cAddressDebug);
	}
	uartFlush();

}

void ICM20948::I2C_writeByte(const uint8_t &regAddress) {
	uint8_t buf = regAddress;
	ret = HAL_I2C_Master_Transmit(&hi2c1, _i2cAddress, &buf, sizeof(buf),
	HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		sprintf(debugBuf, "Error ICM20948::I2C_writeByte\r\n");
		uartFlush();
	}
}

uint8_t ICM20948::I2C_ReadByte(const uint8_t &regAddress) {
	uint8_t retVal = 0x00;

	I2C_writeByte(regAddress);

	ret = HAL_I2C_Master_Receive(&hi2c1, _i2cAddress, &retVal, 0x01,
	HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		sprintf(debugBuf, "Error ICM20948::I2C_ReadByte - while writing\r\n");
		uartFlush();
	}
	return retVal;
}

void ICM20948::I2C_writeTwoBytes(const uint8_t &regAddress,
		const uint8_t &value) {
	uint8_t buf[2] = { regAddress, value };
	ret = HAL_I2C_Master_Transmit(&hi2c1, _i2cAddress, buf, 0x02,
	HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		sprintf(debugBuf, "Error ICM20948::I2C_writeTwoBytes\r\n");
		uartFlush();
	}
}

