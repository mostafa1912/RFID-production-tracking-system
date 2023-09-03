#include "rfid.h"

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

MFRC522::MIFARE_Key key;




/**
 * Initialize.
 */
void setup() {
    Serial.begin(19200); // Initialize serial communications with the PC
    SPI.begin();        // Init SPI bus
    WiFi.begin(ssid,password);

    while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    }



    mfrc522.PCD_Init(); // Init MFRC522 card

    // Prepare the key (used both as key A and as key B)
    // using FFFFFFFFFFFFh which is the default at chip delivery from the factory
    initKey(&key);
}

void loop() {
    
     // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if ( ! mfrc522.PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;

    // Show some details of the PICC (that is: the tag/card)
    Serial.print(F("Card UID:"));
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();


    byte data1[7] = {0}; byte data2[9] = {0};

    memcpy(data1, modelName, sizeof(modelName) - 1); // Copy the string without null-termination
    memcpy(data2, partNum, sizeof(partNum) - 1);     // Copy the string without null-termination



    byte readData1[18];
    byte readData2[18];

    Serial.print("trying to Store: ");dump_byte_array(data1,sizeof(data1));dump_byte_array(data2,sizeof(data2));
    Serial.println();
    //forget about writing in sector 0 -- start from sector 1 with block 7 as trailing block

    WriteDataToBlock(4,data1,mfrc522,key);
    WriteDataToBlock(5,data2,mfrc522,key);

    Serial.println();
    Serial.print("written: ");Serial.println();dump_byte_array(data1,sizeof(data1));
    Serial.println();
    dump_byte_array(data2,sizeof(data2));
    Serial.println();

    ReadDataFromBlock(4,readData1,mfrc522,key);

    ReadDataFromBlock(5,readData2,mfrc522,key);

    Serial.println();
    Serial.print("Read: "); Serial.println();dump_string(readData1,sizeof(readData1));
    Serial.println();
    dump_string(readData2,sizeof(readData2));
    // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();


    sendString();
    
    delay(3000);
}



