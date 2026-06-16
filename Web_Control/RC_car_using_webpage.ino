#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef APSSID
#define APSSID "ESP8266"
#define APPSK "1234"
#endif

#define triggerPin D7
#define echoPin D0
#define IN1 D1
#define IN2 D2
#define IN3 D5
#define IN4 D6

int obstacleCount = 0;
bool autoModeCount = false;
float distance; 
/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

void handleRoot() {
  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <title>RC Car Control</title>
  </head>
  <body style="text-align:center;">
    <h1>ESP8266 RC Car</h1>
  )rawliteral";

  html += "<h2>Distance: " + String(distance,1) + " cm</h2>";

  html += R"rawliteral(
    <p><a href="/forward"><button style="width:120px;height:50px;">FORWARD</button></a></p>

    <p>
      <a href="/left"><button style="width:120px;height:50px;">LEFT</button></a>
      <a href="/stop"><button style="width:120px;height:50px;">STOP</button></a>
      <a href="/right"><button style="width:120px;height:50px;">RIGHT</button></a>
    </p>

    <p><a href="/backward"><button style="width:120px;height:50px;">BACKWARD</button></a></p>

    <p>
      <a href="/auto"><button style="width:120px;height:50px;">AUTO</button></a>
      <a href="/manual"><button style="width:120px;height:50px;">MANUAL</button></a>
    </p>
  </body>
  </html>
  )rawliteral";
  server.send(200, "text/html", html);
}

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("FORWARD");
  server.sendHeader("Location", "/");
  server.send(303);
}
void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("BACKWARD");
  server.sendHeader("Location", "/");
  server.send(303);
}
void stopCarMotorOnly() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
void right() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(200);
  stopCarMotorOnly();
  Serial.println("RIGHT");
  server.sendHeader("Location", "/");
  server.send(303);
}
void left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(200);
  stopCarMotorOnly();
  Serial.println("LEFT");
  server.sendHeader("Location", "/");
  server.send(303);
}
void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  Serial.println("STOP");
  server.sendHeader("Location", "/");
  server.send(303);
}

void autoMode(){
  autoModeCount = true;
  server.sendHeader("Location", "/");
  server.send(303);
}
void Manual(){
  autoModeCount = false;
  stopCarMotorOnly();
  server.sendHeader("Location", "/");
  server.send(303);
}
void moveForward(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW); 
}
void moveRight(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void moveLeft(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void obstacleAvoidance(){
  digitalWrite(triggerPin,LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin,LOW);
  long duration = pulseIn(echoPin, HIGH, 30000);
  distance = (0.0342*duration)/2;
  if(distance > 0 && distance <= 15){
    obstacleCount++;
    stopCarMotorOnly();
    delay(200);
    moveRight();
    delay(200);
    stopCarMotorOnly();
    if(obstacleCount>=2){
      for(int i = 0; i<4 ; i++){
        moveLeft();
        delay(200);
      }
      stopCarMotorOnly();
      obstacleCount=0;
    }
  }
  else{
    obstacleCount = 0;
    moveForward();
  }
}
void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(triggerPin,OUTPUT);
  server.on("/", handleRoot);
  server.on("/forward", forward);
  server.on("/backward", backward);
  server.on("/left", left);
  server.on("/right", right);
  server.on("/stop", stopCar);
  server.on("/auto", autoMode);
  server.on("/manual", Manual);
}

void loop() {
  server.handleClient();
  if(autoModeCount){
    obstacleAvoidance();
  }
}