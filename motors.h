#include <stdio.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define MIN_PWM 0
#define MAX_PWM 4095

#define PWM_CHANNEL1 8
#define PWM_CHANNEL2 9
#define PWM_CHANNEL3 10
#define PWM_CHANNEL4 11
#define PWM_CHANNEL5 12
#define PWM_CHANNEL6 13
#define PWM_CHANNEL7 14
#define PWM_CHANNEL8 15
#define Servo1 7
#define Servo2 6
#define Servo3 5
#define Servo4 4
#define MIN_PULSE_WIDTH 150 //PWM pulse-width turn servo angle to 0
#define MAX_PULSE_WIDTH 600 //PWM pulse-width turn servo angle to 180

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setPWMMotors(int c1, int c2, int c3, int c4) //Declare drivertrain's motor controlling function
{
  char dbg_str[30];
  sprintf(dbg_str,"C1: %d\tC2: %d\tC3: %d\tC4: %d",c1,c2,c3,c4);
  Serial.println(dbg_str);

  pwm.setPWM(PWM_CHANNEL5, 0, c1);
  pwm.setPWM(PWM_CHANNEL6, 0, c2);
  pwm.setPWM(PWM_CHANNEL7, 0, c3);
  pwm.setPWM(PWM_CHANNEL8, 0, c4);
}

void setPWMLinear(int n1, int n2) //Declare linear's motor controlling function
{
  pwm.setPWM(PWM_CHANNEL1, 0, n1);
  pwm.setPWM(PWM_CHANNEL2, 0, n2);
}

void setPWMIntake(int s1, int s2) //Declare intake's motor controlling function
{
  pwm.setPWM(PWM_CHANNEL3, 0, s1);
  pwm.setPWM(PWM_CHANNEL4, 0, s2);
}

void setServoAngle(int channel , int angle) //Declare servo's angle controlling function
{
  pwm.setPWMFreq(60);
  int pulseWidth = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  pwm.setPWM(channel, 0, pulseWidth);
}

void initMotors()
{
  Wire.begin(); // SDA, SCL,400000);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(1600);
  Wire.setClock(400000);
  setPWMMotors(0, 0, 0, 0);
  setPWMLinear(0, 0);
  setPWMIntake(0, 0);
}