#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"

class Button {
 public:
  Button(void);
  void init(byte setup_pin);
  void sample();
  boolean isHeld();
  boolean wasPressed();

 private:
   byte pin;
   byte state;  
   byte debounceDelay;    // the debounce time; increase if the output flickers
   byte buttonState;
   boolean lastButtonState;
   unsigned long lastDebounceTime;  // the last time the output pin was toggled
   unsigned long last_pressed_time;
   
};
#endif
