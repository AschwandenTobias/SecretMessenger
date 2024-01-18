/*
  ESP-NOW Demo - Receive
  esp-now-demo-rcv.ino
  Reads data from Initiator
  
  DroneBot Workshop 2022
  https://dronebotworkshop.com
*/

#include <esp_now.h>
#include <WiFi.h>
#include "rm67162.h"
#include <TFT_eSPI.h>

bool led = 0;
unsigned long lastDataRecvTime = 0;

// Define the necessary button pins
const int buttonPin1 = 0;
const int buttonPin2 = 21;
const int ledPin = 38;

// Define the default button state from button 1
bool buttonState1 = HIGH;
bool lastButtonState1 = HIGH;

// Define the default button state from button 2
bool buttonState2 = HIGH;
bool lastButtonState2 = HIGH;

// Define a data structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

// Create a structured object
struct_message myData;

// Callback function executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  // Turn on the LED when data is received
  led = true;
  digitalWrite(ledPin, HIGH);

  // Copy the received data
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("Character Value: ");
  Serial.println(myData.a);
  Serial.print("Integer Value: ");
  Serial.println(myData.b);
  Serial.print("Float Value: ");
  Serial.println(myData.c);
  Serial.print("Boolean Value: ");
  Serial.println(myData.d);
  Serial.println();

  lastDataRecvTime = millis();

  // Clear the screen and display the received message
  clear_all();
  int x = 258;
  drawString("Received Message:", x, 20);
  drawString(myData.a, x, 60);
  draw(myData.b, x, 100);
  draw(myData.c, x, 140);
  draw(myData.d, x, 180);
}

void clear_all() {
  sprite.fillSprite(TFT_BLACK);
}

void drawString(const String &text, int32_t x, int32_t y) {
  sprite.drawString(text, x, y, 4);
  lcd_PushColors(0, 0, 536, 240, (uint16_t *)sprite.getPointer());
}

void draw(const int number, int32_t x, int32_t y) {
  drawString(String(number), x, y);
}

void draw(const float number, int32_t x, int32_t y) {
  drawString(String(number), x, y);
}

void draw(const double number, int32_t x, int32_t y) {
  drawString(String(number), x, y);
}

void draw(const bool value, int32_t x, int32_t y) {
  drawString(String(value ? "True" : "False"), x, y);
}

void setup() {
  // amoled lcd initialization
  rm67162_init();
  lcd_setRotation(1);
  sprite.createSprite(536, 240);
  sprite.setSwapBytes(1);

  pinMode(ledPin, OUTPUT);
  // Set up Serial Monitor
  Serial.begin(115200);

  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  if (led && (millis() - lastDataRecvTime >= 3000)) {
    led = false;
    digitalWrite(ledPin, LOW);
  }

  // You may add other logic or actions here if needed
  // ...

  // Add a small delay to avoid unnecessary high CPU usage
  delay(10);
}