/* MPU6050_light library for Arduino
 *
 * Authors: Romain JL. Fétick (github.com/rfetick)
 *              simplifications and corrections
 *          Tockn (github.com/tockn)
 *              initial author (v1.5.2)
 */

#include "MPU6050_light.h"
#include "Arduino.h"

#define GYRO_COEFF            0.6
#define NEW_DATA_WEIGHT_GYRO  0.8
#define NEW_DATA_WEIGHT_ACC   1.0

/* INIT and BASIC FUNCTIONS */

MPU6050::MPU6050(TwoWire &w)
{
  wire = &w;
  setGyroOffsets(0, 0, 0);
  setAccOffsets(0, 0, 0);
}

byte MPU6050::begin(int gyro_config_num, int acc_config_num)
{
  // changed calling register sequence [https://github.com/rfetick/MPU6050_light/issues/1] -> thanks to augustosc
  byte status = writeData(MPU6050_PWR_MGMT_1_REGISTER, 0x01); // check only the first connection with status
  writeData(MPU6050_SMPLRT_DIV_REGISTER, 0x00);
  writeData(MPU6050_CONFIG_REGISTER, 0x00);
  setGyroConfig(gyro_config_num);
  setAccConfig(acc_config_num);

  this->update();
  angleX = this->getAccAngleX();
  angleY = this->getAccAngleY();
  prevInterval = millis(); // may cause lack of angular accuracy if begin() is much before the first update()
  return status;
}

byte MPU6050::writeData(byte reg, byte data)
{
  wire->beginTransmission(address);
  wire->write(reg);
  wire->write(data);
  byte status = wire->endTransmission();
  return status; // 0 if success
}

// This method is not used internaly, maybe by user...
byte MPU6050::readData(byte reg)
{
  wire->beginTransmission(address);
  wire->write(reg);
  wire->endTransmission(true);
  wire->requestFrom(address, (uint8_t)1);
  byte data = wire->read();
  return data;
}

/* SETTER */

byte MPU6050::setGyroConfig(int config_num)
{
  byte status;
  switch (config_num)
  {
  case 0: // range = +- 250 deg/s
    gyro_lsb_to_degsec = 131.0;
    status = writeData(MPU6050_GYRO_CONFIG_REGISTER, 0x00);
    break;
  case 1: // range = +- 500 deg/s
    gyro_lsb_to_degsec = 65.5;
    status = writeData(MPU6050_GYRO_CONFIG_REGISTER, 0x08);
    break;
  case 2: // range = +- 1000 deg/s
    gyro_lsb_to_degsec = 32.8;
    status = writeData(MPU6050_GYRO_CONFIG_REGISTER, 0x10);
    break;
  case 3: // range = +- 2000 deg/s
    gyro_lsb_to_degsec = 16.4;
    status = writeData(MPU6050_GYRO_CONFIG_REGISTER, 0x18);
    break;
  default: // error
    status = 1;
    break;
  }
  return status;
}

byte MPU6050::setAccConfig(int config_num)
{
  byte status;
  switch (config_num)
  {
  case 0: // range = +- 2 g
    acc_lsb_to_g = 16384.0;
    status = writeData(MPU6050_ACCEL_CONFIG_REGISTER, 0x00);
    break;
  case 1: // range = +- 4 g
    acc_lsb_to_g = 8192.0;
    status = writeData(MPU6050_ACCEL_CONFIG_REGISTER, 0x08);
    break;
  case 2: // range = +- 8 g
    acc_lsb_to_g = 4096.0;
    status = writeData(MPU6050_ACCEL_CONFIG_REGISTER, 0x10);
    break;
  case 3: // range = +- 16 g
    acc_lsb_to_g = 2048.0;
    status = writeData(MPU6050_ACCEL_CONFIG_REGISTER, 0x18);
    break;
  default: // error
    status = 1;
    break;
  }
  return status;
}

void MPU6050::setGyroOffsets(float x, float y, float z)
{
  gyroXoffset = x;
  gyroYoffset = y;
  gyroZoffset = z;
}

