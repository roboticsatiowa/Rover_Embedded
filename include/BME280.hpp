// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// BME280
// This code is designed to work with the BME280_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Humidity?sku=BME280_I2CS#tabs-0-product_tabset-2

#ifndef BME280_H
#define BME280_H

#include <Wire.h>

// BME280 I2C address is 0x76(108)
#define Addr 0x76

class BME
{
private:
  double cTemp;
  unsigned int dig_T1, dig_P1, dig_H1, dig_H3;
  int dig_T2, dig_T3, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9, dig_H2, dig_H4, dig_H5, dig_H6;

  // Function to read 24 bytes of calibration data from the sensor
  void readCalibrationData()
  {
    unsigned int b1[24];
    for (int i = 0; i < 24; i++)
    {
      b1[i] = readRegister(136 + i);
    }

    // Assign calibration data to global variables
    dig_T1 = (b1[0] & 0xff) + ((b1[1] & 0xff) * 256);
    dig_T2 = b1[2] + (b1[3] * 256);
    dig_T3 = b1[4] + (b1[5] * 256);
    dig_P1 = (b1[6] & 0xff) + ((b1[7] & 0xff) * 256);
    dig_P2 = b1[8] + (b1[9] * 256);
    dig_P3 = b1[10] + (b1[11] * 256);
    dig_P4 = b1[12] + (b1[13] * 256);
    dig_P5 = b1[14] + (b1[15] * 256);
    dig_P6 = b1[16] + (b1[17] * 256);
    dig_P7 = b1[18] + (b1[19] * 256);
    dig_P8 = b1[20] + (b1[21] * 256);
    dig_P9 = b1[22] + (b1[23] * 256);
    dig_H1 = readRegister(161);
  }

  // Function to read humidity calibration data
  void readHumidityCalibrationData()
  {
    unsigned int b1[7];
    for (int i = 0; i < 7; i++)
    {
      b1[i] = readRegister(225 + i);
    }

    // Assign humidity calibration data to global variables
    dig_H2 = b1[0] + (b1[1] * 256);
    dig_H3 = b1[2] & 0xFF;
    dig_H4 = (b1[3] * 16) + (b1[4] & 0xF);
    dig_H5 = (b1[4] / 16) + (b1[5] * 16);
    dig_H6 = b1[6];
  }

  // Function to read a byte from a given register
  unsigned int readRegister(unsigned int reg)
  {
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select data register
    Wire.write(reg);
    // Stop I2C Transmission
    Wire.endTransmission();

    // Request 1 byte of data
    Wire.requestFrom(Addr, 1);

    // Return the read value
    if (Wire.available() == 1)
    {
      return Wire.read();
    }
    return 0;
  }

  // Function to set sensor configuration
  void setSensorConfig()
  {
    // Set humidity oversampling rate = 1
    writeRegister(0xF2, 0x01);
    // Set normal mode, temp and pressure oversampling rate = 1
    writeRegister(0xF4, 0x27);
    // Set standby time = 1000ms
    writeRegister(0xF5, 0xA0);
  }

  // Function to write a value to a given register
  void writeRegister(unsigned int reg, unsigned int value)
  {
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select control register
    Wire.write(reg);
    // Write value to register
    Wire.write(value);
    // Stop I2C Transmission
    Wire.endTransmission();
  }

  // Function to read sensor data
  void readSensorData(unsigned int *data)
  {
    for (int i = 0; i < 8; i++)
    {
      data[i] = readRegister(247 + i);
    }
  }

  // Function to calculate temperature in Celsius
  double calculateTemperature(long adc_t)
  {
    double var1 = (((double)adc_t) / 16384.0 - ((double)dig_T1) / 1024.0) * ((double)dig_T2);
    double var2 = ((((double)adc_t) / 131072.0 - ((double)dig_T1) / 8192.0) *
                   (((double)adc_t) / 131072.0 - ((double)dig_T1) / 8192.0)) *
                  ((double)dig_T3);
    double t_fine = (long)(var1 + var2);
    cTemp = (var1 + var2) / 5120.0;
    return t_fine;
  }

