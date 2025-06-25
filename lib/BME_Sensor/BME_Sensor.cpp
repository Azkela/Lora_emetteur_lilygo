#include "BME_Sensor.h"

BME_Sensor::BME_Sensor() {}

bool BME_Sensor::begin(uint8_t sda_pin, uint8_t scl_pin, uint8_t i2c_addr) {
  Wire.begin(sda_pin, scl_pin);
  if (!bme.begin(i2c_addr)) {
    return false;
  }
  return true;
}

void BME_Sensor::read() {
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0F; // en hPa
}

float BME_Sensor::getTemperature() { return temperature; }
float BME_Sensor::getHumidity() { return humidity; }
float BME_Sensor::getPressure() { return pressure; }