#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>
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

template<int CHAN>
struct output_t
{
    static const int max_count = 500;   // 50Hz -> 10s

    static bool read()
    {
        return digitalRead(CHAN);
    }

    static void write(bool x)
    {
        digitalWrite(CHAN, x);
        count = 0;
    }

    static void update()
    {
        if (count == max_count)
            digitalWrite(CHAN, 0);
        else if (count < max_count)
            ++count;
    }

    static volatile int count;
};

template<int CHAN>
volatile int output_t<CHAN>::count = 0;

typedef output_t<OUTPUT0> ch0;
typedef output_t<OUTPUT1> ch1;
typedef output_t<OUTPUT2> ch2;
typedef output_t<OUTPUT3> ch3;

void ICACHE_RAM_ATTR onTimerISR()
{
    static int i = 0;

    if ((i & 0x3f) == 0)
        digitalWrite(STATUS_LED, true);
    else if ((i & 0x3f) == 8)
        digitalWrite(STATUS_LED, false);

    ch0::update();
    ch1::update();
    ch2::update();
    ch3::update();

    ++i;
}

void handleRoot()
{
    Serial.println("/");
    String buf = String(main_form_preamble)
               + main_form_button(0, ch0::read())
               + main_form_button(1, ch1::read())
               + main_form_button(2, ch2::read())
               + main_form_button(3, ch3::read())
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

    // Timer setup
    timer1_attachInterrupt(onTimerISR);
    timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
    timer1_write(100000);   // 50Hz

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
        ch0::write(!ch0::read());
    });

    server.on("/1", [](){
        Serial.println("/1");
        server.send(200, "text/html", redirect_home);
        ch1::write(!ch1::read());
    });

    server.on("/2", [](){
        Serial.println("/2");
        server.send(200, "text/html", redirect_home);
        ch2::write(!ch2::read());
    });

    server.on("/3", [](){
        Serial.println("/3");
        server.send(200, "text/html", redirect_home);
        ch3::write(!ch3::read());
    });

    server.on("/clr", [](){
        Serial.println("/clr");
        server.send(200, "text/html", redirect_home);
        ch0::write(false);
        ch1::write(false);
        ch2::write(false);
        ch3::write(false);
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
