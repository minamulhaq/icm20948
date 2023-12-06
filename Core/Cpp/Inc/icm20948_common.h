/*
 * icm20948_common.h
 *
 *  Created on: Nov 28, 2023
 *      Author: muhammad.inamulhaq
 */

#ifndef INC_ICM20948_COMMON_H_
#define INC_ICM20948_COMMON_H_

#define WHOAMI 0xEA

#define ACCEL_GYRO_RAW_BYTES_COUNT 0x06

typedef enum {
	BANK0,
	BANK1,
	BANK2,
	BANK3,
} USERBANK;


typedef enum {
	INTERNAL_20_MHZ,
	AUTO_CLK_SELECT,
	TIMING_GENERATOR_IN_RESET
} CLKSEL;

/*
typedef enum FCHOICE {
  BYPASS_DLPF = 0,
  ENALBE_DLPF
} FCHOICE;
*/



typedef enum {
  DLPF_0,
  DLPF_1,
  DLPF_2,
  DLPF_3,
  DLPF_4,
  DLPF_5,
  DLPF_6,
  DLPF_7,
  DLPF_OFF
} DLPF;

typedef enum GYRO_FS_RANGE {
  GYRO_FS_RANGE_250,
  GYRO_FS_RANGE_500,
  GYRO_FS_RANGE_1000,
  GYRO_FS_RANGE_2000
} GYRO_FS_RANGE;

typedef enum ACCEL_FS_SEL {
  ACCEL_FS_RANGE_2G,
  ACCEL_FS_RANGE_4G,
  ACCEL_FS_RANGE_8G,
  ACCEL_FS_RANGE_16G
} ACCEL_FS_SEL;



typedef enum ICM20948_WOM_COMP {
  ICM20948_WOM_COMP_DISABLE,
  ICM20948_WOM_COMP_ENABLE
} ICM20948_WOM_COMP;


typedef enum SELF_TEST {
  SELF_TEST_DISABLED = 0,
  SELF_TEST_ENABLED
} SELF_TEST;


typedef enum ACCEL_AVG {
  ACCEL_AVERAGE_1_OR_4 = 0,
  ACCEL_AVERAGE_8,
  ACCEL_AVERAGE_16,
  ACCEL_AVERAGE_32
} ACCEL_AVG;

typedef struct {
  DLPF accel_dlpfcfg = DLPF_0;
  ACCEL_FS_SEL accel_fs_sel = ACCEL_FS_RANGE_2G;
  FunctionalState accel_fchoice = ENABLE;
public:
  uint8_t getConfiguration(void) {
    return ((accel_dlpfcfg << 3) | (accel_fs_sel << 1) | accel_fchoice);
  }

  float getSensitivityScaleFactor(void) {
    switch (accel_fs_sel) {
      case ACCEL_FS_RANGE_2G:
        return 16384.0;
      case ACCEL_FS_RANGE_4G:
        return 8192.0;
      case ACCEL_FS_RANGE_8G:
        return 4096.0;
      case ACCEL_FS_RANGE_16G:
        return 2048.0;
      default:
        return 16384.0;
    }
  }
} ACCEL_CONFIG;


typedef struct {
  DLPF gyro_dlpfcfg = DLPF_0;
  GYRO_FS_RANGE gyro_fs_sel = GYRO_FS_RANGE_250;
  FunctionalState gyro_fchoice = ENABLE;

public:
  uint8_t getConfiguration(void) {
    return ((gyro_dlpfcfg << 3) | (gyro_fs_sel << 1) | gyro_fchoice);
  }

  float getSensitivityScaleFactor(void) {
    switch (gyro_fs_sel) {
      case GYRO_FS_RANGE_250:
        return 131.0;
      case GYRO_FS_RANGE_500:
        return 65.5;
      case GYRO_FS_RANGE_1000:
        return 32.8;
      case GYRO_FS_RANGE_2000:
        return 16.4;
      default:
        return 131.0;
    }
  }

} GYRO_CONFIG_1;

typedef struct ACCEL_CONFIG_2 {
  SELF_TEST ax_st_en_reg = SELF_TEST_DISABLED;
  SELF_TEST ay_st_en_reg = SELF_TEST_DISABLED;
  SELF_TEST az_st_en_reg = SELF_TEST_DISABLED;
  ACCEL_AVG dec3_cfg = ACCEL_AVERAGE_1_OR_4;
public:
  uint8_t getConfiguration(void) {
    return ((ax_st_en_reg << 4) | (ay_st_en_reg << 3) | (az_st_en_reg << 2) | dec3_cfg);
  }

} ACCEL_CONFIG_2;

typedef enum GYRO_AVG {
  GYRO_AVG_1 = 0,
  GYRO_AVG_2,
  GYRO_AVG_4,
  GYRO_AVG_8,
  GYRO_AVG_16,
  GYRO_AVG_32,
  GYRO_AVG_64,
  GYRO_AVG_128

} GYRO_AVG;

typedef struct GYRO_CONFIG_2 {
  SELF_TEST xgyro_cten = SELF_TEST_DISABLED;
  SELF_TEST ygyro_cten = SELF_TEST_DISABLED;
  SELF_TEST zgyro_cten = SELF_TEST_DISABLED;
  GYRO_AVG gyro_avgcfg = GYRO_AVG_1;
public:
  uint8_t getConfiguration(void) {
    return ((xgyro_cten << 5) | (ygyro_cten << 4) | (zgyro_cten << 3) | gyro_avgcfg);
  }

} GYRO_CONFIG_2;




