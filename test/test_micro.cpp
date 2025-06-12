#include <Arduino.h>
#include <driver/i2s.h>
#include <LoRa.h>
#include <math.h> 

// Pins pour le micro I2S INMP441
#define I2S_WS 15 
#define I2S_SD 35 
#define I2S_SCK 14 

#define I2S_PORT I2S_NUM_0

#define LORA_SS    18
#define LORA_RST   23 
#define LORA_DIO0  26

// Configuration I2S
void setupI2S() {
  const i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 64
  };

  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_PIN_NO_CHANGE, // Pas de sortie
    .data_in_num = I2S_SD
  };

  esp_err_t err = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  if (err != ESP_OK) {
    Serial.printf("Erreur d'installation du driver I2S: %d\n", err);
    while (true);
  }
  
  err = i2s_set_pin(I2S_PORT, &pin_config);
    if (err != ESP_OK) {
    Serial.printf("Erreur de configuration des pins I2S: %d\n", err);
    while (true);
  }
  Serial.println("I2S driver installé.");
}

// Configuration LoRa
void setupLoRa() {
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(868E6)) { // 868 MHz pour l'Europe
    Serial.println("Échec de l'initialisation LoRa !");
    while (true);
  }
  LoRa.setTxPower(20); // Puissance max
  Serial.println("Initialisation LoRa réussie.");
}

void setup() {
  Serial.begin(115200);
  while(!Serial); // Attendre que le port série soit prêt
  Serial.println("Démarrage du capteur de son LoRa...");
  setupI2S();
  setupLoRa();
}

void loop() {
  const int samples = 256;
  int16_t buffer[samples];
  size_t bytesRead;


  i2s_read(I2S_PORT, &buffer, sizeof(buffer), &bytesRead, portMAX_DELAY);
  int samplesRead = bytesRead / sizeof(int16_t);


  double sum = 0;
  for (int i = 0; i < samplesRead; i++) {
    long long sample = buffer[i];
    sum += sample * sample;
  }
  double rms = sqrt(sum / samplesRead);


  double db = 0.0;
  if (rms > 0) { // Éviter log10(0)
      db = 20 * log10(rms);
  }

 
  Serial.print("RMS: ");
  Serial.print(rms);
  Serial.print(" | dB: ");
  Serial.println(db);

  LoRa.beginPacket();
  LoRa.print("Niveau sonore: " + String(db, 2) + " dB");
  LoRa.endPacket();

  delay(1000);
}