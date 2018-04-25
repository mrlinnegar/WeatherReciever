#include "Button.h"
#include "Arduino.h"

Button::Button(void){}

void Button::init(byte setup_pin, unsigned int _hold_delay){
  pin = setup_pin; 
  debounce_delay = 50;     
  hold_delay = _hold_delay; 
  
  pinMode(setup_pin, INPUT);

}

void Button::sample(){
  boolean reading = digitalRead(pin);
  Serial.println(reading);
  bitWrite(state, 1, 0); //is_held = false;
  if (reading != lastButtonState()) {
    last_debounce_time = millis();
  }
  
  if ((millis() - last_debounce_time) > debounce_delay) { //check for a debounce
    bitWrite(state, 0, reading); // high or low
    if(isPressed() && last_pressed_time == NULL) {
      last_pressed_time = millis();
    } else if(!isPressed()) {
      last_pressed_time = NULL;
      bitWrite(state, 1, 0); //is_held = false;
    }

  }

  if(isPressed() && ((millis() - last_pressed_time) > hold_delay)) {
      last_pressed_time = millis();
      bitWrite(state, 1, true ); //is_held = true;
  }

  bitWrite(state, 2, reading);
}

boolean Button::wasPressed(){
  return bitRead(state, 3);  
}

boolean Button::isPressed(){
  return bitRead(state, 0);
}

boolean Button::isHeld() {
  return bitRead(state, 1);
}

boolean Button::lastButtonState(){
  return bitRead(state, 2);
}

