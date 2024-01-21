#include "config.h"

const int bottom_button_pin = 0;
const int top_button_pin = 21;
const int led_pin = 38;

bool menuMode = false;
bool message_Handler = false;
bool chatMode = false;
bool keyboardMode = false;

const char* characters[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", ",", ".", "!", "?", " ", "CAPS", "ENTER", "EXIT"};
const char* charactersLowercase[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", ",", ".", "!", "?", " ", "caps", "enter", "exit"};

char* predefinedMessages[] = {"Bring mir Cola aus dem Coop mit", "Bring mir ein Mate mit", "Habe Lust auf Salziges...", "Schoggi pls <3", "Brauche nix danke!"};
int j = 0;
String selectedMessage;

char* predefinedModes[] = {"Send predefined message", "Write your own message", "Send Picture", "Exit"};
int z = 0;
String selectedMode;

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

char message[64];
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

struct_message message_to_send;
struct_message received_message;
struct_message chat[10];
int freeSlot = 0;

esp_now_peer_info_t peerInfo;
