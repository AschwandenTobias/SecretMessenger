#include <esp_now.h>
#include <WiFi.h>
#include "rm67162.h"
#include <TFT_eSPI.h>

#include "config.h"
#include "buttons.h"


//const char* secretMessages[] = {"Secret Sauce!", "Very HOT!", "Spicey ;)", "Ohhh Yeah :p", "Se yellow from se egg", "Oh my gosh did you see that?"};
int i = 0;

// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Callback function executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&received_message, incomingData, sizeof(received_message));
  add_to_chat(received_message);
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

void add_to_chat(struct_message msg) {
  chat[freeSlot] = msg;
  freeSlot++;
  displayChat();
}

void sendSelectedMessage(const String& message) {
  // Convert the selected message to a char array
  char messageBuffer[32];
  message.toCharArray(messageBuffer, 32);

  // Prepare Message to be sent...
  strcpy(message_to_send.a, messageBuffer);
  message_to_send.isSend = false;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t*)&message_to_send, sizeof(message_to_send));
  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
  } else {
    Serial.println("Sending error");
  }

  struct_message newMessage;

  // Copy the message to the newMessage.a
  strncpy(newMessage.a, message.c_str(), 32);
  newMessage.a[32 - 1] = '\0'; // Ensure null-terminated
  newMessage.isSend = true;
  // Store the newMessage in the chat array
  add_to_chat(newMessage);
}

void selectMessage() {
  while (true) {
    if (pressed_top_button()) {
      j++;
      if (j >= 5) {
        j = 0;
      }
      displayPredefinedMessages(j, TFT_RED);
    }

    if (pressed_bottom_button() || hold_down_bottom_button()) {
      sendSelectedMessage(predefinedMessages[j]);
      break;
    }
    delay(100);
  }
}

void displayKeyboard(int32_t j, uint16_t color, bool upperCase) {
  sprite.setTextColor(TFT_WHITE);

  int arraySize = 44;
  sprite.drawString("Your message: ", 20, 20, 4);
  int x = 20;  // Initial x-coordinate
  int y = 60;  // Initial y-coordinate

  for (int i = 0; i < arraySize; i++) {
    sprite.setTextColor((i == j) ? color : TFT_WHITE);
    if(upperCase) {
      sprite.drawString(String(charactersLowercase[i]), x, y, 4);
    } else {
      sprite.drawString(String(characters[i]), x, y, 4);
    }

    // Adjust x-coordinate based on the position of the string
    if (i >= arraySize - 3) {
      x += 100; // Enough space for the last three strings
    } else if (i > 40) {
      x += 60;
    } else {
      x += 40;
    }

    if (x >= 536) {
      x = 20;  // Reset x-coordinate
      y += 40; // Move to the next line
    }
  }
  lcd_PushColors(0, 0, 536, 240, (uint16_t*)sprite.getPointer());
}

void selectKeyboard(int32_t j, uint16_t color, bool upperCase) {
  int messageIndex = 0;
  while (keyboardMode) {

    // Skip through the letters
    if (pressed_top_button() || hold_down_top_button()) {
      j++;
      if (j > 43) {
        j = 0;
      }
      displayKeyboard(j, TFT_RED, upperCase);
      delay(100); // Add a small delay to control the speed of movement
    }

    if (pressed_bottom_button()) {
      if(j == 43) {
        keyboardMode = false;
        displayChat();
        break;
      } else if (j == 42) {
        keyboardMode = false;
        String messageString(message);
        sendSelectedMessage(messageString);
        break;
      } else if (j == 41) {
        upperCase = !upperCase;
        sprite.fillRect(10,50,600,600,TFT_BLACK);
        displayKeyboard(41, TFT_RED, upperCase);
      } else {
        char selectedChar;
        if (upperCase) {
          selectedChar = charactersLowercase[j][0];
        } else {
          selectedChar = characters[j][0];
        }

        // Add the selected character to the message
        message[messageIndex] = selectedChar;
        messageIndex++;
        message[messageIndex] = '\0'; // Ensure null-terminated

        drawString("Your message: ", 20, 20);
        drawString(message, 200, 20);

        lcd_PushColors(0, 0, 536, 240, (uint16_t*)sprite.getPointer());
      }
    }
    delay(10);
  }
}

void writeOwnMessage() {
  keyboardMode = true;
  sprite.fillSprite(TFT_BLACK);
  displayKeyboard(0, TFT_RED, false);
  delay(200);
  selectKeyboard(0 , TFT_RED, false);
}

void selectMenu() {
  //j = 0;
  while(true) {
    if (pressed_top_button()) {
      j++;
      if (j >= 4) {
        j = 0;
      }
      displayMenu(j, TFT_RED, 4, predefinedModes);
    }
    if (pressed_bottom_button()) {
      if (j == 0) {
        messageHandler();
        break;
      }
      if (j == 1) {
        writeOwnMessage();
        break;
      }
      if (j == 2) {
        // Send Picture();
        break;
      }
      if (j == 3) {
        displayChat();
        break;
      }
    }
    delay(100);
  }
}

void displayMenu(int32_t selected, uint16_t color, int32_t menuSize, char** menuItems) {
  sprite.fillSprite(TFT_BLACK);
  for(int i = 0; i < menuSize; i++) {
    if(i == selected) {
      sprite.setTextColor(color);
    }
    sprite.drawString(menuItems[i], 20, i * 40 + 20, 4);
    sprite.setTextColor(TFT_WHITE);
    lcd_PushColors(0, 0, 536, 240, (uint16_t*)sprite.getPointer());
  }
}

void displayChat() {
  chatMode = true;
  sprite.fillSprite(TFT_BLACK);
  sprite.setTextColor(TFT_WHITE);
  for (int i = 0; i < freeSlot; i++) {
    int32_t x = (chat[i].isSend) ? 200 : 20;
    sprite.drawString(chat[i].a, x, i * 40 + 20, 4);
  }
  lcd_PushColors(0, 0, 536, 240, (uint16_t*)sprite.getPointer());
}

void messageHandler() {
  displayMenu(0, TFT_RED, 5, predefinedMessages);
  delay(200);
  selectMessage();
}

void mainMenu() {
  Serial.println("in main menu");
  j = 0;
  displayMenu(0, TFT_RED, 4, predefinedModes);
  delay(200);
  selectMenu();
}

void setup() {

  // amoled lcd initialization
  rm67162_init();
  lcd_setRotation(1);
  
  sprite.createSprite(536, 240);
  sprite.setSwapBytes(1);

  pinMode(bottom_button_pin, INPUT_PULLUP);
  pinMode(top_button_pin, INPUT_PULLUP);
  pinMode(led_pin, OUTPUT);
  
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
  esp_now_register_recv_cb(OnDataRecv);
  
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

void loop() {
  if (pressed_top_button()) {
    mainMenu();
  }

  if (pressed_bottom_button()) {
    displayChat();
  }
}