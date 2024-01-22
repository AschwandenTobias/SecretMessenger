#include <esp_now.h>
#include <WiFi.h>
#include "rm67162.h"
#include <TFT_eSPI.h>

#define beige       0xe718 
#define greenLight       0xdfb8 //TODO
#define greenDark       0x268c
#define blue  0x845f
#define cyan 0x67ff


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

bool menuMode = false;
bool message_Handler = false;
bool chatMode = false;
bool keyboardMode = false;
bool startScreen = true;

const char* characters[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", ",", ".", "!", "?", " ", "CAPS", "ENTER", "EXIT"};
const char* charactersLowercase[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", ",", ".", "!", "?", " ", "caps", "enter", "exit"};

char message[64];// MAC Address of responder - edit as required
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Define a data structure
typedef struct struct_message {
  char a[32];
  bool isSend;
} struct_message;


char* predefinedMessages[] = {"Bring mir Cola aus dem Coop mit", "Bring mir ein Mate mit", "Habe Lust auf Salziges...", "Schoggi pls <3", "Brauche nix danke!"};
int j = 0;
String selectedMessage;

char* predefinedModes[] = {"Send predefined message", "Write your own message", "Send Picture", "Exit"};
int z = 0;
String selectedMode;

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

// Create a structured object
struct_message myData;
//const char* secretMessages[] = {"Secret Sauce!", "Very HOT!", "Spicey ;)", "Ohhh Yeah :p", "Se yellow from se egg", "Oh my gosh did you see that?"};
int i = 0;

struct_message chat[10];
int freeSlot = 0;

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

  struct_message newMessage;

  // Copy the message to the newMessage.a
  strncpy(newMessage.a, message.c_str(), 32);
  newMessage.a[32 - 1] = '\0'; // Ensure null-terminated

  // Set the isSend flag to true
  newMessage.isSend = true;
  // Store the newMessage in the chat array
  chat[freeSlot] = newMessage;

  // Increment the freeSlot index
  freeSlot++;
  // Format structured data
  strcpy(myData.a, messageBuffer);
  myData.isSend = true;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t*)&myData, sizeof(myData));

  // Display the result
  clear_all();
  drawString("Sent Message:", 20, 20);
  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
    drawString(myData.a, 20, 60);
  } else {
    Serial.println("Sending error");
    draw("Error trying to send...", 20, 60);
  }
  delay(100);
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
        delay(50);
        message_Handler = false;
      }
    }
    delay(100);
  }
}

