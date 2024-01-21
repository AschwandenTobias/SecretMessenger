#include "buttons.h"
#include "config.h"


bool bottom_button_state = HIGH;
bool prev_bottom_button_state = HIGH;
bool top_button_state = HIGH;
bool prev_top_button_state = HIGH;

bool pressed_top_button() {
  top_button_state = digitalRead(top_button_pin);
  if (top_button_state == LOW /*!= prev_top_button_state*/) {
    prev_top_button_state = top_button_state;
    return 1;
  }
  return 0;
}

bool pressed_bottom_button() {
  bottom_button_state = digitalRead(bottom_button_pin);
  if (bottom_button_state == LOW /*!= prev_bottom_button_state*/) {
    prev_bottom_button_state = bottom_button_state;
    return 1;
  }
  return 0;
}

bool pressed_both_buttons() {
  return (pressed_top_button() && pressed_bottom_button());
}

bool hold_down_top_button() {
  top_button_state = digitalRead(top_button_pin);
  if (top_button_state == LOW) {
    return 1;
  }
  return 0;
}

bool hold_down_bottom_button() {
    bottom_button_state = digitalRead(bottom_button_pin);
  if (bottom_button_state == LOW) {
    return 1;
  }
  return 0;
}