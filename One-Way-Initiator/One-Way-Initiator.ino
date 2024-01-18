/*
  ESP-NOW Demo - Transmit
  esp-now-demo-xmit.ino
  Sends data to Responder
  
  DroneBot Workshop 2022
  https://dronebotworkshop.com
*/

// Include Libraries
#include <esp_now.h>
#include <WiFi.h>
#include "rm67162.h"
#include <TFT_eSPI.h>

//Define the nececarry button pins
const int buttonPin1 = 0;
const int buttonPin2 = 21;
const int ledPin = 38;

//define the default button state from button 1
bool buttonState1 = HIGH;
bool lastButtonState1 = HIGH;

//define the default button state from button 2
bool buttonState2 = HIGH;
bool lastButtonState2 = HIGH;

// Variables for test data
int int_value;
float float_value;
bool bool_value = true;

bool mode_Handler = false;
bool message_Handler = false;

// MAC Address of responder - edit as required
uint8_t broadcastAddress[] = {0xEC, 0xDA, 0x3B, 0x9A, 0x41, 0x68};

// Define a data structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

const char* predefinedMessages[] = {"Bring mir Cola aus dem Coop mit", "Bring mir ein Mate mit", "Habe Lust auf Salziges...", "Schoggi pls <3", "Brauche nix danke!"};
int j = 0;
String selectedMessage;

const char* predefinedModes[] = {"Send predefined message", "Write your own message", "Send Picture", "Something"};
int z = 0;
String selectedMode;

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

// Create a structured object
struct_message myData;
const char* secretMessages[] = {"Secret Sauce!", "Very HOT!", "Spicey ;)", "Ohhh Yeah :p", "Se yellow from se egg", "Oh my gosh did you see that?"};
int i = 0;

// Peer info
esp_now_peer_info_t peerInfo;

// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void clear_all() {
  sprite.fillSprite(TFT_BLACK);
}
void drawString(const String& text, int32_t x, int32_t y) {
  sprite.drawString(text, x, y, 4); // Use the 'text' parameter instead of the hardcoded string
  lcd_PushColors(0, 0, 536, 240, (uint16_t*)sprite.getPointer());
}
void draw(const int number, int32_t x, int32_t y) {
  drawString(String(number), x, y); // Convert integer to String and call the draw function for String
}
void draw(const float number, int32_t x, int32_t y) {
  drawString(String(number), x, y); // Convert float to String and call the draw function for String
}
void draw(const double number, int32_t x, int32_t y) {
  drawString(String(number), x, y); // Convert double to String and call the draw function for String
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

  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
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

  // Register the send callback
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void displayPredefinedMessages(int32_t j, uint16_t color) {
  sprite.fillSprite(TFT_BLACK);
  for(int i = 0; i < 5; i++) {
    if(j == i) {
      sprite.setTextColor(color);
    }
    sprite.drawString(predefinedMessages[i], 20, i * 40 + 20, 4);
    sprite.setTextColor(TFT_WHITE);
    lcd_PushColors(0, 0, 536, 240, (uint16_t*)sprite.getPointer());
  }
}

void sendSelectedMessage(const String& message) {
  // Convert the selected message to a char array
  char messageBuffer[32];
  message.toCharArray(messageBuffer, 32);

  // Format structured data
  strcpy(myData.a, messageBuffer);
  int_value = random(1, 20);
  float_value = 1.3 * int_value;
  bool_value = !bool_value;
  myData.b = int_value;
  myData.c = float_value;
  myData.d = bool_value;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t*)&myData, sizeof(myData));

  // Display the result
  clear_all();
  drawString("Sent Message:", 20, 20);
  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
    drawString(myData.a, 20, 60);
    draw(myData.b, 20, 100);
    draw(myData.c, 20, 140);
    draw(myData.d, 20, 180);
  } else {
    Serial.println("Sending error");
    draw("Error trying to send...", 20, 60);
  }

  // Add a delay to prevent multiple sends on a single button press
  delay(1000);
}

void selectMessage() {
  while (message_Handler) {
    buttonState2 = digitalRead(buttonPin2);
    buttonState1 = digitalRead(buttonPin1);

    if (buttonState2 != lastButtonState2) {
      if (buttonState2 == LOW) {
        j++;
        if (j >= 5) {
          j = 0;
        }
        displayPredefinedMessages(j, TFT_RED);
      }
    }

    if (buttonState1 != lastButtonState1) {
      if (buttonState1 == LOW) {
        selectedMessage = predefinedMessages[j];
        sendSelectedMessage(selectedMessage);
        delay(4000);
        message_Handler = false;
      }
    }
    delay(200); // Add a small delay to debounce the buttons
  }
}

void messageHandler() {
  message_Handler = true;
  displayPredefinedMessages(0, TFT_RED);
  delay(1000);
  selectMessage();
}

void modeHandler() {
  mode_Handler = true;
  displayModeOptions(0, TFT_RED);
  delay(1000);
}

void displayModeOptions(int32_t j, uint16_t color) {
  sprite.fillSprite(TFT_BLACK);
  for(int i = 0; i < 5; i++) {
    if(j == i) {
      sprite.setTextColor(color);
    }
    sprite.drawString(predefinedModes[i], 20, i * 40 + 20, 4);
    sprite.setTextColor(TFT_WHITE);
    lcd_PushColors(0, 0, 536, 240, (uint16_t*)sprite.getPointer());
  }
}

void loop() {
  // Read the current state of the buttons
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);

  if(buttonState2 != lastButtonState2) {
    if(buttonState2 == LOW && message_Handler == false) {
      //modeHandler();
      messageHandler();
    }
  }
  // Check if the button state has changed
  if (buttonState1 != lastButtonState1 && message_Handler == false) {
    // If the button is pressed (LOW), send the message
    if (buttonState1 == LOW) {
      // Create test data
      int_value = random(1, 20);
      float_value = 1.3 * int_value;
      bool_value = !bool_value;

      // Format structured data
      strcpy(myData.a, secretMessages[i]);
      myData.b = int_value;
      myData.c = float_value;
      myData.d = bool_value;

      // Send message via ESP-NOW
      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t*)&myData, sizeof(myData));

      // Display the result
      clear_all();
      drawString("Sent Message:", 20, 20);
      if (result == ESP_OK) {
        Serial.println("Sending confirmed");
        drawString(myData.a, 20, 60);
        draw(myData.b, 20, 100);
        draw(myData.c, 20, 140);
        draw(myData.d, 20, 180);
      } else {
        Serial.println("Sending error");
        draw("Error trying to send...", 20, 60);
      }
      i++;
      if(i > 5) {
        i = 0;
      }
      // Add a delay to prevent multiple sends on a single button press
      delay(1000);
    }
    lastButtonState2 = buttonState2;
    // Update the last button state
    lastButtonState1 = buttonState1;
  }
}