void MPU6050::setAccOffsets(float x, float y, float z)
{
  accXoffset = x;
  accYoffset = y;
  accZoffset = z;
}

/* CALC OFFSET */

void MPU6050::calcOffsets()
{
  setGyroOffsets(0, 0, 0);
  setAccOffsets(0, 0, 0);
  float ag[6] = {0, 0, 0, 0, 0, 0}; // 3*acc, 3*gyro

  for (int i = 0; i < CALIB_OFFSET_NB_MES; i++)
  {
    this->fetchData();
    ag[0] += accX;
    ag[1] += accY;
    ag[2] += (accZ - 1.0);
    ag[3] += gyroX;
    ag[4] += gyroY;
    ag[5] += gyroZ;
    delay(2); // wait a little bit between 2 measurements
  }

  accXoffset = ag[0] / CALIB_OFFSET_NB_MES;
  accYoffset = ag[1] / CALIB_OFFSET_NB_MES;
  accZoffset = ag[2] / CALIB_OFFSET_NB_MES;

  gyroXoffset = ag[3] / CALIB_OFFSET_NB_MES;
  gyroYoffset = ag[4] / CALIB_OFFSET_NB_MES;
  gyroZoffset = ag[5] / CALIB_OFFSET_NB_MES;
}

/* UPDATE */

void MPU6050::fetchData()
{
  wire->beginTransmission(address);
  wire->write(MPU6050_ACCEL_OUT_REGISTER);
  wire->endTransmission(false);
  wire->requestFrom(address, (uint8_t)14);

  int16_t rawData[7]; // [ax,ay,az,temp,gx,gy,gz]

  for (int i = 0; i < 7; i++)
  {
    rawData[i] = wire->read() << 8;
    rawData[i] |= wire->read();
  }

  float newAccX = ((float)rawData[0]) / acc_lsb_to_g - accXoffset;
  float newAccY = ((float)rawData[1]) / acc_lsb_to_g - accYoffset;
  float newAccZ = ((float)rawData[2]) / acc_lsb_to_g - accZoffset;
  accX = NEW_DATA_WEIGHT_ACC * newAccX + (1 - NEW_DATA_WEIGHT_ACC) * accX;
  accY = NEW_DATA_WEIGHT_ACC * newAccY + (1 - NEW_DATA_WEIGHT_ACC) * accY;
  accZ = NEW_DATA_WEIGHT_ACC * newAccZ + (1 - NEW_DATA_WEIGHT_ACC) * accZ;

  temp = (rawData[3] + TEMP_LSB_OFFSET) / TEMP_LSB_2_DEGREE;
  float newGyroX = ((float)rawData[4]) / gyro_lsb_to_degsec - gyroXoffset;
  float newGyroY = ((float)rawData[5]) / gyro_lsb_to_degsec - gyroYoffset;
  float newGyroZ = ((float)rawData[6]) / gyro_lsb_to_degsec - gyroZoffset;
  gyroX = NEW_DATA_WEIGHT_GYRO * newGyroX + (1 - NEW_DATA_WEIGHT_GYRO) * gyroX;
  gyroY = NEW_DATA_WEIGHT_GYRO * newGyroY + (1 - NEW_DATA_WEIGHT_GYRO) * gyroY;
  gyroZ = NEW_DATA_WEIGHT_GYRO * newGyroZ + (1 - NEW_DATA_WEIGHT_GYRO) * gyroZ;
}

void MPU6050::update()
{
  // retrieve raw data
  this->fetchData();

  angleAccX = atan2(accY, accZ) * RAD_2_DEG;
  angleAccY = atan2(-accX, sqrt(accY * accY + accZ * accZ)) * RAD_2_DEG;
  
  unsigned long Tnew = millis();
  float dt = (Tnew - prevInterval) * 1e-3;
  prevInterval = Tnew;

  angleX = GYRO_COEFF * (angleX - gyroX * dt) + (1 - GYRO_COEFF) * angleAccX;
  angleY = GYRO_COEFF * (angleY - gyroY * dt) + (1 - GYRO_COEFF) * angleAccY;
}
