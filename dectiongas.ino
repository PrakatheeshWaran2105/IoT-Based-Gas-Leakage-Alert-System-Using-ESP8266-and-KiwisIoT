
/*
  Project: IoT-Based Gas Leakage Alert System
  Board: NodeMCU ESP8266
  Sensor: MQ-2 Gas Sensor
  Platform: KiwisIoT
*/

#include <ESP8266WiFi.h>
#include <KiwisIoT.h>

// -------- WiFi Credentials --------
const char* ssid = "wifi name ";
const char* pass = "wifi password";

// -------- KiwisIoT Topic ID --------
const char* topic = "dash_1791367866871";

KiwisIoT kiwisiot(ssid, pass, topic);

// -------- Pin Configuration --------
#define GAS_PIN A0
#define BUZZER_PIN D5
#define GREEN_LED D6
#define RED_LED D7

// -------- Gas Threshold --------
const int GAS_THRESHOLD = 400;

// -------- Variables --------
int gasValue = 0;
bool gasAlert = false;

unsigned long previousMillis = 0;
const unsigned long sendInterval = 2000;

void setup() {

  Serial.begin(115200);

  pinMode(GAS_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Initial condition
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);

  Serial.println();
  Serial.println("GAS LEAKAGE ALERT SYSTEM");
  Serial.println("------------------------");

  Serial.println("Connecting to KiwisIoT...");

  kiwisiot.begin();

  Serial.println("KiwisIoT Initialized");
  Serial.println("Gas Monitoring Started");
}

void loop() {

  // Maintain KiwisIoT connection
  kiwisiot.run();

  // Read MQ-2 sensor
  gasValue = analogRead(GAS_PIN);

  // Check gas threshold
  if (gasValue >= GAS_THRESHOLD) {

    gasAlert = true;

    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);

  } else {

    gasAlert = false;

    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
  }

  // Display sensor information
  Serial.print("Gas Sensor Value: ");
  Serial.println(gasValue);

  if (gasAlert) {
    Serial.println("WARNING: GAS LEVEL HIGH!");
  } else {
    Serial.println("Status: NORMAL");
  }

  // Send data every 2 seconds
  if (millis() - previousMillis >= sendInterval) {

    previousMillis = millis();

    // Channel 0: Gas sensor reading
    kiwisiot.send("0", String(gasValue));

    // Channel 1: Alert status (1 = Alert, 0 = Normal)
    kiwisiot.send("1", gasAlert ? "1" : "0");

    Serial.println("Data sent to KiwisIoT");

  }

  delay(100);
}
