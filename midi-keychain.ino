/*
   MIDI-KEYCHAIN.ino V0.1b
   @author jambonbill
   @author aiadi
*/

#include "MIDIUSB.h"

byte midichannel = 0;


/*
void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}
*/


void setup() {
  Serial.begin(115200);
  
  for(byte i=2;i<=9;i++){
    pinMode(i, OUTPUT);
    digitalWrite(i,0);
  }
  
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);

}

void beep(byte PIN_NUMBER, byte state){
  digitalWrite(PIN_NUMBER, state);    // turn the LED off by making the voltage LOW
}


void loop() {
  
  midiEventPacket_t rx;
  do {
  /*
    for(byte i=2;i<=9;i++){
      digitalWrite(i,1);
    }
  */
    rx = MidiUSB.read();
    if (rx.header != 0) {
      
      byte msg = rx.byte1;
      midichannel = rx.byte1 & 0x0f;
      byte program = midichannel;
      
      switch (rx.header) {

        case 8://NOTE OFF
          beep(2+rx.byte2%8,0);
          //opa.noteOff(program, rx.byte2);
          //Serial.print("NOTE OFF: ");
          //Serial.print(rx.byte2, HEX);
          //Serial.print("CHN: ");
          //Serial.println(midichannel, HEX);
          break;

        case 9:
          beep(2+rx.byte2%8,1);
          //opa.noteOn(program, rx.byte2);
          //Serial.print("NOTE ON: ");
          //Serial.print(rx.byte2, HEX);
          //Serial.print(" - VELOCITY: ");
          //Serial.print(rx.byte3, HEX);
          //Serial.print(" - CHN: ");
          //Serial.println(midichannel, HEX);

          break;

        case 11://'Control change'
          //Serial.print("CC: ");
          //Serial.print("CC#");
          //Serial.print(rx.byte2, HEX);
          //Serial.print(" VAL:");
          //Serial.println(rx.byte3, HEX);
          //ccParam(rx.byte2, rx.byte3);
          break;

        case 12://'Program change' (104 user programs apparently)
          //Serial.print("PRG: ");
          //Serial.print(rx.byte2, HEX);
          //opa.loadInternal(midichannel, rx.byte2);
          break;


        case 0xe://'Pitch wheel'
          //Serial.print("PITCH");
          //opa.pitchBend(program, rx.byte2, rx.byte3);//[program,coarse[-128,128],fine[-128,128]]
          break;

        default:
          /*
            Serial.print("Received: ");
            Serial.print(rx.header, HEX);
            Serial.print("-");
            Serial.print(rx.byte1, HEX);
            Serial.print("-");
            Serial.print(rx.byte2, HEX);
            Serial.print("-");
            Serial.println(rx.byte3, HEX);
          */
          break;

      }
    }
  } while (rx.header != 0);
}
