#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#ifndef STASSID
#define STASSID "5G" // example wifi SSID
#define STAPSK  "ololo111222333" // example password
#endif


const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

const String int2s[16] = {"c0","c1","c2","c3","c4","c5","c6","c7","c8","c9","c10","c11","c12","c13","c14","c15"};
uint16_t chanels[16];


void addCORS(){
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS"); 
  server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  }

void sendError(uint8_t code, String err){
  Serial.println();
  Serial.print(code, DEC);
  Serial.print(":");
  Serial.print(err);
  Serial.println();
  
  DynamicJsonDocument doc(1024);
  doc["error"] = err;
  String output = "";
  serializeJson(doc, output);
  addCORS();
  server.send(400, "application/json", output);
  }

void getChanelValue() {
  DynamicJsonDocument doc(1024);
  for (uint8_t pwmnum=0; pwmnum < 16; pwmnum++) doc[int2s[pwmnum]] = chanels[pwmnum];
  String output = "";
  serializeJson(doc, output);
  addCORS();
  server.send(200, "application/json", output);
}

void setChanelValue() {
  if (server.hasArg("plain")== false){
    sendError(400, "Json body not received");
    return;
  }
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, server.arg("plain"));
  if (error) {
    sendError(500, "deserializeJson() failed");
    return;
  }
  for (uint8_t pwmnum=0; pwmnum < 16; pwmnum++) 
    {
      int value = doc[int2s[pwmnum]];
      if (value>=4095) value=4095;
      if (value<0) value=0;
      chanels[pwmnum] = value;
      pwm.setPWM(pwmnum, 0,  value);
    }
    getChanelValue();
}


void handleRoot() {
  server.send(200, "text/plain", "<html><body><h1>LEDs</h1></body></html>");
}


void handleNotFound() {
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}


void handleOptions() {
  server.sendHeader("Access-Control-Max-Age", "10000");
  addCORS();
  server.send(200, "text/plain", "" );
}


void setupUART(){
  Serial.begin(115200);
  delay(500);
  Serial.println("-=Leader=-");
  }


void setupWIFI(){
  Serial.println("WiFi connecting to : ");
  Serial.print(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  }


void setupServer(){
  server.on("/", HTTP_GET, handleRoot);
  server.on("/api", HTTP_OPTIONS, handleOptions);
  server.on("/api", HTTP_GET, getChanelValue);
  server.on("/api", HTTP_POST, setChanelValue);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
  }


void setupPWM(){
  Wire.setClock(400000L);
  pwm.begin();
  pwm.setPWMFreq(1600);
  Serial.println("PWM driver started");
  }

void setup(void) {
  setupUART();
  setupWIFI();
  setupServer();
  setupPWM();
}

void loop(void) {  
  server.handleClient();
}
