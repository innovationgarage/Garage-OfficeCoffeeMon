#include <ESP8266WiFi.h>
#include "credentials.h"

//#define DEBUG

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println();
#endif
}

void connect()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

#ifdef DEBUG
  Serial.print("Connecting to wifi...");
#endif

  while (WiFi.status() != WL_CONNECTED) {

#ifdef DEBUG
    Serial.print(".");
#endif
    delay(250);
  }

#ifdef DEBUG
  Serial.println();
  Serial.println(WiFi.localIP());
#endif
}

void loop() {

  switch (random(7))
  {
    case 1: case 4: case 5: sendMessage("Everybody is drinking too much coffee :coffee: but not cleaning the machine..."); break;
    case 2: sendMessage("Too... much... coffee... :recycle: :coffee: :hankey:"); break;
    case 3:  sendMessage("I am super _cereal_ you guys, it is overflowing :finnadie:"); break;
    default:  sendMessage("Somebody please clean the disposal bin of the coffee machine... it is FULL :scream:"); break;
  }

  // deepSleep time is defined in microseconds. Multiply
  // seconds by 1e6
  ESP.deepSleep(random(10, 35) * 60 * 1e6); // 10 to 35 mins (near max)
}

void sendMessage(String msg) {
  connect();

  //Serial.println("Connecting to host...");
  WiFiClientSecure client;
  if (!client.connect(HOST, 443)) {
    //Serial.println("Connection failed");
    client.stop();
    return;
  }
  //Serial.println("Connected to host");

  String request = "";
  request += "POST ";
  request += URL;
  request += " HTTP/1.1\r\n";

  request += "Host: ";
  request += HOST;
  request += "\r\n";

  int len = msg.length() + 12;  // JSON wrapper length
  request += "Content-Length: ";
  request += len;
  request += "\r\n";
  request += "Accept: application/json\r\n";
  request += "Connection: close\r\n";
  request += "Content-Type: application/json\r\n";
  request += "\r\n";
  request += "{\"text\": \"";
  request += msg;
  request += "\"}";

  /*Serial.print(request);
    Serial.println();*/
  client.print(request);

  long timeout = millis() + 5000;
  while (!client.available()) {
    if (millis() > timeout) {
      //Serial.println("Request timed out");
      client.stop();
      return;
    }
  }

#ifdef DEBUG
  Serial.println("Response:");
  while (client.available()) {
    Serial.write(client.read());
  }
  Serial.println();
  Serial.println("Request complete");
#endif

  WiFi.disconnect();
}
