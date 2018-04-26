
#include <VirtualWire.h>
#include <LiquidCrystal.h>
#include <SparkFunDS1307RTC.h>
#include <Wire.h>
#include "Button.h"

#define PREV_BUTTON_PIN 12     // the number of the pushbutton pin
#define NEXT_BUTTON_PIN 6
#define DISPLAY_BUTTON_HOLD_TIME 100
#define LED_PIN 9      // the number of the LED pin
#define REFRESH_INTERVAL 1000

LiquidCrystal lcd(8, 7, 5, 4, 3, 2);
 
unsigned long lastUpdate;
boolean refresh;
float temp;
float pressure;
int toDraw = 0;
Button prevButton = Button();
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
    prevButton.init(PREV_BUTTON_PIN);
    nextButton.init(NEXT_BUTTON_PIN);
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
  lcd.print(String(rtc.hour()) + ":");    
  if (rtc.minute() < 10)
    lcd.print('0');                       
  lcd.print(String(rtc.minute()));        
 
  lcd.setCursor(0,1);

  lcd.print(rtc.dayStr());               
  lcd.setCursor(8,0);
  lcd.print(String(rtc.date()) + "/");
  if (rtc.month() < 10)
    lcd.print('0');                      
  lcd.print(String(rtc.month()) + "/");  
  lcd.println(String(rtc.year()));       
}


void loop()
{
    static int8_t lastMinute = -1;
    nextButton.sample();
    prevButton.sample();

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

    if(nextButton.wasPressed()){
      toDraw++;
      if(toDraw >2){
        toDraw = 0;
      }
      refresh = true;
    }

    if(prevButton.wasPressed()){
      toDraw--;
      if(toDraw <0){
        toDraw = 2;
      }
      refresh = true;
    }
    
    if(lastUpdate + REFRESH_INTERVAL < millis() || refresh){
      switch(toDraw){
        case 0:
          drawTimw();
          break;
        case 1:
          drawPressure();
          break;
        case 2:
          drawTemp();
          break;
         default:
          drawTime();
          break;
        }
       refresh = false;
      lastUpdate = millis();
    }
    
}


