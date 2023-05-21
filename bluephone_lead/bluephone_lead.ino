
#include <MIDI.h>
#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/sin2048_int8.h> // sine table for oscillator
#include <tables/saw_analogue512_int8.h>
#include <tables/triangle_analogue512_int8.h>
#include <tables/square_analogue512_int8.h>
#include <tables/cos512_int8.h>
#include <mozzi_midi.h>
#include <Smooth.h>
#include <AutoMap.h>
#include <ADSR.h>

#define CONTROL_RATE 256 // Hz, powers of 2 are most reliable

#define pot1 A0
#define pot2 A1
#define rotary1 2
#define rotary2 4
#define rotary_button 11

bool output = 0;

//dial tone things
int dtmf[2][13] = {
    {400, 697, 697, 697, 770, 770, 770, 852, 852, 852, 941, 941, 941},
    {450, 1209, 1336, 1477, 1209, 1336, 1477, 1209, 1336, 1477, 1209, 1336, 1477}
};

//midi
MIDI_CREATE_DEFAULT_INSTANCE();

//FM synth things
#define MIN_INTENSITY 10
#define MAX_INTENSITY 700
#define MIN_MOD_SPEED 2
#define MAX_MOD_SPEED 20000
#define MIN_VIB_SPEED 4
#define MAX_VIB_SPEED 200
#define MAX_OFFSET 50

AutoMap kMapIntensity(0,1023,MIN_INTENSITY,MAX_INTENSITY);
AutoMap kMapModSpeed(0,1023,MIN_MOD_SPEED,MAX_MOD_SPEED);
AutoMap kMapVibSpeed(0,1023,MIN_VIB_SPEED,MAX_VIB_SPEED);
AutoMap kMapVibIntensity(0,1023,0,255);
AutoMap kMapOffset(0,1023,0,MAX_OFFSET);

Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aCarrier(SIN2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aModulator(SIN2048_DATA);
Oscil<SIN2048_NUM_CELLS, CONTROL_RATE> kIntensityMod(SIN2048_DATA);
Oscil<SIN2048_NUM_CELLS, CONTROL_RATE> kVibMod(SIN2048_DATA);

int mod_ratio = 5; 
long fm_intensity; 
float smoothness = 0.95f;
Smooth <long> aSmoothIntensity(smoothness);

// regular oscillator
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aOsc1(SIN2048_DATA);
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aOsc2(SIN2048_DATA);

int offset = 0;

byte vibrato_intensity = 255;

//control things
bool lead = 0;
int mode = 0;
int wave = 0;

long debounce_time = 0;
int debounce_delay = 10;
bool button_status = 1;

void HandleNoteOn(byte channel, byte note, byte velocity) {
  
 if (channel == 5)
 {
  output = 1;
    
  float note1 = mtof(note);
  int mod_freq = note1 * 5;
  aCarrier.setFreq(note1);
  aModulator.setFreq(mod_freq);
   
  switch (wave) {      
    case 0:
        aOsc1.setTable(SAW_ANALOGUE512_DATA);
        aOsc2.setTable(SAW_ANALOGUE512_DATA);
    break;
    case 1:
        aOsc1.setTable(TRIANGLE_ANALOGUE512_DATA);
        aOsc2.setTable(TRIANGLE_ANALOGUE512_DATA);
    break;
    case 2:
        aOsc1.setTable(SQUARE_ANALOGUE512_DATA);
        aOsc2.setTable(SQUARE_ANALOGUE512_DATA);
    break;
    case 3:
        aOsc1.setTable(COS512_DATA);
        aOsc2.setTable(COS512_DATA);
    break;
  }

  switch(mode)
  {
    case 0:
        aOsc1.setFreq(note1);
    break;
    case 1:

    break;
    case 2:
        
        aOsc1.setFreq(note1);
        float note2 = note1 + offset;
        aOsc2.setFreq(note2);
    break;
  }
  lead = 1;
}    
  if (channel == 6)
  {
      output = 0;
      mode = 0;
      aOsc1.setTable(SIN2048_DATA);
      aOsc2.setTable(SIN2048_DATA);
      aOsc1.setFreq(dtmf[0][note]);
      aOsc2.setFreq(dtmf[1][note]);
      lead = 1;
  }
}

void HandleNoteOff(byte channel, byte note, byte velocity) {
  if(channel == 5 || channel == 6)
  {
    lead = 0;
  }
}

void setup() {

  MIDI.setHandleNoteOn(HandleNoteOn);
  MIDI.setHandleNoteOff(HandleNoteOff);
  MIDI.begin(MIDI_CHANNEL_OMNI);

  aOsc1.setFreq(440);
  aOsc2.setFreq(440);

  startMozzi(CONTROL_RATE);

  pinMode(2, INPUT_PULLUP);
  pinMode(3, OUTPUT);
  pinMode(4, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  digitalWrite(3, 0);

  attachInterrupt(digitalPinToInterrupt(2), rotary_turn, CHANGE);

  kIntensityMod.setFreq(2);
  kVibMod.setFreq(2);
  aCarrier.setFreq(200);
  aModulator.setFreq(1000);



}

void rotary_turn()
{
  if(digitalRead(2) != digitalRead(4))
  {
    wave++;
    if (wave > 3)
    {
      wave = 0;
    }
  }
  else
  {
    wave--;
    if (wave < 0)
    {
      wave = 3;
    }
  }

}


void button_press()
{
  mode++;
  if (mode > 2)
  {
    mode = 0;
  }
}

void updateControl(){
  MIDI.read();

  if (digitalRead(rotary_button) < button_status)
  {
    if (millis() - debounce_time > debounce_delay)
    {
      button_status = 0;
      debounce_time = millis();
      button_press();
    }
  }

  button_status = digitalRead(rotary_button);

  int pot1_value = mozziAnalogRead(pot1);
  int pot2_value = mozziAnalogRead(pot2);

  float vib_speed = (float)kMapVibSpeed(pot2_value)/1000;
  vibrato_intensity = (byte)kMapVibIntensity(pot1_value);
  kVibMod.setFreq(vib_speed);

  float mod_speed = (float)kMapModSpeed(pot2_value)/1000;
  kIntensityMod.setFreq(mod_speed);
  int pot1_calibrated = kMapIntensity(pot1_value);
  fm_intensity = ((long)pot1_calibrated * (kIntensityMod.next()+128))>>8; // shift back to range after 8 bit multiply

  offset = kMapOffset(pot1_value);

}


AudioOutput_t updateAudio(){
  if (mode == 1 && output == 1)
  {
      long modulation = aSmoothIntensity.next(fm_intensity) * aModulator.next();
      return aCarrier.phMod(modulation)*lead/2;
  }
  else if (mode == 0 && output == 1)
  {
      Q15n16 vibrato = (Q15n16) vibrato_intensity * kVibMod.next();
      return aOsc1.phMod(vibrato)*lead*2/3; // phase modulation to modulate frequency
  }
  else
  {
      return MonoOutput::from16Bit(100*lead*(aOsc1.next()+aOsc2.next()));
  }
}


void loop() {
  audioHook(); 
}
