#ifndef RFID_H 
#define RFID_H
#include <Arduino.h>
#include <MFRC522.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <config.h>



namespace RFIDConstants {
    const byte numSectors = 16;
    const byte blocksPerSector = 4;
    const byte blockSize = 16;
};

void dump_byte_array(byte *buffer, byte bufferSize); //helper function to print memory to serial


/*
    writes data to the tag, going to the next sector if one isnt enough--not working yet 
    FIXME:dynamically allocate data
*/


void store_data(byte *data,MFRC522 rfid, MFRC522::MIFARE_Key key);


void ReadDataFromBlock(int blockNum, byte readBlockData[],MFRC522 mfrc522,MFRC522::MIFARE_Key key) ;

void WriteDataToBlock(int blockNum, byte blockData[],MFRC522 mfrc522,MFRC522::MIFARE_Key key) ;

void dump_string(const byte *buffer, size_t bufferSize) ;

void initKey(MFRC522::MIFARE_Key *key);

void sendString();

#endif