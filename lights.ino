#include <FastLED.h>
#include <avr/power.h> 

#define DATA_PIN      20
#define COLOR_ORDER GRB
#define NUM_LEDS      32
#define DEBOUNCE     10 // button debouncer

int HUE=15;
int SATURATION=255;
int BRIGHTNESS=150;
int SPEEDO=10;
int STEPS=25;

CRGB leds[NUM_LEDS];
TBlendType    currentBlending;
CRGBPalette16 currentPalette;


byte buttons[] = {0,1,2,3,4,5,6,7,8,10,21,22,23,14,15,16}; 
// This handy macro lets us determine how big the array up above is, by checking the size
#define NUMBUTTONS sizeof(buttons)
// we will track if a button is just pressed, just released, or 'currently pressed' 
byte pressed[NUMBUTTONS], justpressed[NUMBUTTONS], justreleased[NUMBUTTONS];

void setup() {
  byte i;
 
  // Make input & enable pull-up resistors on switch pins
  for (i=0; i<NUMBUTTONS; i++){
    pinMode(buttons[i], INPUT_PULLUP);
  }
  
  // pin13 LED
  pinMode(13, OUTPUT);
  
  //FastLED.setMaxPowerInVoltsAndMilliamps( VOLTS, MAX_MA);
  FastLED.addLeds<WS2812B,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
}
  
void loop() {

Gradient();
digitalWrite(13, LOW);

check_switches();      // when we check the switches we'll get the current state

  for (byte i = 0; i<NUMBUTTONS; i++){
    if (pressed[i]) {
      digitalWrite(13, HIGH);
      // is the button pressed down at this moment
    }
    if (justreleased[i]) {
      if (i == 0){  
        HUE=190;
      }else if (i == 1){
        HUE=170;
      }else if (i == 2){
        HUE=140;
      }else if (i == 3){
        HUE=100;
      }else if (i == 4){
        HUE=70;
      }else if (i == 5){
        HUE=30;
      }else if (i == 6){
        HUE=0;
  }
  for (byte i=0; i<NUMBUTTONS; i++){  // remember, check_switches() will necessitate clearing the 'just pressed' flag
    justpressed[i] = 0;
  }
}
  }
}

void check_switches()
{
  static byte previousstate[NUMBUTTONS];
  static byte currentstate[NUMBUTTONS];
  static long lasttime;
  byte index;

  if (millis() < lasttime){ // we wrapped around, lets just try again
     lasttime = millis();
  }
  
  if ((lasttime + DEBOUNCE) > millis()) {
    // not enough time has passed to debounce
    return; 
  }
  // ok we have waited DEBOUNCE milliseconds, lets reset the timer
  lasttime = millis();
  
  for (index = 0; index<NUMBUTTONS; index++){ // when we start, we clear out the "just" indicators
    justreleased[index] = 0;
     
    currentstate[index] = digitalRead(buttons[index]);   // read the button
   
    if (currentstate[index] == previousstate[index]) {
      if ((pressed[index] == LOW) && (currentstate[index] == LOW)) {
          // just pressed
          justpressed[index] = 1;
      }
      else if ((pressed[index] == HIGH) && (currentstate[index] == HIGH)) {
          // just released
          justreleased[index] = 1;
      }
      pressed[index] = !currentstate[index];  // remember, digital HIGH means NOT pressed
    }
    //Serial.println(pressed[index], DEC);
    previousstate[index] = currentstate[index];   // keep a running tally of the buttons
  }
}

// GRADIENT --------------------------------------------------------------
void Gradient()
{
  SetupGradientPalette();

  static uint8_t startIndex = 0;
  startIndex = startIndex + 2;  // motion SPEEDO 

  FillLEDsFromPaletteColors( startIndex);
  FastLED.show();
  FastLED.delay(SPEEDO);
}

void SetupGradientPalette()
{
  CRGB light = CHSV( HUE + 5, SATURATION - 15, BRIGHTNESS);
  CRGB light1 = CHSV( HUE + 10, SATURATION - 10, BRIGHTNESS);
  CRGB light2 = CHSV( HUE + 15, SATURATION - 20, BRIGHTNESS);
  CRGB medium = CHSV ( HUE - 3, SATURATION, BRIGHTNESS);
  CRGB medium1 = CHSV ( HUE - 7, SATURATION, BRIGHTNESS);
  CRGB medium2 = CHSV ( HUE - 11, SATURATION, BRIGHTNESS);
  CRGB dark  = CHSV( HUE + 3, SATURATION - 30, BRIGHTNESS);
  CRGB dark1  = CHSV( HUE, SATURATION - 20, BRIGHTNESS);
  CRGB dark2  = CHSV( HUE -3, SATURATION - 15, BRIGHTNESS);

  
  
  currentPalette = CRGBPalette16( 
    light,  light1,  light2,  light1,
    medium, medium1, medium2,  medium1,
    dark,  dark1,  dark2,  dark1,
    medium, medium1, medium2,  medium1 );
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = BRIGHTNESS;
  
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += STEPS;
  }
}