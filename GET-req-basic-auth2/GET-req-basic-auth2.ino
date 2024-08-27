#include "WiFi.h"
#include "HTTPClient.h"
#include "base64.h"
 
const char* ssid = "redmi12";
const char* password =  "9429571163";
 
String authUsername = "bhakti";
String authPassword = "123";
 
void setup() {
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to WiFi");
 
}
 
void loop() {
 
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
 
    HTTPClient http;
 
    http.begin("https://fleetkaptan.up.railway.app/api/rfid/esp-list");
 
    String auth = base64::encode(authUsername + ":" + authPassword);
    http.addHeader("Authorization", "Basic " + auth);
 
    int httpCode = http.GET(); 
 
    if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
    }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end();
  }
 
  delay(10000);
 
}