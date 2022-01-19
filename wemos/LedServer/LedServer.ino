/*
    Need to work
    1. add File>>Preference>>additional links manager boards  http://arduino.esp8266.com/stable/package_esp8266com_index.json
    2. install board: Tools>>Board>>Manage boards "ESP8266 community"
    3. install libs:
        https://learn.adafruit.com/16-channel-pwm-servo-driver?view=all#install-adafruit-pca9685-library-1825143-2
        https://arduinojson.org/v6/doc/installation/  (https://www.youtube.com/watch?v=GUTpaY1YaXo  Select the version: 6.19.*)

    For programming select board "LOLIN(WEMOS) D1 R2 & mini" and currect port ("/dev/ttyUSB0" for example)
    Click "Upload", and board: hold "PRG" and click "RST" and you can release the button "PRG" to "programming mode" (https://youtu.be/P3ewe-sx0zw?t=180)
    
    Use Monitor port baud rate: 115200 for view starting log and debug
*/

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

ESP8266WebServer server(80);

const char* ssid = STASSID;
const char* password = STAPSK;


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
  char temp[1800];
  
  snprintf(temp, 1800,
    "<!DOCTYPE html><html lang=\"en\"><head> <meta charset=\"UTF-8\"> <title>Remolight Prototype</title> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3\" crossorigin=\"anonymous\"> <script src=\"https://releases.jquery.com/git/jquery-3.x-git.min.js\"></script> <script src=\"https://remolight.enweave.net/remolight.js\"></script></head><body><div class=\"container\"> <h1>Remolight prototype</h1> <div id=\"app\" class=\"mt-5 mb-5\"> <div class=\"row flex-row\"> <div class=\"col mb-3\"> <label class=\"form-label\" for=\"channel1\">Channel 1</label> <input class=\"form-control form-control-color\" type=\"color\" id=\"channel1\"> </div><div class=\"col mb-3\"> <label class=\"form-label\" for=\"channel2\">Channel 2</label> <input class=\"form-control form-control-color\" type=\"color\" id=\"channel2\"> </div><div class=\"col mb-3\"> <label class=\"form-label\" for=\"channel3\">Channel 3</label> <input class=\"form-control form-control-color\" type=\"color\" id=\"channel3\"> </div><div class=\"col mb-3\"> <label class=\"form-label\" for=\"channel4\">Channel 4</label> <input class=\"form-control form-control-color\" type=\"color\" id=\"channel4\"> </div></div><div class=\"row\"> <div class=\"col mb-3\"> <div class=\"form-check form-switch\"> <input class=\"form-check-input\" type=\"checkbox\" role=\"switch\" id=\"channel_uv\"> <label class=\"form-check-label\" for=\"channel_uv\">UV</label> </div></div></div></div></div></body></html>"
    );
  server.send(200, "text/html", temp);
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
