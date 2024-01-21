#pragma once

#include <esp_now.h>
#include <WiFi.h>
#include "rm67162.h"
#include <TFT_eSPI.h>

// Button and LED pins
extern const int bottom_button_pin;
extern const int top_button_pin;
extern const int led_pin;

// Modes  
extern bool menuMode;
extern bool message_Handler;
extern bool chatMode;
extern bool keyboardMode;

// Characters for the keyboard
extern const char* characters[];
extern const char* charactersLowercase[];

// Predefined messages
extern char* predefinedMessages[];
extern int j;
extern String selectedMessage;

// Menu modes
extern char* predefinedModes[];
extern int z;
extern String selectedMode;

// TFT display
extern TFT_eSPI tft;
extern TFT_eSprite sprite;

// Broadcast Address
extern char message[64];
extern uint8_t broadcastAddress[];

// Structured data for messages
typedef struct struct_message {
  char id;
  char a[32];
  bool isSend;
} struct_message;
extern struct_message message_to_send;
extern struct_message received_message;

extern struct_message chat[];
extern int freeSlot;

// Peer information for ESP-NOW
extern esp_now_peer_info_t peerInfo;