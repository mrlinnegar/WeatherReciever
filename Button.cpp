#include "Button.h"
#include "Arduino.h"
#define LAST_BUTTON_STATE 1
#define WAS_PRESSED 2
#define BUTTON_STATE 3


Button::Button(void){}

void Button::init(byte setup_pin){
  pin = setup_pin; 
  debounceDelay = 50;     
  pinMode(setup_pin, INPUT);

}

void Button::sample(){
  int reading = digitalRead(pin);

  if (reading != bitRead(state, LAST_BUTTON_STATE)) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != bitRead(state, BUTTON_STATE) ) {
      bitWrite(state, BUTTON_STATE, reading);

      if(reading == HIGH &&  !bitRead(state, WAS_PRESSED)){
        bitWrite(state, WAS_PRESSED, 1);
      }
    }
  }
  
  bitWrite(state, LAST_BUTTON_STATE, reading);
}

boolean Button::wasPressed(){
  boolean buttonState = bitRead(state, BUTTON_STATE);
  boolean wasPressed = bitRead(state, WAS_PRESSED);
  if(wasPressed == 1 && buttonState == 1){
    bitWrite(state, WAS_PRESSED, 0);
  }
  return buttonState;  
}



