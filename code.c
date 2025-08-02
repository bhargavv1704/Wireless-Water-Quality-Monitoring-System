#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2  // DS18B20 data pin (GPIO2, D4 on NodeMCU)
const int pH_Pin = A0; // Analog pin connected to the pH sensor

// WiFi credentials
const char *ssid = "poco";
const char *password = "12345678";

// Set up a web server on port 80
ESP8266WebServer server(80);

// Set up OneWire and DallasTemperature
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Function to convert voltage to pH value
float voltageToPH(float voltage) {
  // Calibration constants
  const float voltageAtpH7 = 4.8; // Voltage at pH 7 (update this with your measured value)
  const float slope = 1.1233;    // Slope calculated from the calibration data
  return 7 + (voltage - voltageAtpH7) / slope;
}

void handleRoot() {
  int sensorValue = analogRead(pH_Pin);   // Read the analog value from the pH sensor
  float voltage = sensorValue * (5.0 / 1023.0);   // Convert the analog value to voltage
  float pHValue = voltageToPH(voltage);    // Convert the voltage to pH value
  
  // Temperature sensor code
  sensors.requestTemperatures();  // Send the command to get temperatures
  float temperature = sensors.getTempCByIndex(0);  // Get temperature in Celsius
  
  // Construct the response
  String response = "pH Value: " + String(pHValue, 2) + " | Temperature: " + String(temperature) + " °C";
  
  // Send the response
  server.send(200, "text/plain", response);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  sensors.begin();  // Initialize the DallasTemperature library
  
  server.on("/", handleRoot);  // Define the route for the root URL
  server.begin();  // Start the server
  
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();  // Handle incoming client requests
}