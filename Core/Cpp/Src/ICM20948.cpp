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
	HAL_UART_Transmit(&huart2, (uint8_t*) debugBuf, sizeof(debugBuf),
	HAL_MAX_DELAY);
	clearBuffer();
}

ICM20948::ICM20948(uint8_t address) :
		_i2cAddress(address << 1), _i2cAddressDebug(address), _currentBank(
				BANK0) {

	HAL_Delay(500);
	setAccelConfig(_accel_config);
	clearBuffer();
	if (isDeviceConnected() == HAL_OK) {
		sprintf(debugBuf, "I2C Device 0x%x detected\r\n", _i2cAddressDebug);
		uartFlush();
		whoAmI();
		reset();
		wakeUp();
		setClockSource(AUTO_CLK_SELECT);
	} else {
		sprintf(debugBuf, "I2C Device 0x%x is not detected\r\n",
				_i2cAddressDebug);
		uartFlush();
	}

}
HAL_StatusTypeDef ICM20948::isDeviceConnected() {
	return HAL_I2C_IsDeviceReady(&hi2c1, _i2cAddress, 2, 5);
}

uint8_t ICM20948::whoAmI(void) {
	switchUserBank(BANK0);

	uint8_t id = readByte(REGISTER_WHO_AM_I);
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

/**
 * Configure Accelerometer cofniguration
 * @param	ACCEL_CONFIG - struct to register ACEL_CONFIG
 * 			Register Address - 0x14 - Bank 2
 * 			Reset Value = 0x01
 * return	value of register == config
 */
bool ICM20948::setAccelConfig(ACCEL_CONFIG &config) {
	_accel_config = config;
	uint8_t value = 0x00;
	value = _accel_config.getConfiguration();
	sprintf(debugBuf, "setAccelConfig: Value before: %x\r\n", value);
	uartFlush();
	switchUserBank(BANK2);

	writeByte(REGISTER_ACCEL_CONFIG, value);
	HAL_Delay(50);
	value = readByte(REGISTER_ACCEL_CONFIG);
	HAL_Delay(50);
	sprintf(debugBuf, "setAccelConfig: Value after: %x\r\n", value);
	uartFlush();

	return true;
}

void ICM20948::readAccelGyroRaw(uint8_t *buffer) {
	switchUserBank(BANK0);
	uint8_t raw[ACCEL_GYRO_RAW_BYTES_COUNT] = { 0 };
	readBytes(REGISTER_ACCEL_OUT, raw, ACCEL_GYRO_RAW_BYTES_COUNT);
	float accelGyroData[6];
	accelGyroData[0] = static_cast<int16_t>(((raw[0]) << 8) | raw[1])/ _accel_config.getSensitivityScaleFactor();
	accelGyroData[1] = static_cast<int16_t>(((raw[2]) << 8) | raw[3])/ _accel_config.getSensitivityScaleFactor();
	accelGyroData[2] = static_cast<int16_t>(((raw[4]) << 8) | raw[5])/ _accel_config.getSensitivityScaleFactor();


	accelGyroData[4] = static_cast<int16_t>(((raw[6]) << 8) | raw[7])/ _gyro_config_1.getSensitivityScaleFactor();
	accelGyroData[5] = static_cast<int16_t>(((raw[8]) << 8) | raw[9])/ _gyro_config_1.getSensitivityScaleFactor();
	accelGyroData[6] = static_cast<int16_t>(((raw[10]) << 8) | raw[11])/ _gyro_config_1.getSensitivityScaleFactor();

	sprintf(debugBuf, "%.3f,\t%.3f,\t%.3f,\t%.3f,\t%.3f,\t%.3f\r\n",
			accelGyroData[0],
			accelGyroData[1],
			accelGyroData[2],
			accelGyroData[3],
			accelGyroData[4],
			accelGyroData[5]);
	uartFlush();

}

void ICM20948::switchUserBank(const USERBANK &newBank) {
	if (_currentBank != newBank) {
		_currentBank = newBank;
		writeByte(REGISTER_BANK_SEL, _currentBank << 4);
	}
}

/**
 * Resets the internal registers of IMU
 * Register PWR_MGMT_1 - Address 0x06 - Bank 0
 * Reset Value: 0x41
 */

void ICM20948::reset(void) {
	switchUserBank(BANK0);
	writeByte(REGISTER_PWR_MGMT_1, BIT_RESET | 0x41);
	HAL_Delay(20);
	if (readByte(REGISTER_PWR_MGMT_1) == 0x41) {
		sprintf(debugBuf, "IMU 0x%x reset successful\r\n", _i2cAddressDebug);
	} else {
		sprintf(debugBuf, "IMU 0x%x reset unsuccessful\r\n", _i2cAddressDebug);
	}
	uartFlush();
	HAL_Delay(100);
}

void ICM20948::wakeUp(void) {
	switchUserBank(BANK0);
	uint8_t new_val = readByte(REGISTER_PWR_MGMT_1);
	new_val &= 0xBF;
	writeByte(REGISTER_PWR_MGMT_1, new_val);
	HAL_Delay(100);
}


void ICM20948::odrAlignEnable(void) {
	switchUserBank(BANK2);
	writeByte(REGISTER_ODR_ALIGN_EN, ENABLE);

}

void ICM20948::setClockSource(CLKSEL source) {
	switchUserBank(BANK0);
	uint8_t new_val = readByte(REGISTER_PWR_MGMT_1);
	new_val |= source;
	writeByte(REGISTER_PWR_MGMT_1, new_val);
	HAL_Delay(100);
}

void ICM20948::writeByte(const uint8_t registerAddress, uint8_t value) {
	HAL_I2C_Mem_Write(&hi2c1, static_cast<uint16_t>(_i2cAddress),
			static_cast<uint16_t>(registerAddress),
			static_cast<uint16_t>(sizeof(uint8_t)), &value, sizeof(uint8_t),
			HAL_MAX_DELAY);
}

uint8_t ICM20948::readByte(const uint8_t registerAddress) {
	uint8_t value = 0x00;
	HAL_I2C_Mem_Read(&hi2c1, static_cast<uint16_t>(_i2cAddress),
			static_cast<uint16_t>(registerAddress),
			static_cast<uint16_t>(sizeof(uint8_t)), &value, sizeof(value),
			HAL_MAX_DELAY);
	return value;
}

void ICM20948::readBytes(const uint8_t registerAddress, uint8_t *buffer,
		uint16_t bytes) {
	HAL_I2C_Mem_Read(&hi2c1, static_cast<uint16_t>(_i2cAddress),
			static_cast<uint16_t>(registerAddress),
			static_cast<uint16_t>(sizeof(uint8_t)), buffer, bytes,
			HAL_MAX_DELAY);

}
