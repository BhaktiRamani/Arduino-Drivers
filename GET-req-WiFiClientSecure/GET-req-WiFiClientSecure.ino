#include <WiFi.h>
#include <base64.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

const char* ssid = "redmi12";
const char* password = "9429571163";

String authusername = "bhakti";
String authpassword = "123";
const char* uqid = "u9nm3eog8h4h";

//String uqid = "wafd0lf7eluu";

const char* server = "fleetkaptan.up.railway.app";


WiFiClientSecure client;

void setup() {

  Serial.begin(9600);

  WiFi.begin(ssid, password);
  //pinMode(2,OUTPUT);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to WiFi");
}

void loop() {

  StaticJsonDocument<1024> jsonDocument;
  DeserializationError error;


  String auth = "Basic " + base64::encode(authusername + ":" + authpassword);
  Serial.println(auth);

  client.setInsecure();

  Serial.println("Starting connection to server...");
  if (!client.connect(server, 443)) {
    Serial.println("connection failed");

  } else {
    Serial.println("Connected to Server");
    //client.println("GET/api/rfid/u9nm3eog8h4h/bhakti/write-to-rfid HTTP/ 1.1");
    client.println("GET /api/rfid/u9nm3eog8h4h/" + authusername + "/write-to-rfid/" + " HTTP/1.1");
    //client.println("GET /api/rfid/" + uqid +  "HTTP/1.1");

    //client.println("GET /fleetkaptan.up.railway.app/"  + authusername + "/HTTP/1.1");
    client.println("Host: fleetkaptan.up.railway.app");
    client.println("User-Agent: ESP32");
    client.println("Authorization: " + auth);
    client.println("Connection: close");
    client.println();
    String response;

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      //Serial.println(line);
      if (line == "\r") {
        Serial.println("headers received");
        break;
      }
      
    }

    // while(!client.available())
    // {
    //   Serial.println(".");
    // }
    if (client.available()) {
        response = client.readString();
      }
    response.trim();
    client.stop();
    Serial.println(response);
    error = deserializeJson(jsonDocument, response);

    if (error) {
      Serial.print("Deserialization error: ");
      Serial.println(error.c_str());
      return;
    }
    // bool sent_from_server = jsonDocument["sent_from_server"];
    // Serial.println(sent_from_server);
    Serial.println(response);
    
    int id = jsonDocument["id"];
    const char* unique_id_recived_from_server = jsonDocument["esp"]["unique_id"];
    Serial.println("its here");
    Serial.println(id);
    Serial.println(unique_id_recived_from_server);
    bool sent_from_server = jsonDocument["sent_from_server"];
    Serial.println(sent_from_server);
    // if(unique_id_recived_from_server == uqid)
    // {
    //   Serial.println("its LED time");
    //   digitalWrite(2,HIGH);
    //   delay(1000);
    //   digitalWrite(2,LOW);
    //   delay(2000);
    // }
    delay(7000);
  }
}