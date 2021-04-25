#include <SPI.h>
#include<MFRC522.h>// Affectation des broches
#define RST_PIN 16
#define SS_PIN 15
MFRC522 mfrc522(SS_PIN, RST_PIN);

voidsetup() {
  // Initialisation du Module RFID
  Serial.begin(9600);while(!Serial);SPI.begin();mfrc522.PCD_Init();mfrc522.PCD_DumpVersionToSerial(); 
  // Affichage des données de la bibliothèque
  Serial.println(F("Scan PICC to see UID, type, and data blocks..."));}
  void loop() {
    // Attente d'une carte RFID
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;}
    // Récupération des informations de la carte RFID
    if( ! mfrc522.PICC_ReadCardSerial()) 
    {return;}
    // Affichagedes informations de la carte RFID
    mfrc522.PICC_DumpToSerial(&(mfrc522.uid));}
}
