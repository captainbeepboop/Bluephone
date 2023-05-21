
#include <MIDI.h>
#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/sin512_int8.h> // sine table for oscillator
#include <tables/whitenoise8192_int8.h>
#include <ADSR.h>

#define CONTROL_RATE 256

MIDI_CREATE_DEFAULT_INSTANCE();

Oscil <SIN512_NUM_CELLS, AUDIO_RATE> kick_oscil(SIN512_DATA);
Oscil <SIN512_NUM_CELLS, AUDIO_RATE> snare_oscil(SIN512_DATA);

Oscil<WHITENOISE8192_NUM_CELLS, AUDIO_RATE> snare_noise(WHITENOISE8192_DATA);
Oscil<WHITENOISE8192_NUM_CELLS, AUDIO_RATE> hihat_noise(WHITENOISE8192_DATA);

ADSR <CONTROL_RATE, CONTROL_RATE> kick_envelope;
ADSR <CONTROL_RATE, CONTROL_RATE> snare_envelope;
ADSR <CONTROL_RATE, CONTROL_RATE> hihat_envelope;

ADSR <CONTROL_RATE, CONTROL_RATE> kick_tone_envelope;
ADSR <CONTROL_RATE, CONTROL_RATE> snare_tone_envelope;

//drum values

byte gains[3] = {0, 0, 0};
int tones[2] = {100, 300};
int tone_mods[2] = {0, 0};
int attacks[3] = {15, 15, 15};
int decays[3] = {50, 150, 100};
int white_noise[2] = {20, 20};

int min_decay = 50;
int max_decay = 500;
int min_tone = 20;
int max_tone = 400;

int drum_index = 0;

void update_envelopes()

{

    tones[0] = map(mozziAnalogRead(A0), 0, 1024, min_tone, max_tone);
    tones[1] = map(mozziAnalogRead(A2), 0, 1024, min_tone, max_tone);
    decays[0] = map(mozziAnalogRead(A1), 0, 1024, min_decay, max_decay);
    decays[1] = map(mozziAnalogRead(A3), 0, 1024, min_decay, max_decay);
    decays[2] = map(mozziAnalogRead(A4), 0, 1024, min_decay, max_decay);
    

    kick_envelope.setTimes(attacks[0], decays[0], 0, 0);
    kick_tone_envelope.setTimes(attacks[0], decays[0], 0, 0);

    snare_envelope.setTimes(attacks[1], decays[1], 0, 0);
    snare_tone_envelope.setTimes(attacks[1], decays[1], 0, 0);

    hihat_envelope.setTimes(attacks[2], decays[2], 0, 0);
}


void setup() {
  
    startMozzi(CONTROL_RATE);
  
    kick_envelope.setADLevels(400, 0);
    snare_envelope.setADLevels(400, 0);
    hihat_envelope.setADLevels(400, 0);
    
    kick_tone_envelope.setADLevels(900, 0);
    snare_tone_envelope.setADLevels(900, 0);

    snare_noise.setFreq((float)AUDIO_RATE/WHITENOISE8192_SAMPLERATE);
    hihat_noise.setFreq((float)AUDIO_RATE/WHITENOISE8192_SAMPLERATE);

    update_envelopes();

    MIDI.setHandleNoteOn(HandleNoteOn);  // Put only the name of the function
    MIDI.begin(10);

}

void updateControl() {

    //update drums


    
    kick_envelope.update();
    kick_tone_envelope.update();
    gains[0] = kick_envelope.next();
    tone_mods[0] = tones[0] + kick_tone_envelope.next();

    snare_envelope.update();
    snare_tone_envelope.update();
    gains[1] = snare_envelope.next();
    tone_mods[1] = tones[1] + snare_tone_envelope.next();
    
    kick_oscil.setFreq(tone_mods[0]);
    snare_oscil.setFreq(tone_mods[1]);

    hihat_envelope.update();
    gains[2] = hihat_envelope.next();

    MIDI.read();

}

void play_drum(int drum) {

    update_envelopes();


    switch (drum) {
        case 0:
        kick_envelope.noteOn();
        kick_tone_envelope.noteOn();
        break;
    
        case 1:
        snare_envelope.noteOn();
        snare_tone_envelope.noteOn();
        break;
    
        case 2:
        hihat_envelope.noteOn();
        break;
    }
}

void HandleNoteOn(byte channel, byte note, byte velocity) {

        play_drum(note);
}


int updateAudio(){
    return MonoOutput::from8Bit(
        (gains[0] * kick_oscil.next() >> 8)
  
        +(gains[1] * snare_oscil.next() >> 8)
        +(gains[1] * white_noise[0]/100 * snare_noise.next() >> 8)

        +(gains[2] * white_noise[1]/100 * hihat_noise.next() >> 8)
    ); 
}


void loop(){
  audioHook(); // required here
}



