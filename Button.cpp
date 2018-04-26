#include "Button.h"
#include "Arduino.h"
#define LAST_BUTTON_STATE 1
#define BUTTON_STATE 3
#define WAS_PRESSED 2
#define RESET_FLAG 4


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

      if(reading == HIGH &&  !bitRead(state, RESET_FLAG)){
        bitWrite(state, RESET_FLAG, 0);
      } 
    }
  }
  
  bitWrite(state, LAST_BUTTON_STATE, reading);
}

boolean Button::wasPressed(){
  boolean buttonState = bitRead(state, BUTTON_STATE);
  boolean resetFlag = bitRead(state, RESET_FLAG);
  if(buttonState == 1){
    if(resetFlag == 0){
      bitWrite(state, RESET_FLAG, 1);
      return buttonState;
    }
    return 0;
  } else {
     bitWrite(state, RESET_FLAG, 0);
     return buttonState;    
  }
  
}



