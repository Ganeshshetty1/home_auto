#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiSTA.h>
#include <WiFiType.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>

#include <Adafruit_Sensor.h>


#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP32.h>


char ssid[] = "Ganesh";
char wifiPassword[] = "123456789";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "dc29d610-8b2a-11ed-b193-d9789b2af62b";
char password[] = "eabe83e8271e008c875763b1da31343db7129dbd";
char clientID[] = "9157af40-8b2f-11ed-b193-d9789b2af62b";

unsigned long lastMillis = 0;
#include "DHT.h"

#define DHTPIN T7     

#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

float h,t,f,hic,hif;
void setup() {
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID, ssid, wifiPassword);
  pinMode(T5,OUTPUT);

 
}

void loop() {
	Cayenne.loop();

	
	if (millis() - lastMillis > 10000) {
		lastMillis = millis();
  Cayenne.virtualWrite(0, h);
  Cayenne.virtualWrite(1, t);
  Cayenne.virtualWrite(2, hic);
	}
   h = dht.readHumidity();
  
  t = dht.readTemperature();
  
  
   f = dht.readTemperature(true);

  
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  hif = dht.computeHeatIndex(f, h);
  
  hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
  
	
}
CAYENNE_IN(3)
  {
    
    int currentValue=getValue.asInt();
    if(currentValue==1){
      digitalWrite(T5,HIGH);
      }
      else{
       digitalWrite(T5,LOW); 
      }       }




//Default function for processing actuator commands from the Cayenne Dashboard.
//You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
