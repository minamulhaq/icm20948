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
char debugBuf[50];

void uartFlush(void) {
	HAL_UART_Transmit(&huart2, (uint8_t*) debugBuf, sizeof(debugBuf),HAL_MAX_DELAY);
}

ICM20948::ICM20948(uint8_t address) :
		_i2cAddress(address << 1), _currentBank(BANK0) {

	uint8_t id = whoAmI();
	if (id == WHOAMI){
		sprintf(debugBuf, "Imu 0x%x\tid:%x\tconnected\r\n", _i2cAddress>>1 &0xFF, id & 0xFF);
	}else {
		sprintf(debugBuf, "Imu 0x%x is not connected\r\n", _i2cAddress>>1 &0xFF);
	}
	uartFlush();
}

uint8_t ICM20948::whoAmI(void) {
	switchUserBank(BANK0);
	return I2C_ReadByte(REGISTER_WHO_AM_I);
}

void ICM20948::switchUserBank(const USERBANK &newBank) {
	if (_currentBank != newBank) {
		_currentBank = newBank;
		I2C_writeTwoBytes(REGISTER_BANK_SEL, _currentBank<< 4);

	}
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

