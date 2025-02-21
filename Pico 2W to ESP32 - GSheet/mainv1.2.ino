#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


#define RXD1 16  // Change to your specific RX pin
#define TXD1 17  // Change to your specific TX pin
#define LED_BUILTIN 2 // Onboard LED ESP32 (GPIO)

// Replace these with your network credentials
const char* ssid = "HUAWEI_E5783_DB93";          // Replace with your WiFi SSID
const char* password = "iqiqbL6AEGb";  // Replace with your WiFi Password

const String scriptUrl = "https://script.google.com/macros/s/AKfycbz9UrFdLhgf8y_yhzMlPfUSGyaBoY3e3TVb5L656EXU4ng4sE81_ylAN4Jbt_kjyhe51A/exec";

unsigned long resetInterval = 10 * 60 * 1000; // 10 minutes in milliseconds
unsigned long lastResetTime = 0;

void setup() {
  Serial.begin(9600);
  Serial1.begin(115200,SERIAL_8N1, RXD1, TXD1);
  WiFi.begin(ssid, password);
  
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  blinkLED(5,200);
}


void sendDataToGoogleSheet(float receivedFloat) {
  HTTPClient http;

  http.begin(scriptUrl);  // Your Apps Script URL

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String httpRequestData = "receivedFloat=" + String(receivedFloat, 4);  // Sends data with 4 decimal places

  int httpResponseCode = http.POST(httpRequestData);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Error in HTTP request: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

void blinkLED(int times, int delayTime) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(delayTime);
    digitalWrite(LED_BUILTIN, LOW);
    delay(delayTime);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
    // Reset ESP32 every 10 minutes
  if (millis() - lastResetTime >= resetInterval) {
    Serial.println("Restarting ESP32...");
    blinkLED(12, 200);  // Blink LED quickly 5 times (100ms on/off)
    ESP.restart();
  }

  if (Serial1.available() >= sizeof(float)) {
    float receivedFloat;

    Serial1.readBytes((char*)&receivedFloat, sizeof(receivedFloat));

    Serial.print("Received float: ");
    Serial.println(receivedFloat, 2);

    digitalWrite(LED_BUILTIN, HIGH); // Turn ON LED when buffer received
    delay(200); // Small indication delay
    digitalWrite(LED_BUILTIN, LOW); // Turn OFF LED

    sendDataToGoogleSheet(receivedFloat);
   
  }
}
