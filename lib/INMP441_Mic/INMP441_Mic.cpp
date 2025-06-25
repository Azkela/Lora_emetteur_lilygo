#include "INMP441_Mic.h"
#include <math.h>

INMP441_Mic::INMP441_Mic() {}

bool INMP441_Mic::begin(int ws_pin, int sd_pin, int sck_pin) {
  const i2s_config_t i2s_config = {
      .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
      .sample_rate = 16000, .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
      .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
      .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
      .dma_buf_count = 8, .dma_buf_len = 64
  };
  const i2s_pin_config_t pin_config = {
      .bck_io_num = sck_pin, .ws_io_num = ws_pin,
      .data_out_num = I2S_PIN_NO_CHANGE, .data_in_num = sd_pin
  };
  
  if (i2s_driver_install(i2s_port, &i2s_config, 0, NULL) != ESP_OK) return false;
  if (i2s_set_pin(i2s_port, &pin_config) != ESP_OK) return false;

  return true;
}

void INMP441_Mic::read() {
  const int samples = 256;
  int16_t buffer[samples];
  size_t bytesRead;
  i2s_read(i2s_port, &buffer, sizeof(buffer), &bytesRead, portMAX_DELAY);
  
  double sum = 0;
  if (bytesRead > 0) {
    int samplesRead = bytesRead / sizeof(int16_t);
    for (int i = 0; i < samplesRead; i++) {
      sum += buffer[i] * buffer[i];
    }
    double rms = sqrt(sum / samplesRead);
    soundLevelDb = (rms > 0) ? (20 * log10(rms)) : 0.0;
  } else {
    soundLevelDb = 0.0;
  }
}

double INMP441_Mic::getSoundLevelDb() { return soundLevelDb; }