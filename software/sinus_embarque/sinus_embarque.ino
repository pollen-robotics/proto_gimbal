//
// Slow and precise BLDC motor driver using SPWM and SVPWM modulation
// Part of code used from http://elabz.com/
// (c) 2015 Ignas Gramba www.berryjam.eu
// 

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

#define PHASE1 0
#define PHASE2 2.094395102393195492308428922186335
#define PHASE3 4.188790204786390984616857844372671


#include <CommandHandler.h>

//Create a CommandHandler Instance
CommandHandler cmdHdl;


const int EN = 8;
const int IN1 = 9;
const int IN2 = 10;
const int IN3 = 11;

bool direct = true;
double amplitude = 1;
double t = 0;  // position on the sinus
double period = 5;  // time for on oscillation

double last_time;
double current_time;
double detla_time;

//////////////////////////////////////////////////////////////////////////////

void setup() {

  Serial.begin(115200); // setting up the serial port to 9600 baud

  cmdHdl.addCommand("A", setAmplitude);  // send A,0.5; to change amplitude to 0.5, min 0, max 1
  cmdHdl.addCommand("F", setFreq);  // send F,1; to set frequency to 1Hz, should be above 0 
  cmdHdl.setDefaultHandler(unrecognized);
  
  //
  setPwmFrequency(IN1); // Increase PWM frequency to 32 kHz  (make unaudible)
  setPwmFrequency(IN2);
  setPwmFrequency(IN3);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  
  pinMode(EN, OUTPUT);
  digitalWrite(EN, HIGH);

  current_time = millis();

}

//////////////////////////////////////////////////////////////////////////////

void loop() {

  cmdHdl.processSerial(Serial);

  //
  last_time = current_time;
  current_time = millis();
  detla_time = (current_time - last_time) / 1000;

  if (direct == true) {
    t = t + detla_time;
  } else {
    t = t - detla_time;
  }
  
  if (t > period) {
    t = t - period;
  } else if (t < 0) {
    t = period - t;
  }

  apply_pwm(IN1, pwm_value(PHASE1));
  apply_pwm(IN2, pwm_value(PHASE2));
  apply_pwm(IN3, pwm_value(PHASE3));
}

void apply_pwm(int pin, double value) {
  analogWrite(pin, (int) ((value * 255) + 0.5));
}


double pwm_value(double phase) {
  return amplitude * (sin(TWO_PI * t / period + phase) / 2 + 0.5);
}

void setAmplitude(){
  double newAmplitude;
  newAmplitude = cmdHdl.readDoubleArg();
  if (cmdHdl.argOk) {
    if (newAmplitude >= 0){
      if (newAmplitude<= 1) {
        amplitude = newAmplitude;
        return;
      }
    }
  }
  sendError();
}

void setFreq(){
  double freq;
  freq = cmdHdl.readDoubleArg();
  if (cmdHdl.argOk) {
    if (freq > 0) {
      double newPeriod = 1 / freq;
      t = t * newPeriod / period;
      period = newPeriod;
      return;
    }
  }
  sendError();
}

void sendError(){
  cmdHdl.initCmd();
  cmdHdl.addCmdString("ERROR");
  cmdHdl.sendCmdSerial();
}

// This gets set as the default handler, and gets called when no other command matches.
void unrecognized(const char *command) {
  cmdHdl.initCmd();
  cmdHdl.addCmdString("UNKNOWN");
  cmdHdl.sendCmdSerial();
}

void setPwmFrequency(int pin) {
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | 0x01;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | 0x01;
    }
  }
  else if(pin == 3 || pin == 11) {
    TCCR2B = TCCR2B & 0b11111000 | 0x01;
  }
}
