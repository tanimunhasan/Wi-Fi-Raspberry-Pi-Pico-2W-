#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


#define RXD1 16  // Change to your specific RX pin
#define TXD1 17  // Change to your specific TX pin

// Replace these with your network credentials
const char* ssid = "your WiFi SSID";          // Replace with your WiFi SSID
const char* password = "WiFi Password";  // Replace with your WiFi Password

const String scriptUrl = "https://script.google.com/macros/s/AKfycbz9UrFdLhgf8y_yhzMlPfUSGyaBoY3e3TVb5L656EXU4ng4sE81_ylAN4Jbt_kjyhe51A/exec";

void setup() {
  Serial.begin(9600);
  Serial1.begin(115200,SERIAL_8N1, RXD1, TXD1);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
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

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial1.available() >= sizeof(float)) {
    float receivedFloat;

    Serial1.readBytes((char*)&receivedFloat, sizeof(receivedFloat));

    Serial.print("Received float: ");
    Serial.println(receivedFloat, 2);
    sendDataToGoogleSheet(receivedFloat);
  }
}
