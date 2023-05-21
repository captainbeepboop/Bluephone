#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/cos2048_int8.h>
#include <Smooth.h>
#include <AutoMap.h>

#define MIN_INTENSITY 700
#define MAX_INTENSITY 10
#define MIN_MOD_SPEED 10000
#define MAX_MOD_SPEED 1

#define pot1 A0
#define pot2 A1

AutoMap kMapIntensity(0,1023,MIN_INTENSITY,MAX_INTENSITY);
AutoMap kMapModSpeed(0,1023,MIN_MOD_SPEED,MAX_MOD_SPEED);

Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCarrier(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aModulator(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, CONTROL_RATE> kIntensityMod(COS2048_DATA);

int mod_ratio = 5; 
long fm_intensity; 
float smoothness = 0.95f;
Smooth <long> aSmoothIntensity(smoothness);


void setup(){
  startMozzi(); // :))
}


void updateControl(){

    //note needs to be here
  int mod_freq = carrier_freq * mod_ratio; //hello

  // set the FM oscillator frequencies
  aCarrier.setFreq(carrier_freq); //
  aModulator.setFreq(mod_freq);

  int pot1_value = mozziAnalogRead(pot1); 
  int pot1_calibrated = kMapIntensity(pot1_value);
  fm_intensity = ((long)pot1_calibrated * (kIntensityMod.next()+128))>>8; // shift back to range after 8 bit multiply

  int pot2_value = mozziAnalogRead(pot2); // value is 0-1023
  float mod_speed = (float)kMapModSpeed(pot2_value)/1000;
  kIntensityMod.setFreq(mod_speed);
}


int updateAudio(){
  long modulation = aSmoothIntensity.next(fm_intensity) * aModulator.next();
  return aCarrier.phMod(modulation);
}


void loop(){
  audioHook();
}