/* Registers ICM20948 USER BANK 0*/
#define REGISTER_WHO_AM_I 0x00
#define REGISTER_USER_CTRL 0x03
#define REGISTER_LP_CONFIG 0x05
#define REGISTER_PWR_MGMT_1 0x06
#define REGISTER_PWR_MGMT_2 0x07
#define REGISTER_INT_PIN_CFG 0x0F
#define REGISTER_INT_ENABLE 0x10
#define REGISTER_INT_ENABLE_1 0x11
#define REGISTER_INT_ENABLE_2 0x12
#define REGISTER_INT_ENABLE_3 0x13
#define REGISTER_I2C_MST_STATUS 0x17
#define REGISTER_INT_STATUS 0x19
#define REGISTER_INT_STATUS_1 0x1A
#define REGISTER_INT_STATUS_2 0x1B
#define REGISTER_INT_STATUS_3 0x1C
#define REGISTER_DELAY_TIME_H 0x28
#define REGISTER_DELAY_TIME_L 0x29
#define REGISTER_ACCEL_OUT 0x2D  // accel data registers begin
#define REGISTER_GYRO_OUT 0x33   // gyro data registers begin
#define REGISTER_TEMP_OUT 0x39
#define REGISTER_EXT_SLV_SENS_DATA_00 0x3B
#define REGISTER_EXT_SLV_SENS_DATA_01 0x3C
#define REGISTER_FIFO_EN_1 0x66
#define REGISTER_FIFO_EN_2 0x67
#define REGISTER_FIFO_RST 0x68
#define REGISTER_FIFO_MODE 0x69
#define REGISTER_FIFO_COUNT 0x70
#define REGISTER_FIFO_R_W 0x72
#define REGISTER_DATA_RDY_STATUS 0x74
#define REGISTER_FIFO_CFG 0x76

/* Registers ICM20948 USER BANK 1*/
#define REGISTER_SELF_TEST_X_GYRO 0x02
#define REGISTER_SELF_TEST_Y_GYRO 0x03
#define REGISTER_SELF_TEST_Z_GYRO 0x04
#define REGISTER_SELF_TEST_X_ACCEL 0x0E
#define REGISTER_SELF_TEST_Y_ACCEL 0x0F
#define REGISTER_SELF_TEST_Z_ACCEL 0x10
#define REGISTER_XA_OFFS_H 0x14
#define REGISTER_XA_OFFS_L 0x15
#define REGISTER_YA_OFFS_H 0x17
#define REGISTER_YA_OFFS_L 0x18
#define REGISTER_ZA_OFFS_H 0x1A
#define REGISTER_ZA_OFFS_L 0x1B
#define REGISTER_TIMEBASE_CORR_PLL 0x28

/* Registers ICM20948 USER BANK 2*/
#define REGISTER_GYRO_SMPLRT_DIV 0x00
#define REGISTER_GYRO_CONFIG_1 0x01
#define REGISTER_GYRO_CONFIG_2 0x02
#define REGISTER_XG_OFFS_USRH 0x03
#define REGISTER_XG_OFFS_USRL 0x04
#define REGISTER_YG_OFFS_USRH 0x05
#define REGISTER_YG_OFFS_USRL 0x06
#define REGISTER_ZG_OFFS_USRH 0x07
#define REGISTER_ZG_OFFS_USRL 0x08
#define REGISTER_ODR_ALIGN_EN 0x09
#define REGISTER_ACCEL_SMPLRT_DIV_1 0x10
#define REGISTER_ACCEL_SMPLRT_DIV_2 0x11
#define REGISTER_ACCEL_INTEL_CTRL 0x12
#define REGISTER_ACCEL_WOM_THR 0x13
#define REGISTER_ACCEL_CONFIG 0x14
#define REGISTER_ACCEL_CONFIG_2 0x15
#define REGISTER_FSYNC_CONFIG 0x52
#define REGISTER_TEMP_CONFIG 0x53
#define REGISTER_MOD_CTRL_USR 0x54

/* Registers ICM20948 USER BANK 3*/
#define REGISTER_I2C_MST_ODR_CFG 0x00
#define REGISTER_I2C_MST_CTRL 0x01
#define REGISTER_I2C_MST_DELAY_CTRL 0x02
#define REGISTER_I2C_SLV0_ADDR 0x03
#define REGISTER_I2C_SLV0_REG 0x04
#define REGISTER_I2C_SLV0_CTRL 0x05
#define REGISTER_I2C_SLV0_DO 0x06

/* Registers ICM20948 ALL BANKS */
#define REGISTER_BANK_SEL 0x7F



/* Register Bits */
#define BIT_RESET 0x80
#define BIT_I2C_MST_EN 0x20
#define BIT_SLEEP 0x40
#define BIT_LP_EN 0x20
#define BIT_BYPASS_EN 0x02
#define BIT_GYR_EN 0x07
#define BIT_ACC_EN 0x38
#define BIT_FIFO_EN 0x40
#define BIT_INT1_ACTL 0x80
#define BIT_INT_1_LATCH_EN 0x20
#define BIT_ACTL_FSYNC 0x08
#define BIT_INT_ANYRD_2CLEAR 0x10
#define BIT_FSYNC_INT_MODE_EN 0x06




#endif /* INC_ICM20948_COMMON_H_ */
