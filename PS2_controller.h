#include <PS2X_lib.h>
PS2X ps2x;  // Create PS2 Controller Class object

#define BEBUG_CTRL

// Calibration for different kinds of PS2 controller, this value only suitable for the PS2 controller comes with VRC2023 K12 Maker kit 
#define X_JOY_CALIB 127
#define Y_JOY_CALIB 128

#define PS2_DAT 12 
#define PS2_CMD 13 
#define PS2_SEL 15 
#define PS2_CLK 14 

#define TOP_SPEED 4095
#define NORM_SPEED 2048
#define TURNING_FACTOR 1

#define SINGLE_HAND_DRIVING 0
#define TWO_HAND_DRIVING 1
bool driving_mode = SINGLE_HAND_DRIVING;
void setupPS2controller()
{
  int err = -1;
  while (err != 0)
  {
    err = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, true, true);
  }

}
bool PS2control()
{
  int speed = NORM_SPEED;
  if (ps2x.Button(PSB_R2))
    speed = TOP_SPEED;
   if (ps2x.ButtonPressed(PSB_SELECT))
    driving_mode =! driving_mode;
  

  int nJoyX = X_JOY_CALIB - ps2x.Analog(PSS_RX); //Read x-joystick
  int nJoyY = Y_JOY_CALIB - (driving_mode ? ps2x.Analog(PSS_LY) :ps2x.Analog(PSS_RY)); //Read y-joystick
  int nMotMixL;                         
  int nMotMixR;                          

  if(nJoyX == -1 && nJoyY == 0) //In case of lost connection with the controller
  {
    setPWMMotors(0, 0, 0, 0);
    return 0;
  }

  bool temp = (nJoyY * nJoyX > 0);
  if (nJoyX) //Calculate variable for turning
  {
    nMotMixL = -nJoyX + (nJoyY * temp);
    nMotMixR = nJoyX + (nJoyY * !temp);
  }
  else //Calculate variable for moving forward or backward
  {
    nMotMixL = nJoyY;
    nMotMixR = nJoyY;
  }
  #ifdef BEBUG_CTRL
  Serial.print(F("Calculated value from joystick: "));
  Serial.print(nMotMixL);
  Serial.print("\t");
  Serial.println(nMotMixR);
  #endif
  int c1 = 0, c2 = 0, c3 = 0, c4 = 0;

  switch (nMotMixR >0)
    {
      case(true):
        {
          c3 = nMotMixR;
          c3 = map(c3, 0, 128, 0, speed);
          break;
        }
      case(false):
        {
          c4 = abs(nMotMixR) + 1;
          c4 = map(c4, 0, 128, 0, speed);
          break;
        }
    }

  switch (nMotMixL > 0)
    {
      case(true):
        {
          c1 = nMotMixL;
          c1 = map(c1, 0, 128, 0, speed);
          break;
        }
      case(false):
        {
          c2 = abs(nMotMixL);
          c2 = map(c2, 0, 128, 0, speed);
          break;    
        }
    }
  setPWMMotors(c1, c2, c3, c4); //Set drivertrain's motor PWM

  int n1 = 0, n2 = 0;
  if (ps2x.Button(PSB_PAD_UP)) //Set PWM value when press pad-up
  {
    n1 = speed;
    n2 = 0;
  }
  if (ps2x.Button(PSB_PAD_DOWN)) //Set PWM value when press pad-down
  {
    n1 = 0;
    n2 = speed;
  }
  setPWMLinear(n1, n2); //Set linear's motor PWM

  int s1 = 0, s2=0;
  if (ps2x.Button(PSB_PAD_RIGHT)) //Set PWM value when press pad-right
  { 
    s1 = speed;
    s2 = 0;
  }
  if (ps2x.Button(PSB_PAD_LEFT)) //Set PWM value when press pad-left
  {
    s1 = 0;
    s2 = speed;
  }
  setPWMIntake(s1, s2); //Set intake's motor PWM

  int angles[] = {0, 180}; //Declare the angle values that the servo needs to rotate to
  int angle1Index = 0;
  int angle2Index = 0;
  int angle3Index = 0;
  int angle4Index = 0;

  setServoAngle(Servo1, angles[angle1Index]); //Set servo beginning angle to 0
  setServoAngle(Servo2, angles[angle2Index]);
  setServoAngle(Servo3, angles[angle3Index]);
  setServoAngle(Servo4, angles[angle4Index]);

  if (ps2x.ButtonPressed(PSB_TRIANGLE)) //Set servo 1 angle when press triangle button (Y button)
  {
    switch (angle1Index=0) 
      {  case (true): //In case the servo 1 angle is 0, turn it to 180
        {
          angle1Index++; 
          setServoAngle(Servo1, angles[angle1Index]); 
          break;
        }
        case (false): //In case the servo 1 angle is 180, turn it to 0
        {
          angle1Index=0;
          setServoAngle(Servo1, angles[angle1Index]);
          break;
        }
      }    
  }

  if (ps2x.ButtonPressed(PSB_CIRCLE)) //Set servo 2 angle when press circle button (B button)
{
    switch (angle2Index=0) 
      {  case (true): //In case the servo 2 angle is 0, turn it to 180
        {
          angle2Index++; 
          setServoAngle(Servo2, angles[angle2Index]);
          break;
        }
        case (false): //In case the servo 2 angle is 180, turn it to 0
        {
          angle2Index=0;
          setServoAngle(Servo2, angles[angle2Index]);
          break;
        }
      }    
  }

  if (ps2x.ButtonPressed(PSB_CROSS)) //Set servo 3 angle when press cross button (A button)
{
    switch (angle3Index=0) 
      {  case (true): //In case the servo 3 angle is 0, turn it to 180
        {
          angle3Index++; 
          setServoAngle(Servo3, angles[angle3Index]);
          break;
        }
        case (false): //In case the servo 3 angle is 180, turn it to 0
        {
          angle3Index=0;
          setServoAngle(Servo3, angles[angle3Index]);
          break;
        }
      }    
  }

  if (ps2x.ButtonPressed(PSB_SQUARE)) //Set servo 2 angle when press square button (X button)
{
    switch (angle4Index=0)  
      {  case (true): //In case the servo 4 angle is 0, turn it to 180
        {
          angle4Index++; 
          setServoAngle(Servo4, angles[angle4Index]);
          break;
        }
        case (false): //In case the servo 4 angle is 180, turn it to 0
        {
          angle4Index=0;
          setServoAngle(Servo4, angles[angle4Index]);
          break;
        }
      }    
  }

  return 1;
}
