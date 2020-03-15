#include <Arduino.h>
#include <DallasTemperature.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <OneWire.h>
#include <stdio.h>
#include <time.h>

// Use pin D4 or GPIO2 for ESP01
#ifdef NODEMCUV2
#define ONE_WIRE_BUS D4
#elif defined ESP01
#define ONE_WIRE_BUS 2
#else
#define ONE_WIRE_BUS D4
#endif

static const unsigned long TIMEOUT = 15000; // ms
static const bool RETAINED = true;
static const bool QOS = 2;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float Celcius = 0;
float Fahrenheit = 0;

WiFiClient net;
MQTTClient client;

void setup() {
  pinMode(ONE_WIRE_BUS, INPUT_PULLUP);
  Serial.begin(9600);
  sensors.begin();

  auto startTime = millis();

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("\nConnecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - startTime >= TIMEOUT) {
      Serial.println("\nFailed to connect. Sleeping.");
      ESP.deepSleep(SLEEP_TIME);
      return;
    }

    Serial.print(".");
    delay(1000);
  }

  configTime(0, 0, TIME_SERVER, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) {
    if (millis() - startTime >= TIMEOUT) {
      Serial.println("\nFailed to get time. Sleeping.");
      ESP.deepSleep(SLEEP_TIME);
      return;
    }
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nConnecting to broker " + String(BROKER));
  client.begin(BROKER, net);

  while (!client.connect("arduino")) {
    if (millis() - startTime >= TIMEOUT) {
      Serial.println("\nFailed to connect to broker. Sleeping.");
      ESP.deepSleep(SLEEP_TIME);
      return;
    }
    Serial.print(".");
    delay(1000);
  }

  Serial.println("");
}

void loop() {
  sensors.requestTemperatures();
  Celcius = sensors.getTempCByIndex(0);
  Serial.print(" C  ");
  Serial.print(Celcius);

  time_t now = time(nullptr);
  Serial.println(ctime(&now));

  String metricValue =
      "{ \"temp\": " + String(Celcius) + ", \"time\": " + String(now) + "}";
  Serial.println("Posting" + metricValue);

  client.publish(TOPIC, metricValue, RETAINED, QOS);

  Serial.println("Published.");
  Serial.println("Going to sleep");
  ESP.deepSleep(SLEEP_TIME);
}
