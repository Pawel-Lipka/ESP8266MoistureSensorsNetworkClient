  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include "WiFiConnection.h"
  #include "MoistureSensor.h"

#define analog_Pin 0
#define digitalPowerPinForSensor 4
#define CONNECTION_ATTEMPTS 5

  

  String serverURL = "http://kwiatki.local/data";  
  HTTPClient http;
  WiFiClient client;
  MoistureSensor moistureSensor(analog_Pin,digitalPowerPinForSensor,700,200);


  void setup() {
    
    Serial.begin(9600);
    conectToWiFi();
    
  }

  void loop() {
    
      //read from sensor
      int rawValue = moistureSensor.readRaw();
      int percValue = moistureSensor.getMoisturePercent();

      String url = serverURL + "?id=" + String(ESP.getChipId()) + "&raw=" + rawValue + "&perc=" + percValue;

      http.begin(client,url);
      int httpCode = 0;
      int attempt = CONNECTION_ATTEMPTS;
      
      // try send data to server - Until response 200 from server or CONNECTION_ATTEMPTS times.
      do{
        
        httpCode = http.GET();

        if (httpCode != 200) {
          delay(10000);
          //String response = http.getString();
          //Serial.println("Odpowiedź serwera: " + response);
        } //else {
          //Serial.println("Błąd żądania: " + http.errorToString(httpCode));
          //delay(10000);
        //}
        
        attempt--;
    }
    while(httpCode != 200 && attempt > 0);

    http.end();
  
      //ESP.deepSleep(15e6);
    ESP.deepSleep(3600e6); // one hour
  
  }