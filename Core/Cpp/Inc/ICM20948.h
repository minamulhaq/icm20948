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
	bool setAccelConfig(const ACCEL_CONFIG& config);
private:

	// fields
	uint8_t _i2cAddress;
	uint8_t _i2cAddressDebug;
	USERBANK _currentBank;

	ACCEL_CONFIG _accel_config;


	// Methods
	void switchUserBank (const USERBANK& newBank);


	void I2C_writeByte(const uint8_t& regAddress);
	uint8_t I2C_ReadByte(const uint8_t& regAddress);
	void I2C_writeTwoBytes(const uint8_t& regAddress, const uint8_t& value);
};

#endif /* SRC_ICM20948_H_ */