void displayKeyboard(int32_t j, uint16_t color, bool upperCase) {
  sprite.setTextColor(TFT_WHITE);


  int arraySize = sizeof(characters) / sizeof(characters[0]);
  sprite.drawString("Your message: ", 20, 20, 4);
  int x = 20;  // Initial x-coordinate
  int y = 60;  // Initial y-coordinate

  for (int i = 0; i < arraySize; i++) {
    sprite.setTextColor((i == j) ? color : TFT_WHITE);
    if(upperCase) {
      sprite.drawString(String(charactersLowercase[i]), x, y, 4);
    } else { //CHANGE?
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
    buttonState2 = digitalRead(buttonPin2);
    buttonState1 = digitalRead(buttonPin1);

    if (buttonState2 == LOW) {
      j++;
      if (j > 43) {
        j = 0;
      }
      displayKeyboard(j, TFT_RED, upperCase);
      delay(100); // Add a small delay to control the speed of movement
    }

    if (buttonState1 != lastButtonState1) {
      if (buttonState1 == LOW) {
        if(j == 43) {
          keyboardMode = false;
          displayChat();
        } else if (j == 42) {
          keyboardMode = false;
          String messageString(message);
          sendSelectedMessage(messageString);
          displayChat();
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
      lastButtonState1 = buttonState1; // Update lastButtonState1
    }

    delay(10);
  }
}

void writeOwnMessage() {
  keyboardMode = true;
  sprite.fillSprite(TFT_BLACK);
  displayKeyboard(0, TFT_RED, false);
  delay(500);
  selectKeyboard(0 , TFT_RED, false);
}

void selectMenu() {
  //j = 0;
  while(menuMode) {
    buttonState2 = digitalRead(buttonPin2);
    buttonState1 = digitalRead(buttonPin1);

    if (buttonState2 != lastButtonState2) {
      if (buttonState2 == LOW) {
        j++;
        if (j >= 4) {
          j = 0;
        }
        displayMenu(j, TFT_RED, 4, predefinedModes);
      }
    }
    if (buttonState1 != lastButtonState1) {
      if (buttonState1 == LOW) {
        menuMode = false;
        if(j == 0) { //CHANGE
          messageHandler();
        } else if(j == 1) {
          writeOwnMessage();
        } else if(j == 2) {
          //Write your method to display and select the pictures and call it here
        } else if(j == 3) {
          displayChat();
        }
      }
    }
    delay(100);
  }
}

void displayMenu(int32_t selected, uint16_t color, int32_t menuSize, char** menuItems) {
  sprite.fillSprite(TFT_BLACK);

  /* TODO
  sprite.fillRoundRect(0, 0, 536, 240, 4, beige);
  sprite.setTextColor(TFT_BLACK);
  */
  for(int i = 0; i < menuSize; i++) {
    if(i == selected) {
      sprite.setTextColor(color);
    }
    /* TODO
    sprite.fillRoundRect(20, i * 40 + 20, 20, 240, 4, beige);
    */
    sprite.drawString(menuItems[i], 20, i * 40 + 20, 4);
    sprite.setTextColor(TFT_WHITE);
    lcd_PushColors(0, 0, 536, 240, (uint16_t*)sprite.getPointer());
  }
}

void displayChat() {
  chatMode = true;
  /*TODO
  sprite.fillRoundRect(0, 0, 536, 240, 4, beige);
  sprite.setTextColor(TFT_BLACK);
  */
  sprite.fillSprite(TFT_BLACK); //TODO
  sprite.setTextColor(TFT_WHITE);
  for (int i = 0; i < freeSlot; i++) {
    /*TODO
    size_t strLen = strlen(chat[i].a);
    int32_t y = strLen * 20 + 20;
    */
    int32_t x = (chat[i].isSend) ?  200 : 20; //TODO: change x pos
    //sprite.fillRoundRect(x, y, 536, 240, greenLight);

    sprite.drawString(chat[i].a, x, i * 40 + 20, 4);
  }
  lcd_PushColors(0, 0, 536, 240, (uint16_t*)sprite.getPointer());
}

void messageHandler() {
  message_Handler = true;
  displayMenu(0, TFT_RED, 5, predefinedMessages);
  delay(200); //CHANGE
  selectMessage();
  displayChat();
}

void mainMenu() {
  j = 0;
  menuMode = true;
  displayMenu(0, TFT_RED, 4, predefinedModes);
  delay(200);
  selectMenu();
}

void loop() {
  //sprite.fillSprite(TFT_BLACK);
  /* TODO
  //sprite.fillRoundRect(0, 0, 536, 240, 4, beige);
  int screenHeight = 240; // replace with your actual screen height
  int textHeight = 5; // replace with your actual text height
  int verticalPosition = (screenHeight - textHeight) / 2;
  //sprite.setTextColor(TFT_BLACK);
  sprite.setTextSize(textHeight);
  //sprite.fillRoundRect(10, verticalPosition -40, 516, textHeight * 24, 8, greenDark);
  //sprite.drawRoundRect(10, verticalPosition -40, 516, textHeight * 24, 8, TFT_BLACK);
  sprite.setTextColor(TFT_WHITE);
  sprite.drawString("Secret Messenger", 20, verticalPosition);
  lcd_PushColors(0, 0, 536, 240, (uint16_t*)sprite.getPointer());
  sprite.setTextSize(1);
  */

  //sprite.fillRectHGradient(0, 0, 536, 240, cyan, blue);
  if(startScreen) {
  sprite.fillSprite(TFT_BLACK);
  sprite.setTextSize(5);
  sprite.setTextColor(TFT_WHITE);
  sprite.drawString("Secret Messenger", 20, 105);
  lcd_PushColors(0, 0, 536, 240, (uint16_t*)sprite.getPointer());
  //sprite.setTextColor(TFT_WHITE);
  sprite.setTextSize(1);
  }
  
 
  // Read the current state of the buttons
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);

  if(buttonState2 != lastButtonState2) {
    if(buttonState2 == LOW && message_Handler == false) {
      startScreen = false;
      mainMenu();
    }
  }
  // Check if the button state has changed
  if (buttonState1 != lastButtonState1 && message_Handler == false) {
    if (buttonState1 == LOW) {
      startScreen = false;
      //TODO: do something, light LED?
    } 
    lastButtonState2 = buttonState2;
    lastButtonState1 = buttonState1;
  }
}