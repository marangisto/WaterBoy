#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include "Content.h"

const char* ssid = "";
const char* password = "";

ESP8266WebServer server(80);

const int ESP_LED = 2;
const int STATUS_LED = 14;
const int OUTPUT3 = 12;
const int OUTPUT2 = 13;
const int OUTPUT1 = 4;
const int OUTPUT0 = 5;

void handleRoot()
{
    Serial.println("/");
    String buf = String(main_form_preamble)
               + main_form_button(0, digitalRead(OUTPUT0))
               + main_form_button(1, digitalRead(OUTPUT1))
               + main_form_button(2, digitalRead(OUTPUT2))
               + main_form_button(3, digitalRead(OUTPUT3))
               + String(main_form_postamble);

    digitalWrite(STATUS_LED, 1);
    server.send(200, "text/html", buf);
    digitalWrite(STATUS_LED, 0);
}

void handleNotFound()
{
    digitalWrite(STATUS_LED, 1);
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET)?"GET":"POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i=0; i<server.args(); i++)
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    server.send(404, "text/plain", message);
    Serial.println(message);
    digitalWrite(STATUS_LED, 0);
}

void setup(void){
    pinMode(ESP_LED, OUTPUT);
    pinMode(STATUS_LED, OUTPUT);
    pinMode(OUTPUT0, OUTPUT);
    pinMode(OUTPUT1, OUTPUT);
    pinMode(OUTPUT2, OUTPUT);
    pinMode(OUTPUT3, OUTPUT);
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
      digitalWrite(STATUS_LED, 1);
      delay(250);
      digitalWrite(STATUS_LED, 0);
      delay(250);
      Serial.print(".");
    }

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (MDNS.begin("watergate"))
        Serial.println("MDNS responder started");

    MDNS.addService("http", "tcp", 80);
    MDNS.addServiceTxt("http", "tcp", "url", "http://watergate.local");
    MDNS.addServiceTxt("http", "tcp", "webthing", "true");

    server.on("/", handleRoot);
  
    server.on("/0", [](){
        Serial.println("/0");
        server.send(200, "text/html", redirect_home);
        digitalWrite(OUTPUT0, !digitalRead(OUTPUT0));
    });
  
    server.on("/1", [](){
        Serial.println("/1");
        server.send(200, "text/html", redirect_home);
        digitalWrite(OUTPUT1, !digitalRead(OUTPUT1));
    });
  
    server.on("/2", [](){
        Serial.println("/2");
        server.send(200, "text/html", redirect_home);
        digitalWrite(OUTPUT2, !digitalRead(OUTPUT2));
    });
  
    server.on("/3", [](){
        Serial.println("/3");
        server.send(200, "text/html", redirect_home);
        digitalWrite(OUTPUT3, !digitalRead(OUTPUT3));
    });
  
    server.on("/clr", [](){
        Serial.println("/clr");
        server.send(200, "text/html", redirect_home);
        digitalWrite(OUTPUT0, 0);
        digitalWrite(OUTPUT1, 0);
        digitalWrite(OUTPUT2, 0);
        digitalWrite(OUTPUT3, 0);
    });
  
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");
}

void loop(void)
{
    static uint16_t i = 0;

    server.handleClient();

    if (++i == 0)
        digitalWrite(ESP_LED, 0);
    else if (i == 1024)
        digitalWrite(ESP_LED, 1);
}

