#include "rfid.h"


/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}



/*helper routine to print the hex values as readable text */
void dump_string(const byte *buffer, size_t bufferSize) {
     for (size_t i = 0; i < bufferSize; i++) {
        Serial.print(static_cast<char>(buffer[i]));
    }

}



/*This function takes in a block of bytes to be written in memory*/
void store_data(byte *data,MFRC522 rfid, MFRC522::MIFARE_Key key)
{   
        for (int sector = 1; sector<=15; sector++)
        {
            bool status = rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, (sector*4) + 3, &key, &rfid.uid);  
            byte blockAddr = sector * 4;  // Start block of the sector

            if(status)
            {  
                byte dataSize = sizeof(*data);
                byte numBytesWritten = 0;    
                
                while (numBytesWritten < dataSize)
                {

                    byte numBytesToWrite = min(dataSize-numBytesWritten,16);

                    
                    /*
                    blockAddr is the trailer block of the sector
                    data is a pointer to the entire data u want to write 
                    */
                    MFRC522::StatusCode status = rfid.MIFARE_Write(blockAddr,data+numBytesToWrite, numBytesWritten );
                    if(!status)
                    {
                        Serial.print("error writing to block");
                        break;
                    }

                    numBytesWritten += numBytesToWrite;
                    blockAddr ++;


                }

                if(numBytesWritten >= dataSize)
                {
                    break;
                }
            }    
        }
};



/*Takes in a block of bytes to be written in the specified block number -- do not input trailer blocks*/
void WriteDataToBlock(int blockNum, byte blockData[],MFRC522 mfrc522,MFRC522::MIFARE_Key key) 
{
  /* Authenticating the desired data block for write access using Key A */
  MFRC522::StatusCode status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Authentication failed for Write: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("Authentication success");
  }

  
  /* Write data to the block */
  status = mfrc522.MIFARE_Write(blockNum, blockData, 16);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Writing to Block failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("Data was written into Block successfully");
  }
  
};

/*reads data from a specific block number and saves it in the given array*/
void ReadDataFromBlock(int blockNum, byte readBlockData[],MFRC522 mfrc522,MFRC522::MIFARE_Key key) 
{
  /* Authenticating the desired data block for Read access using Key A */
  MFRC522::StatusCode status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK)
  {
     Serial.print("Authentication failed for Read: ");
     Serial.println(mfrc522.GetStatusCodeName(status));
     return;
  }
  else
  {
    Serial.println("Authentication success");
  }

  byte bufferLen = 18;

  /* Reading data from the Block */
  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen );
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("Block was read successfully");  
  }
};


//this function intializes the key as the default value for key A and B which is 0xFFFFFFh
void initKey(MFRC522::MIFARE_Key *key)
{
   for (byte i = 0; i < 6; i++) {
        (*key).keyByte[i] = 0xFF;
    }
}


void sendString(){

    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;
        http.begin(client,"http://192.168.220.1:8000");
        
        String postData = "";
        postData.concat(modelName);
        postData.concat(partNum);
        
        
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        
        int httpResponseCode = http.POST(postData);

        if (httpResponseCode > 0) {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
            String response = http.getString();
            Serial.println("Response: " + response);
        } else {
            Serial.print("Error sending POST request: ");
            Serial.println(httpResponseCode);
        }

        http.end();
    }
}

