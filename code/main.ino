#include <Wire.h>
#include <Adafruit_PN532.h>

#define SDA_PIN 8
#define SCL_PIN 9

Adafruit_PN532 nfc(-1, -1, &Wire);

uint8_t KEY_DEFAULT[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

void printHex(const uint8_t *data, uint8_t len) {
  for (uint8_t i = 0; i < len; i++) {
    if (data[i] < 0x10) Serial.print("0");
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
}

void decodeAccessBits(uint8_t trailer[16]) {
  uint8_t c1 = ((trailer[7] & 0x10) >> 4) | ((trailer[8] & 0x01) << 1) | ((trailer[8] & 0x10) >> 2);
  uint8_t c2 = ((trailer[7] & 0x20) >> 5) | ((trailer[8] & 0x02))       | ((trailer[8] & 0x20) >> 3);
  uint8_t c3 = ((trailer[7] & 0x40) >> 6) | ((trailer[8] & 0x04) >> 1) | ((trailer[8] & 0x40) >> 4);

  Serial.println("  Access Bits (C1 C2 C3):");
  for (uint8_t block = 0; block < 4; block++) {
    uint8_t b1 = (c1 >> block) & 1;
    uint8_t b2 = (c2 >> block) & 1;
    uint8_t b3 = (c3 >> block) & 1;

    Serial.print("   Bloque ");
    Serial.print(block);
    Serial.print(" → ");
    Serial.print(b1); Serial.print(" ");
    Serial.print(b2); Serial.print(" ");
    Serial.println(b3);
  }
}

void detectNTAG() {
  uint8_t page[4];

  if (!nfc.mifareultralight_ReadPage(3, page)) return;

  if (page[0] != 0xE1) {
    Serial.println("No CC válido → Ultralight genérico");
    return;
  }

  uint8_t mem = page[2];
  uint16_t bytes = mem * 8;

  Serial.print("NTAG detectado | Memoria útil: ");
  Serial.print(bytes);
  Serial.println(" bytes");

  if (bytes == 144) Serial.println("→ NTAG213");
  else if (bytes == 504) Serial.println("→ NTAG215");
  else if (bytes == 888) Serial.println("→ NTAG216");
  else Serial.println("→ NTAG desconocido");
}

void setup() {
  Serial.begin(115200);
  delay(3000);

  Wire.begin(SDA_PIN, SCL_PIN);

  Serial.println("\n=== NFC IDENT & AUDIT TOOL ===");

  nfc.begin();
  if (!nfc.getFirmwareVersion()) {
    Serial.println("PN532 NO detectado");
    while (1);
  }

  nfc.SAMConfig();
  Serial.println("Acerca una tarjeta NFC...\n");
}

void loop() {
  uint8_t uid[10];
  uint8_t uidLength;

  if (!nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
    delay(200);
    return;
  }

  Serial.println("=== TARJETA DETECTADA ===");
  Serial.print("UID: ");
  printHex(uid, uidLength);
  Serial.println();

  uint8_t data[16];

  // Intentar MIFARE Classic
  bool classic = nfc.mifareclassic_AuthenticateBlock(
    uid, uidLength, 0, 0, KEY_DEFAULT
  );

  if (classic) {
    Serial.println("\nTipo: MIFARE Classic");

    for (uint8_t sector = 0; sector < 16; sector++) {
      uint8_t trailer = sector * 4 + 3;

      if (!nfc.mifareclassic_AuthenticateBlock(uid, uidLength, trailer, 0, KEY_DEFAULT))
        continue;

      if (nfc.mifareclassic_ReadDataBlock(trailer, data)) {
        Serial.print("Sector ");
        Serial.print(sector);
        Serial.println(" Trailer:");
        printHex(data, 16);
        Serial.println();

        decodeAccessBits(data);
      }
    }
  } 
  else {
    Serial.println("\nNo Classic → probando NTAG / Ultralight");
    detectNTAG();
  }

  Serial.println("\nRetira la tarjeta...\n");
  delay(2500);
}
