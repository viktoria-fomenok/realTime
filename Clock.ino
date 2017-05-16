//#include <DS3232RTC.h>
#include "RTClib.h"
#include <Time.h> 
#include <Wire.h> 
#include "FastLED.h"

RTC_DS3231 RTC;


#define NUM_LEDS 90 // Number of LED controlled
#define COLOR_ORDER BRG  // Define color order for your strip
#define DATA_PIN 6  // Data pin for led comunication
#define DST_PIN 2  // Define DST adjust button pin
#define MIN_PIN 4  // Define Minutes adjust button pin
#define HUR_PIN 5  // Define Hours adjust button pin
#define BRI_PIN 3  // Define Light sensor pin

CRGB leds[NUM_LEDS]; // Define LEDs strip
                    // 0,0,0,0
                    // 1,1,1,1
                    //  1 2 3 4 5 6 7 8 9 10111213141516171819202122232425262728

byte digits[12][21] = {
{
0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
, // Digit 0
{
0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1 }
, // Digit 1
{
1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0 }
, // Digit 2
{
1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1 }
, // Digit 3
{
1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1 }
, // Digit 4
{
1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1 }
, // Digit 5
{
1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
, // Digit 6
{
0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1 }
, // Digit 7
{
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
, // Digit 8
{
1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1 },
// Digit 9

{
1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0 }, 

// Digit oC
{
0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0 }
}
// Digit C
}; 

/*byte digits[12][28] = {{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},  // Digit 0
                       {0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},   // Digit 1
                       {1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},   // Digit 2
                       {1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},   // Digit 3
                       {1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1},   // Digit 4
                       {1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1},   // Digit 5
                       {1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},   // Digit 6
                       {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},   // Digit 7
                       {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},   // Digit 8
                       {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1},   // Digit 9 | 2D Array for numbers on 7 segment
                       {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},   // Digit *0
                       {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0}};  // Digit C*/
                       
bool Dot = true;  //Dot state
bool DST = false; //DST state
bool TempShow = false;
int last_digit = 0;

// int ledColor = 0x0000FF; // Color used (in hex)
long ledColor = CRGB::DarkOrchid; // Color used (in hex)
//long ledColor = CRGB::MediumVioletRed;
//Random colors i picked up
long ColorTable[16] = {
  CRGB::Amethyst,
  CRGB::Aqua,
  CRGB::Blue,
  CRGB::Chartreuse,
  CRGB::DarkGreen,
  CRGB::DarkMagenta,
  CRGB::DarkOrange,
  CRGB::DeepPink,
  CRGB::Fuchsia,
  CRGB::Gold,
  CRGB::GreenYellow,
  CRGB::LightCoral,
  CRGB::Tomato,
  CRGB::Salmon,
  CRGB::Red,
  CRGB::Orchid
};


void setup(){ 
  Serial.begin(9600); 
//  Wire.begin(); 
  LEDS.addLeds<WS2812B, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS); // Set LED strip type
  LEDS.setBrightness(75); // Set initial brightness
  pinMode(DST_PIN, INPUT_PULLUP); // Define DST adjust button pin
  pinMode(MIN_PIN, INPUT_PULLUP); // Define Minutes adjust button pin
  pinMode(HUR_PIN, INPUT_PULLUP); // Define Hours adjust button pin
  TempShow = false;   
} 

// Get time in a single number, if hours will be a single digit then time will be displayed 155 instead of 0155
int GetTime(){
  tmElements_t Now;
  RTC.(Now);
  //time_t Now = RTC.Now();// Getting the current Time and storing it into a DateTime object 
  int hour=Now.Hour;
  int minutes=Now.Minute;
  int second =Now.Second;
  if (second % 2==0) {Dot = false;}
    else {Dot = true;};
  return (hour*100+minutes);
  };

// Check Light sensor and set brightness accordingly
void BrightnessCheck(){
  const byte sensorPin = BRI_PIN; // light sensor pin
  const byte brightnessLow = 20; // Low brightness value
  const byte brightnessHigh = 70; // High brightness value
  int sensorValue = analogRead(sensorPin); // Read sensor
  Serial.print("Sensor is: ");Serial.println(sensorValue);
  sensorValue = map(sensorValue, 0, 255, 10, 100);
  LEDS.setBrightness(sensorValue);
  // if (sensorValue <= 10) {LEDS.setBrightness(brightnessHigh);}
  // else {LEDS.setBrightness(brightnessLow);}  
  };
  
// Convert time to array needet for display 
void TimeToArray(){
  int Now = GetTime();  // Get time
  
  int cursor = 90; // last led number
  
  // Serial.print("Time is: ");Serial.println(Now);
  if (DST){   // if DST is true then add one hour
   Now+=100;
  // Serial.print("DST is ON, time set to : ");Serial.println(Now);
  }; 
  /*if (Dot){leds[57]=ledColor;
           leds[56]=ledColor;}   
    else  {leds[57]=0x000000;
           leds[56]=0x000000;
    };*/
    
        if (Dot){
    leds[42]=ledColor;
    leds[43]=ledColor;
    leds[44]=ledColor;
    leds[45]=ledColor;
    leds[46]=ledColor;
    leds[47]=ledColor;
    }

    else {

    leds[42]=0x000000;
    leds[43]=0x000000;
    leds[44]=0x000000;
    leds[45]=0x000000;
    leds[46]=0x000000;
    leds[47]=0x000000;

    };
        

  for(int i=1;i<=4;i++){
    int digit = Now % 10; // get last digit in time
    if (i==1){
    //  Serial.print("Digit 4 is : ");Serial.print(digit);Serial.print(" ");

      //cursor = 86;
      cursor =69;
      for(int k=0; k<=20;k++){ 
        // Serial.print(digits[digit][k]);
        if (digits[digit][k]== 1){leds[cursor]=ledColor;}
         else if (digits[digit][k]==0){leds[cursor]=0x000000;};
         cursor ++;
        };
      // Serial.println();

      if (digit != last_digit)
      {
        cylon();
        ledColor =  ColorTable[random(16)];
      }
      last_digit = digit;
      
      }
    else if (i==2){
      // Serial.print("Digit 3 is : ");Serial.print(digit);Serial.print(" ");

      //cursor =58;
      cursor =48;
      
      for(int k=0; k<=20;k++){ 
        // Serial.print(digits[digit][k]);
        if (digits[digit][k]== 1){leds[cursor]=ledColor;}
         else if (digits[digit][k]==0){leds[cursor]=0x000000;};
         cursor ++;
        };
      // Serial.println();
      }
    else if (i==3){
      // Serial.print("Digit 2 is : ");Serial.print(digit);Serial.print(" ");
      //cursor =28;
      cursor =21;
      for(int k=0; k<=20;k++){ 
        // Serial.print(digits[digit][k]);
        if (digits[digit][k]== 1){leds[cursor]=ledColor;}
         else if (digits[digit][k]==0){leds[cursor]=0x000000;};
         cursor ++;
        };
      // Serial.println();
      }
    else if (i==4){
      // Serial.print("Digit 1 is : ");Serial.print(digit);Serial.print(" ");
      cursor =0;
      for(int k=0; k<=20;k++){ 
        // Serial.print(digits[digit][k]);
        if (digits[digit][k]== 1){leds[cursor]=ledColor;}
         else if (digits[digit][k]==0){leds[cursor]=0x000000;};
         cursor ++;
        };
      // Serial.println();
      }
    Now /= 10;
  }; 
};

// Convert temp to array needet for display 
void TempToArray(){
  tmElements_t tm;
  RTC.(tm);

  if (tm.Second != 20) {
    TempShow = false;
    return;
  }
  TempShow = true;
  int t = RTC.temperature();
  int celsius = (t / 4.0) * 100;

  Serial.print("Temp is: ");Serial.println(celsius);
  
  int cursor = 90; // last led number
    
    /*leds[57]=0x000000;
    leds[56]=0x000000;*/

    leds[42]=ledColor;
    leds[43]=ledColor;
    leds[44]=ledColor;
    leds[45]=ledColor;
    leds[46]=ledColor;
    leds[47]=ledColor;
      
  for(int i=1;i<=4;i++){
    int digit = celsius % 10; // get last digit in time
    if (i==1){
      Serial.print("Digit 4 is : ");Serial.print(digit);Serial.print(" ");

      //cursor = 86;
      cursor =69;
      for(int k=0; k<=20;k++){ 
        Serial.print(digits[11][k]);
        if (digits[11][k]== 1){leds[cursor]=ledColor;}
         else if (digits[11][k]==0){leds[cursor]=0x000000;};
         cursor ++;
        };
      Serial.println();
    }
    else if (i==2){
      Serial.print("Digit 3 is : ");Serial.print(digit);Serial.print(" ");

      //cursor =58;
      cursor =48;
      for(int k=0; k<=20;k++){ 
        Serial.print(digits[10][k]);
        if (digits[10][k]== 1){leds[cursor]=ledColor;}
         else if (digits[10][k]==0){leds[cursor]=0x000000;};
         cursor ++;
        };
      Serial.println();
      }
    else if (i==3){
      Serial.print("Digit 2 is : ");Serial.print(digit);Serial.print(" ");
      //cursor =28;
      cursor =21;
      for(int k=0; k<=20;k++){ 
        Serial.print(digits[digit][k]);
        if (digits[digit][k]== 1){leds[cursor]=ledColor;}
         else if (digits[digit][k]==0){leds[cursor]=0x000000;};
         cursor ++;
        };
      Serial.println();
      }
    else if (i==4){
      Serial.print("Digit 1 is : ");Serial.print(digit);Serial.print(" ");
      cursor =0;
      for(int k=0; k<=20;k++){ 
        Serial.print(digits[digit][k]);
        if (digits[digit][k]== 1){leds[cursor]=ledColor;}
         else if (digits[digit][k]==0){leds[cursor]=0x000000;};
         cursor ++;
        };
      Serial.println();
      }
    celsius /= 10;
  }; 
};


void DSTcheck(){
   int buttonDST = digitalRead(2);
   // Serial.print("DST is: ");Serial.println(DST);
   if (buttonDST == LOW){
    if (DST){
      DST=false;
      // Serial.print("Switching DST to: ");Serial.println(DST);
      }
      else if (!DST){
        DST=true;
        // Serial.print("Switching DST to: ");Serial.println(DST);
      };
   delay(500);   
   };
  }

void TimeAdjust(){
  int buttonH = digitalRead(HUR_PIN);
  int buttonM = digitalRead(MIN_PIN);
  if (buttonH == LOW || buttonM == LOW){
    delay(500);
    tmElements_t Now;
    RTC.(Now);
    int hour=Now.Hour;
    int minutes=Now.Minute;
    int second =Now.Second;
      if (buttonH == LOW){
        if (Now.Hour== 23){Now.Hour=0;}
          else {Now.Hour += 1;};
        }else {
          if (Now.Minute== 59){Now.Minute=0;}
          else {Now.Minute += 1;};
          };
    RTC.write(Now); 
    }
  }

/* coool effect function*/
void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }


void cylon () {
  static uint8_t hue = 0;
  Serial.print("x");
  // First slide the led in one direction
  for(int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show(); 
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
  }
  Serial.print("x");

  // Now go in the other direction.  
  for(int i = (NUM_LEDS)-1; i >= 0; i--) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
  }
}
  
void loop()  // Main loop
{ 
  BrightnessCheck(); // Check brightness
  DSTcheck(); // Check DST
  TimeAdjust(); // Check to se if time is geting modified
  TimeToArray(); // Get leds array with required configuration
  TempToArray();
  FastLED.show(); // Display leds array
  if (TempShow == true) delay (8000);
}

