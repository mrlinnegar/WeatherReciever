
#include <VirtualWire.h>
#include <LiquidCrystal.h>
#include <SparkFunDS1307RTC.h>
#include <Wire.h>
#include "Button.h"

#define DISPLAY_BUTTON_PIN 6     // the number of the pushbutton pin
#define NEXT_BUTTON_PIN 10
#define DISPLAY_BUTTON_HOLD_TIME 100
#define LED_PIN 9      // the number of the LED pin
#define SQW_INPUT_PIN 2   // Input pin to read SQW
#define SQW_OUTPUT_PIN 13 // LED to indicate SQW's state


LiquidCrystal lcd(8, 7, 5, 4, 3, 2);
 
unsigned long lastUpdate;
boolean refresh;
float temp;
float pressure;
int toDraw = 0;
Button displayButton = Button();
Button nextButton = Button();

void createChars(){
  for(int i = 0; i < 8; i++){
    uint8_t custom_hex1[8] = {
      0x1F,
      0x1F,
      0x1F,
      0x1F,
      0x1F,
      0x1F,
      0x1F,
      0x1F
    };
   for(int f = 0; f<8; f++){
      if(f < (7 - i)){
        custom_hex1[f] = 0x00;
      }  
   }
    lcd.createChar(i, custom_hex1);
  }
}


void setup()
{
    Serial.begin(9600);
    createChars();
    displayButton.init(DISPLAY_BUTTON_PIN, DISPLAY_BUTTON_HOLD_TIME);
    nextButton.init(NEXT_BUTTON_PIN, DISPLAY_BUTTON_HOLD_TIME);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);      
    lcd.begin(16, 2);
    lcd.clear();
    lcd.print(" Connecting...");
    lastUpdate = millis();
    
    rtc.begin();
        
    // Initialise the IO and ISR
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);	 // Bits per sec
    vw_rx_start();       // Start the receiver PLL running
}

void drawTemp(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp");
  lcd.setCursor(7,0);
  lcd.print(String(temp));
  lcd.print((char)223);
  lcd.print(F("c"));
  drawChart();
}

void drawPressure(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press");
  
  lcd.setCursor(7,0);
  lcd.print(String(pressure)+"mb");
  drawChart();
}


void drawChart(){
  lcd.setCursor(0,1);
  for(int i = 0; i < 8; i++){
    lcd.write(char(i));
  }
}

void drawTime()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(String(rtc.hour()) + ":"); // Print hour
  if (rtc.minute() < 10)
    lcd.print('0'); // Print leading '0' for minute
  lcd.print(String(rtc.minute())); // Print minute
 
  lcd.setCursor(0,1);

  // Few options for printing the day, pick one:
  lcd.print(rtc.dayStr()); // Print day string
  //Serial.print(rtc.dayC()); // Print day character
  //Serial.print(rtc.day()); // Print day integer (1-7, Sun-Sat)
  lcd.setCursor(8,0);
  lcd.print(String(rtc.date()) + "/");
  if (rtc.month() < 10)
    lcd.print('0'); // Print leading '0' for minute// (or) print date
  lcd.print(String(rtc.month()) + "/"); // Print month
  lcd.println(String(rtc.year()));        // Print year
}


void loop()
{
    static int8_t lastMinute = -1;
    displayButton.sample();

    rtc.update();
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      float temperature = word(buf[0], buf[1]);
      float pressureData = word(buf[2], buf[3]);

      temp = (temperature / 100.00) - 20.00;
      pressure = (pressureData / 100.00) + 926.00;
    }

    if(displayButton.isPressed()){
      toDraw++;
      if(toDraw >2){
        toDraw = 0;
      }
      refresh = true;
    }
    
    if(lastUpdate + 1000 < millis() || refresh){
      switch(toDraw){
        case 0:
          drawTemp();
          break;
        case 1:
          drawPressure();
          break;
        case 2:
          drawTime();
          break;
         default:
          break;
        }
       refresh = false;
      lastUpdate = millis();
    }
    
    /*
    if (rtc.minute() != lastMinute) // If the second has changed
    {
      drawTime(); // Print the new time
      
      lastMinute = rtc.minute(); // Update lastSecond value
    }
    */
    
}


