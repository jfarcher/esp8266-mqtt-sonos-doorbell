#include "config.h"

#include <PubSubClient.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>




const int dbPin = 5;   
long lastDebounceTime = 0;
long debounceDelay = 500;



WiFiClient ESPClient;
PubSubClient client(ESPClient);


void setup() {
  pinMode(dbPin, INPUT);
  Serial.begin(115200);
  setup_wifi();
  setup_OTA();
  client.setServer(mqtt_server, 1883);

}
void setup_wifi() {  
  WiFi.mode(WIFI_STA);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(5000);
    ESP.restart(); 
  }
  Serial.println(WiFi.localIP());
}

void setup_OTA() {
  delay(10);
  ArduinoOTA.setHostname("doorbell");

  ArduinoOTA.onStart([](){
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([](){
    Serial.println("\nEnd");
  });
 ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
});
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
ArduinoOTA.begin();
   

}


void reconnect() {
  while (!client.connected()){
    Serial.println("Attempting MQTT connection.....");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");      


    } else{
      Serial.println("Failed, rc=");
      Serial.println(client.state());
      delay(5000);
    }
  }
}




#define mqtt_topic (String("house/doorbell/front").c_str())

void loop() {
    ArduinoOTA.handle();

  if (!client.connected()) {
    reconnect();
  }

  
  int dbin = digitalRead(dbPin);


  if ((millis() - lastDebounceTime) > debounceDelay) {
    
      if (dbin == HIGH) {
    
            client.publish(mqtt_topic, String(dbin).c_str(), false);
Serial.println(dbin);
              delay(5000);

    
    }
  }

 

}

