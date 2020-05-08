#include <MIDIElements.h>

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

void setup()
{
  enc = new MIDIEncoder(17,18,midiChannel,20,true, false);
  
  usbMIDI.setHandleNoteOff(OnNoteOff); //set event handler for note off
  usbMIDI.setHandleNoteOn(OnNoteOn); //set event handler for note on
  usbMIDI.setHandleControlChange(OnControlChange); // set event handler for CC
}

void loop()
{
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
