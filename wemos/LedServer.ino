#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#ifndef STASSID
#define STASSID "5G"
#define STAPSK  "ololo111222333"
#endif



Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "application/json", "{}");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}

void setup(void) {
  Wire.setClock(400000L);
  
  pwm.begin();
  pwm.setPWMFreq(1600);
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  digitalWrite(LED_BUILTIN, LOW);
}

void loop(void) {
  
  // Drive each PWM in a 'wave'
  for (uint16_t i=4096; i>0; i -= 8) 
  {
    for (uint8_t pwmnum=0; pwmnum < 16; pwmnum++) 
    {
      pwm.setPWM(pwmnum, 0, (i + (4096/16)*pwmnum) % 4096 );
      server.handleClient();
    }
  }
}
