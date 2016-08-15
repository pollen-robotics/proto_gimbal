// Pollen Robotics
// Inspired from http://www.berryjam.eu/2015/04/driving-bldc-gimbals-at-super-slow-speeds-with-arduino/

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

#define PHASE1 0
#define PHASE2 2.094395102393195492308428922186335
#define PHASE3 4.188790204786390984616857844372671

#define three_way_min(x,y,z) (x < y ? (x < z ? x : z) : (y < z ? y : z))
#define three_way_max(x,y,z) (x > y ? (x > z ? x : z) : (y > z ? y : z))

#include <CommandHandler.h>

//Create a CommandHandler Instance
CommandHandler cmdHdl;

const int SENSOR_PWM = 2;
const int EN = 8;
const int IN1 = 9;
const int IN2 = 10;
const int IN3 = 11;

bool direct = true;
double amplitude = 1;
double t = 0;  // position on the sinus
double period = 5;  // time for on oscillation

double newPeriod;
double minPeriod = 0.05;
double maxPeriod = 10;

double last_time;
double current_time;
double detla_time;

bool space_vector = false;
double V1;
double V2;
double V3;
double Voff;

bool pos_control = false;
double current_angle;
double target_angle = 0;
double delta_angle = 0;
double Kp = 10;

const int numReadings = 10;
int readIndex = 0;

double reading;

double sin_readings[numReadings];
double sin_total;

double cos_readings[numReadings];
double cos_total;

//////////////////////////////////////////////////////////////////////////////

void setup() {

  Serial.begin(115200); // setting up the serial port to 9600 baud

  cmdHdl.addCommand("E", setEnable);  // send E,1; to enable side, E,0; to disable
  cmdHdl.addCommand("D", setDir);  // send D,1; to set direction to one side, D,0; for the other

  cmdHdl.addCommand("SV", setSpaceVector);  // send SV,1; to enable Space Vector, SV,0; to for Sinus, default is Space Vector

  cmdHdl.addCommand("A", setAmplitude);  // send A,0.5; to change amplitude to 0.5, min 0, max 1
  cmdHdl.addCommand("F", setFreq);  // send F,1; to set frequency to 1Hz, should be above 0

  cmdHdl.addCommand("SV", setSpaceVector);  // send SV,1; to enable Space Vector, SV,0; to for Sinus, default is Space Vector

  cmdHdl.addCommand("EPC", setEnablePositionControl);  
  cmdHdl.addCommand("POS", setPositionTarget);

  cmdHdl.setDefaultHandler(unrecognized);

  //
  setPwmFrequency(IN1); // Increase PWM frequency to 32 kHz  (make unaudible)
  setPwmFrequency(IN2);
  setPwmFrequency(IN3);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);

  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);

  pinMode(SENSOR_PWM, INPUT);

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

  //
  if (pos_control == true) {
    current_angle = min(max(read_angle(), -PI), PI);
  
    delta_angle = target_angle - current_angle;

    if (delta_angle > 0) {
      direct = false;
    } else {
      direct = true;
    }
    
    setPeriod(Kp * abs(delta_angle));
  }
  
  //
  update_pwm();
}

void setPeriod(double freq) {

    if (freq > 1/maxPeriod) {
        newPeriod = 1 / freq;
        if (newPeriod < minPeriod) {
          newPeriod = minPeriod;
        }
    } else {
      newPeriod = maxPeriod;
    }
    
    t = t * newPeriod / period;
    period = newPeriod;
}

double read_angle() {
  
  reading = (pulseIn(SENSOR_PWM, HIGH) / 900.0 - 0.5) * 2 * PI;
    
  sin_readings[readIndex] = sin(reading);
  cos_readings[readIndex] = cos(reading);

  readIndex = readIndex + 1;
  if (readIndex >= numReadings) {
    readIndex = 0;
  }

  sin_total = 0;
  cos_total = 0;
  for(int i = 0; i < numReadings; i++) {
    sin_total += sin_readings[i];
    cos_total += cos_readings[i];
  }  
  
  return atan2(sin_total, cos_total);
}

void update_pwm() {

  V1 = sin_pwm_value(PHASE1);
  V2 = sin_pwm_value(PHASE2);
  V3 = sin_pwm_value(PHASE3);

  if (space_vector == true) {
    Voff = (three_way_min(V1, V2, V3) + three_way_max(V1, V2, V3)) / 2.0 - 0.5;
    V1 = V1 - Voff;
    V2 = V2 - Voff;
    V3 = V3 - Voff;
  }

  apply_pwm(IN1, V1);
  apply_pwm(IN2, V2);
  apply_pwm(IN3, V3);
}


double sin_pwm_value(double phase) {
  return amplitude * (sin(TWO_PI * t / period + phase) / 2 + 0.5);
}

void apply_pwm(int pin, double value) {
  analogWrite(pin, (int) ((value * 255) + 0.5));
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
      setPeriod(freq);
      return;
    }
  }
  sendError();
}

void setDir(){
  bool dir;
  dir = cmdHdl.readBoolArg();
  if (cmdHdl.argOk) {
    direct = dir;
    return;
  }
  sendError();
}

void setEnable(){
  bool en;
  en = cmdHdl.readBoolArg();
  if (cmdHdl.argOk) {
    digitalWrite(EN, en);
    return;
  }
  sendError();
}

void setSpaceVector(){
  bool en;
  en = cmdHdl.readBoolArg();
  if (cmdHdl.argOk) {
    space_vector = en;
    return;
  }
  sendError();
}

void setEnablePositionControl(){
  bool en;
  en = cmdHdl.readBoolArg();
  if (cmdHdl.argOk) {
    pos_control = en;
    return;
  }
  sendError();
}

void setPositionTarget(){
  double angle_deg;
  angle_deg = cmdHdl.readDoubleArg();
  if (cmdHdl.argOk) {
    if (angle_deg > -180) {
      if (angle_deg < 180) {
        target_angle = DEG_TO_RAD * angle_deg;
        return;
      }
    }
  }
  sendError();
}


void sendError(){
  cmdHdl.initCmd();
  cmdHdl.addCmdString("ERROR");
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}

// This gets set as the default handler, and gets called when no other command matches.
void unrecognized(const char *command) {
  cmdHdl.initCmd();
  cmdHdl.addCmdString("UNKNOWN");
  cmdHdl.addCmdTerm();
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