  // Function to calculate pressure in hPa
  double calculatePressure(long adc_p, double t_fine)
  {
    double var1 = (t_fine / 2.0) - 64000.0;
    double var2 = var1 * var1 * dig_P6 / 32768.0;
    var2 += var1 * dig_P5 * 2.0;
    var2 = (var2 / 4.0) + (dig_P4 * 65536.0);
    var1 = ((dig_P3 * var1 * var1 / 524288.0) + (dig_P2 * var1)) / 524288.0;
    var1 = (1.0 + var1 / 32768.0) * dig_P1;
    double pressure = 1048576.0 - adc_p;
    pressure = (pressure - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = dig_P9 * pressure * pressure / 2147483648.0;
    var2 = pressure * dig_P8 / 32768.0;
    pressure = (pressure + (var1 + var2 + dig_P7) / 16.0) / 100;
    return pressure;
  }

  // Function to calculate humidity in percentage
  double calculateHumidity(long adc_h, double t_fine)
  {
    double var_H = t_fine - 76800.0;
    var_H = (adc_h - (dig_H4 * 64.0 + dig_H5 / 16384.0 * var_H)) *
            (dig_H2 / 65536.0 * (1.0 + dig_H6 / 67108864.0 * var_H * (1.0 + dig_H3 / 67108864.0 * var_H)));
    double humidity = var_H * (1.0 - dig_H1 * var_H / 524288.0);
    if (humidity > 100.0)
    {
      humidity = 100.0;
    }
    else if (humidity < 0.0)
    {
      humidity = 0.0;
    }
    return humidity;
  }

  // Main function to get BME sensor information and display it
  void getBMEsensorInfo()
  {
    unsigned int data[8];
    readCalibrationData();
    readHumidityCalibrationData();
    setSensorConfig();
    readSensorData(data);

    // Convert raw data to actual values
    long adc_p = (((long)(data[0] & 0xFF) * 65536) + ((long)(data[1] & 0xFF) * 256) + (long)(data[2] & 0xF0)) / 16;
    long adc_t = (((long)(data[3] & 0xFF) * 65536) + ((long)(data[4] & 0xFF) * 256) + (long)(data[5] & 0xF0)) / 16;
    long adc_h = ((long)(data[6] & 0xFF) * 256 + (long)(data[7] & 0xFF));

    double t_fine = calculateTemperature(adc_t);
    double pressure = calculatePressure(adc_p, t_fine);
    double humidity = calculateHumidity(adc_h, t_fine);

    // Output data to serial monitor
    Serial.print("Temperature in Celsius : ");
    Serial.print(cTemp);
    Serial.println(" C");
    Serial.print("Temperature in Fahrenheit : ");
    Serial.print(cTemp * 1.8 + 32);
    Serial.println(" F");
    Serial.print("Pressure : ");
    Serial.print(pressure);
    Serial.println(" hPa");
    Serial.print("Relative Humidity : ");
    Serial.print(humidity);
    Serial.println(" RH");
    delay(1000);
  }

public:
  void getTemperature()
  {
    unsigned int data[8];
    readCalibrationData();
    readHumidityCalibrationData();
    setSensorConfig();
    readSensorData(data);

    long adc_t = (((long)(data[3] & 0xFF) * 65536) + ((long)(data[4] & 0xFF) * 256) + (long)(data[5] & 0xF0)) / 16;

    calculateTemperature(adc_t);
    return cTemp;
  }

  void getPressure()
  {
    unsigned int data[8];
    readCalibrationData();
    readHumidityCalibrationData();
    setSensorConfig();
    readSensorData(data);

    // Convert raw data to actual values
    long adc_p = (((long)(data[0] & 0xFF) * 65536) + ((long)(data[1] & 0xFF) * 256) + (long)(data[2] & 0xF0)) / 16;
    long adc_t = (((long)(data[3] & 0xFF) * 65536) + ((long)(data[4] & 0xFF) * 256) + (long)(data[5] & 0xF0)) / 16;

    double t_fine = calculateTemperature(adc_t);
    double pressure = calculatePressure(adc_p, t_fine);
    return pressure;
  }

  void getHumidity()
  {
    unsigned int data[8];
    readCalibrationData();
    readHumidityCalibrationData();
    setSensorConfig();
    readSensorData(data);

    // Convert raw data to actual values
    long adc_t = (((long)(data[3] & 0xFF) * 65536) + ((long)(data[4] & 0xFF) * 256) + (long)(data[5] & 0xF0)) / 16;
    long adc_h = ((long)(data[6] & 0xFF) * 256 + (long)(data[7] & 0xFF));

    double t_fine = calculateTemperature(adc_t);
    double humidity = calculateHumidity(adc_h, t_fine);

    return humidity;
  }
};

#endif
