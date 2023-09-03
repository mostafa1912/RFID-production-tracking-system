#ifndef CONFIG_H 
#define CONFIG_H

#include <Arduino.h>


#define RST_PIN         D3           // Configurable, see typical pin layout above
#define SS_PIN          D4          // Configurable, see typical pin layout above

#define modelName "COCF60_"
#define partNum "123456789"
 
#define ssid "pi4"
#define password "hello123"


#define SERVER_URL "pi.local:8000"
#define DATA_TO_SEND "Hello from ESP8266!"

#endif