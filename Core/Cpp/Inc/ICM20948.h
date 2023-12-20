/*
 * ICM20948.h
 *
 *  Created on: Nov 28, 2023
 *      Author: muhammad.inamulhaq
 */

#ifndef SRC_ICM20948_H_
#define SRC_ICM20948_H_

#include "stm32l4xx_hal.h"
#include "icm20948_common.h"


class ICM20948 {
public:
	ICM20948(uint8_t address);
	HAL_StatusTypeDef isDeviceConnected(void);
	uint8_t whoAmI(void);
	void reset(void);
	void wakeUp(void);
	void odrAlignEnable(FunctionalState state);
	void setClockSource(CLKSEL source);
	void setAccelConfig(ACCEL_CONFIG &config);
	void setGyroConfig1(GYRO_CONFIG_1 &config);
	void setGyroSmplRtDiv(uint16_t frequency);
	void setAccelSmplRtDiv(uint16_t frequency);
	void readAccelGyroRaw(float* accelGyroData);
	uint8_t getGyroSampleRateDivider(uint16_t frequency);
	uint16_t getAccelSampleRateDivider(uint16_t frequency);


private:

	// fields
	uint8_t _i2cAddress;
	uint8_t _i2cAddressDebug;
	USERBANK _currentBank;

	ACCEL_CONFIG _accel_config;
	GYRO_CONFIG_1 _gyro_config_1;

	// Methods
	inline void switchUserBank(const USERBANK &newBank);

	inline void writeByte(const uint8_t registerAddress, uint8_t value);
	inline uint8_t readByte(const uint8_t registerAddress);
	inline void readBytes(const uint8_t registerAddress, uint8_t* buffer, uint16_t count);


//	void I2C_writeByte(const uint8_t& regAddress);
//	uint8_t I2C_ReadByte(const uint8_t& regAddress);
//	void I2C_writeTwoBytes(const uint8_t& regAddress, const uint8_t& value);
};


#endif
