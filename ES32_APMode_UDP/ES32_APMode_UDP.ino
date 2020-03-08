#include<WiFi.h>
#include "AsyncUDP.h"
#include <WiFiAP.h>
#define SSID_NAME "ESP32APPalatip"
#define SSID_PASS "12345678"
IPAddress local_ip={192,168,1,4};
IPAddress gateway={192,168,1,1};
IPAddress subnet={255,255,255,0};
AsyncUDP udp;
WiFiServer server(80);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(SSID_NAME,SSID_PASS);
  Serial.print("IP : ");
  Serial.print(WiFi.softAPIP());
  server.begin();

      if(udp.listen(1234)) {
//        Serial.print("UDP Listening on IP: ");
//        Serial.println(WiFi.localIP());
        udp.onPacket([](AsyncUDPPacket packet) {
            Serial.print("UDP Packet Type: ");
            Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
            Serial.print(", From: ");
            Serial.print(packet.remoteIP());
            Serial.print(":");
            Serial.print(packet.remotePort());
            Serial.print(", To: ");
            Serial.print(packet.localIP());
            Serial.print(":");
            Serial.print(packet.localPort());
            Serial.print(", Length: ");
            Serial.print(packet.length());
            Serial.print(", Data: ");
            Serial.write(packet.data(), packet.length());
            Serial.println();
            //reply to the client
            packet.printf("Got %u bytes of data", packet.length());
        });
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  udp.broadcast("Anyone here?");
}
