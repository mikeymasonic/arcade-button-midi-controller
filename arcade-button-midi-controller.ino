#include <FastLED.h>
// Because conditional #includes don't work w/Arduino sketches...
//#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET
#include <avr/power.h> // ENABLE THIS LINE FOR GEMMA OR TRINKET
#include <MIDIElements.h>

#define DATA_PIN      20
#define COLOR_ORDER GRB
#define NUM_LEDS      32
#define DEBOUNCE     10 // button debouncer, how many ms to debounce, 5+ ms is usually plenty

//MIDI PINS
boolean debug = false; // print to serial instead of midi
boolean secondary = true; // enable secondary midi messages
int midiChannel = 1; // midi channel number

int butPin1 = 0; 
int butPin2 = 1; 
int butPin3 = 2;
int butPin4 = 3;
int butPin5 = 4;
int butPin6 = 5;
int butPin7 = 6;
int butPin8 = 7;
int butPin9 = 25;
int butPin10 = 32;
int butPin11 = 21;
int butPin12 = 22;
int butPin13 = 23;
int butPin14 = 14;
int butPin15 = 15;
int butPin16 = 16;
int potPin1 = A10;
int potPin2 = A11;
int potPin3 = A18;
int clickPin = 19;
int ledPin = 33;

int HUE=15;
int SATURATION=255;
int BRIGHTNESS=150;
int SPEEDO=10;
int STEPS=25;

CRGB leds[NUM_LEDS];
TBlendType    currentBlending;
CRGBPalette16 currentPalette;

// here is where we define the buttons that we'll use. button "1" is the first, button "6" is the 6th, etc
byte buttons[] = {0,1,2,3,4,5,6,7,8,10,21,22,23,14,15,16}; 
// This handy macro lets us determine how big the array up above is, by checking the size
#define NUMBUTTONS sizeof(buttons)
// we will track if a button is just pressed, just released, or 'currently pressed' 
byte pressed[NUMBUTTONS], justpressed[NUMBUTTONS], justreleased[NUMBUTTONS];

// declare all your components here
MIDIEncoder *enc;
Button but(butPin1, midiChannel, 48, secondary, debug); 
Button but2(butPin2, midiChannel, 49, secondary, debug);
Button but3(butPin3, midiChannel, 50, secondary, debug);
Button but4(butPin4, midiChannel, 51, secondary, debug);
Button but5(butPin5, midiChannel, 47, secondary, debug);
Button but6(butPin6, midiChannel, 46, secondary, debug);
Button but7(butPin7, midiChannel, 45, secondary, debug);
Button but8(butPin8, midiChannel, 44, secondary, debug);
Button but9(butPin9, midiChannel, 43, secondary, debug);
Button but10(butPin10, midiChannel, 42, secondary, debug);
Button but11(butPin11, midiChannel, 41, secondary, debug);
Button but12(butPin12, midiChannel, 40, secondary, debug);
Button but13(butPin13, midiChannel, 39, secondary, debug);
Button but14(butPin14, midiChannel, 38, secondary, debug);
Button but15(butPin15, midiChannel, 37, secondary, debug);
Button but16(butPin16, midiChannel, 36, secondary, debug);
Potentiometer pot1(potPin1, midiChannel, 17, secondary, debug);
Potentiometer pot2(potPin2, midiChannel, 18, secondary, debug);
Potentiometer pot3(potPin3, midiChannel, 19, secondary, debug);
Button clk(clickPin, midiChannel, 21, secondary, debug);
Led led(ledPin, midiChannel, 3, true); // led on pin 15 with PWM enabled, triggered on the same CC number as pot


void setup() {
  byte i;
 
  // Make input & enable pull-up resistors on switch pins
  for (i=0; i<NUMBUTTONS; i++){
    pinMode(buttons[i], INPUT_PULLUP);
  }
  
  // pin13 LEDs
  pinMode(13, OUTPUT);
  
  //FastLED.setMaxPowerInVoltsAndMilliamps( VOLTS, MAX_MA);
  FastLED.addLeds<WS2812B,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  //MIDI Setup
  enc = new MIDIEncoder(17,18,midiChannel,20,true, false);
  usbMIDI.setHandleNoteOff(OnNoteOff); //set event handler for note off
  usbMIDI.setHandleNoteOn(OnNoteOn); //set event handler for note on
  usbMIDI.setHandleControlChange(OnControlChange); // set event handler for CC
}
  
void loop() {

  //MIDI Button Reads
  //delay(10);
  but.read(); 
  but2.read();
  but3.read();
  but4.read();
  but5.read();
  but6.read();
  but7.read();
  but8.read();
  but9.read();
  but10.read();
  but11.read();
  but12.read();
  but13.read();
  but14.read();
  but15.read();
  but16.read();
  pot1.read();
  pot2.read();
  pot3.read();
  enc->read();
  clk.read();

  usbMIDI.read(); // read all the incoming midi messages

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

//====================================================================
// event handlers
void OnNoteOn(byte channel, byte note, byte velocity)
{
  // add all your output component sets that will trigger with note ons
  led.setOn(channel, note, velocity);
}

void OnNoteOff(byte channel, byte note, byte velocity)
{
  // add all your output component sets that will trigger with note ons
  led.setOff(channel, note, velocity);
}

void OnControlChange(byte channel, byte control, byte value)
{
  // add all your output component sets that will trigger with cc
  led.setOn(channel, control, value);
}