#pragma once

#include <esp_now.h>
#include <WiFi.h>
#include "rm67162.h"
#include <TFT_eSPI.h>

extern bool bottom_button_state;
extern bool prev_bottom_button_state;
extern bool top_button_state;
extern bool prev_top_button_state;

bool pressed_top_button();
bool pressed_bottom_button();
bool pressed_both_buttons();
bool hold_down_top_button();
bool hold_down_bottom_button();