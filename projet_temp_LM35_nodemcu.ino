#define __ESP8266_MQTT_H__

#include <ESP8266WiFi.h> // ESP8266WiFi.h library
#include <SPI.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <universal-mqtt.h>
//#include <WiFiClientSecureBearSSL.h>
#include <rBase64.h>
#include <MQTT.h>

#include <CloudIoTCore.h>
#include <CloudIoTCoreMqtt.h>

String payload;


//const char* ssid     = "HG531V1-SHNK85";//"Infinix S5 Lite";//"supinfo";
//const char* password = "A2WHNUE6VJ";//"kimmyjoke";//"20202021";


//On créé une instance qui contiendra le serveur web sur le port 80
ESP8266WebServer server(80); // declaration du serveur web

const int LM_35=A0;
int input_val=0;
float celsius=0;
float farnheit=0;

unsigned long lastMillis = 0;

// Place your message handler code here.
String topic="Monitoring_Node_MCU";

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}



void setup() {
  // Initialisation du port serie
  Serial.begin(115200);

  delay(1000);
  Serial.println("Connection au ");
       Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  //  Connection au WiFi 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connecté au wifi");
  server.begin();// on démarre le serveur web
  Serial.println("Web server lancé!");


  setupCloudIoT();  
}

void loop() {
  WiFiClient client;
  input_val = analogRead(A0);
  celsius=(input_val*500.0 / 1023.0);
  farnheit=1.8*celsius+32;
  


  if (isnan(input_val)) {
    return;
  }



  Serial.print("Affichage des mesures de temperature :");
  Serial.print("En celsius:");
  Serial.print(celsius);
  Serial.print("\n");
  Serial.print("En farnheit :");
  Serial.println(farnheit);
  delay(1000);

  mqttClient->loop();

  if (!mqttClient->connected()) {
    connect();
    Serial.println("je suis la");
  }
 
 
  if (millis() - lastMillis > 60000) {
    lastMillis = millis();

    payload =
      String("{\"timestamp\":") + time(nullptr) +
      String(",\"temperature\":") + celsius +
      String("}");
    publishTelemetry(payload);
  }
  
  messageReceived(topic,payload); 
  delay(50000);
}