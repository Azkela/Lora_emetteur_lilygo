// ===== CODE FINAL POUR LILYGO T-BEAM =====

#include <LoRa.h>

// --- MODIFICATION ---
// On assigne le port Série 2 aux broches GPIO 13 et 14 qui sont libres
#define RXD2 13
#define TXD2 14

void setup() {
  // Le port série USB, pour afficher ce qu'on reçoit
  Serial.begin(115200);
  
  // Initialisation du port série qui reçoit les données du Mega sur les nouvelles broches
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // Initialisation LoRa
  // ATTENTION: Les pins pour LoRa sont spécifiques à la T-Beam et ne doivent pas être changées.
  // D'après votre schéma : SS=18, RST=23, DIO0=26.
  LoRa.setPins(18, 23, 26);
  if (!LoRa.begin(868E6)) { // 868 MHz pour l'Europe
    Serial.println("Erreur de démarrage LoRa !");
    while (1);
  }
  LoRa.setTxPower(20);
  Serial.println("LILYGO prêt à recevoir les données du Mega sur GPIO13 et à émettre en LoRa.");
}

void loop() {
  // Est-ce qu'il y a des données qui arrivent du Mega sur Serial2 ?
  if (Serial2.available() > 0) {
    // Lire la ligne de texte envoyée par le Mega (ex: "0.591")
    String dustValue = Serial2.readStringUntil('\n');
    dustValue.trim(); // Enlever les espaces ou caractères invisibles

    if (dustValue.length() > 0) {
      // Afficher sur le moniteur USB ce qu'on a reçu
      Serial.print("Reçu du Mega: '");
      Serial.print(dustValue);
      Serial.println("'");

      // Envoyer cette valeur via LoRa
      LoRa.beginPacket();
      LoRa.print("Poussiere: " + dustValue + " V");
      LoRa.endPacket();
      
      Serial.println("Donnée transmise via LoRa.");
    }
  }
}