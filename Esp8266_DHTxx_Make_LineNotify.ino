#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include "DHT.h"
#include "certs.h"

#ifndef STASSID
#define STASSID "your_wifi_ssid"
#define STAPSK "your_wifi_password"
#endif

#define DHTPIN D3     
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);

#define THRESHOLD_TEMP  30.5  // degree Celsius
#define THRESHOLD_HUMI  80.0  // percent
  
const char* uri = "/7qy8o4kd5behjsj98vnbbs8egtb79sld";

ESP8266WiFiMulti WiFiMulti;
BearSSL::WiFiClientSecure client;
X509List cert(cert_Root_CA);
HTTPClient https;

void setup() {

  Serial.begin(115200);
  dht.begin();
  
  // Attach to a WiFi AP
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(STASSID, STAPSK);
  Serial.print("Wait for WiFi... ");
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("WiFi connected: ");
  Serial.print(WiFi.SSID());
  Serial.print(" ");
  Serial.println(WiFi.localIP());

  // Set time via NTP, as required for x.509 validation
  configTime(8 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(NULL);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(NULL);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));

  // Set WiFiClientSecuteBareSSL 
  Serial.print("Connecting to ");
  Serial.println(remote_host);
  Serial.printf("Using certificate: %s\n", cert_Root_CA);
  client.setTrustAnchors(&cert);
}

void loop() {
  // read humidity and temperature (Celsius) from DHT sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    goto EXIT;
  }
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));

  // check if temperature or humidity are over the thresholds
  if (t<THRESHOLD_TEMP && h<THRESHOLD_HUMI) goto EXIT;
   
  // upload data to the webhook trigger at make.com
  char buf[100];
  sprintf(buf,"%s?temp=%.1f&humi=%.1f", uri,t,h); 
  Serial.print("[HTTPS] begin...\n");
  if (https.begin(client, remote_host, remote_port, buf, true)) {  // HTTPS
    Serial.print("[HTTPS] GET...\n");
    // start connection and send HTTP header
    int httpCode = https.GET();
    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = https.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
    }

    https.end();
  } else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }
EXIT:    
  Serial.println("Wait 10s before next round...");
  delay(10000);
}
