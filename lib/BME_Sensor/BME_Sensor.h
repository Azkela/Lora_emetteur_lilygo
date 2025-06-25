#ifndef BME_SENSOR_H
#define BME_SENSOR_H

#include <Adafruit_BME280.h>
#include <Wire.h>

class BME_Sensor {
public:
  BME_Sensor();
  bool begin(uint8_t sda_pin, uint8_t scl_pin, uint8_t i2c_addr = 0x76);
  void read();
  float getTemperature();
  float getHumidity();
  float getPressure();

private:
  Adafruit_BME280 bme;
  float temperature;
  float humidity;
  float pressure;
};

#endif
