#include <WiFi.h>
#include <base64.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

const char* ssid = "redmi12";
const char* password = "9429571163";

String authusername = "bhakti";
String authpassword = "123";
String D0 = "0";
String data = "kavya";
String uid = "peify4s9u453";
const char* uqid = "peify4s9u453";


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

void loop()
{
  post_data(D0,data,uid);
}


void post_data(String D0, String data, String uid)
{
  //btSerial.end();

  StaticJsonDocument<1024> jsonDocument;
  DeserializationError error;

  String input_s = authusername + ":" + authpassword;
  String encoded = base64::encode(input_s);
  String auth = "Basic " + encoded;

  WiFiClientSecure client;
  client.setInsecure();
  
  if (client.connect("fleetkaptan.up.railway.app", 443) && WiFi.status() == WL_CONNECTED)
  {
 
    String this_will_be_sent_to_server_hehe = "D0=" + String(D0) + "&data=" + String(data) + "&uid=" + String(uid);

    client.println("POST /api/rfid/peify4s9u453/bhakti/read-esp-scanned HTTP/1.1");
    client.println("Host: fleetkaptan.up.railway.app");
    client.println("User-Agent: ESP32");
    client.println("Authorization: " + auth);
    client.println("Content-Type: application/x-www-form-urlencoded;");
    client.println("Content-Length: " + String(this_will_be_sent_to_server_hehe.length()));
    client.println();
    client.println(this_will_be_sent_to_server_hehe);

    while (client.connected())
    {
      String line = client.readStringUntil('\n');
      if (line == "\r")
      {
        Serial.println("headers received");
        break;
      }
    }

    String response;
    if (client.available())
    {
      response = client.readString();
    }
    response.trim();
    Serial.println(response);
    client.stop();

    error = deserializeJson(jsonDocument, response);

    if (error)
    {
      Serial.print("Deserialization error: ");
      Serial.println(error.c_str());
      return;
    }

    //digitalWrite(RED2, LOW);
    // Access the JSON data
    if (jsonDocument.containsKey("id") && !jsonDocument["id"].isNull())
    {

      int id = jsonDocument["id"];
      const char *uniqueId = jsonDocument["esp"]["unique_id"];
      const char *value = jsonDocument["value"];
      Serial.println(uniqueId);
      // for(int i = 0; i<10;i++)
      // {
      //   Serial.println(value);
      // }
    }
    else
    {
      Serial.println("ID not found or null");
    }

    client.stop();
  }
  else
  {
    Serial.println(F("Connection wasnt established"));
    //digitalWrite(RED2, HIGH);
  }
  //btSerial.begin(9600);
  delay(9000);
}
