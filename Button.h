#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"

class Button {
 public:
  Button(void);
  void init(byte setup_pin, unsigned int _hold_delay);
  void sample();
  boolean isPressed();
  boolean isHeld();
  boolean wasPressed();

 private:
   byte pin;
   byte state;  
   byte debounce_delay;    // the debounce time; increase if the output flickers
   unsigned int hold_delay;

   
   unsigned long last_debounce_time;  // the last time the output pin was toggled
   unsigned long last_pressed_time;
   boolean lastButtonState();
   
};
#endif
