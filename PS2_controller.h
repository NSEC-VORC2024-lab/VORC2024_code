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
  if (ps2x.Button(PSB_R2) && speed == TOP_SPEED)
    speed = NORM_SPEED;
   if (ps2x.ButtonPressed(PSB_SELECT))
    driving_mode =! driving_mode;
  

  int nJoyX = X_JOY_CALIB - ps2x.Analog(PSS_RX); //Read x-joystick
  int nJoyY = Y_JOY_CALIB - (driving_mode ? ps2x.Analog(PSS_LY) :ps2x.Analog(PSS_RY)); //Read y-joystick
  int nMotMixL;                         
  int nMotMixR;                          
  
int angles[] = {0, 180}; //Declare the angle values that the servo needs to rotate to
int angle1Index = 0;
int angle2Index = 0;

bool Servo1Direction = true; //Check whether servo 1 is at the 180-degree or 0-degree position
bool Servo2Direction = true; //Check whether servo 2 is at the 180-degree or 0-degree position
bool Servo3Direction = true; //Check if servo 3 is rotating clockwise or counterclockwise
bool Servo4Direction = true; //Check if servo 4 is rotating clockwise or counterclockwise

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
  if (ps2x.Button(PSB_PAD_UP)) //Set PWM value when press D-pad up
  {
    n1 = speed;
    n2 = 0;
  }
  if (ps2x.Button(PSB_PAD_DOWN)) //Set PWM value when press D-pad down
  {
    n1 = 0;
    n2 = speed;
  }
  setPWMLinear(n1, n2); //Set linear's motor PWM

  int s1 = 0, s2=0;
  if (ps2x.Button(PSB_PAD_RIGHT)) //Set PWM value when press D-pad right
  { 
    s1 = speed;
    s2 = 0;
  }
  if (ps2x.Button(PSB_PAD_LEFT)) //Set PWM value when press D-pad left
  {
    s1 = 0;
    s2 = speed;
  }
  setPWMIntake(s1, s2); //Set intake's motor PWM

  //Press A button to adjust servo 1 angle
  switch(ps2x.ButtonPressed(PSB_CIRCLE) && Servo1Direction)
  {
    case(1 && 0):
    {
      angle1Index=0;
      Serial.println("Nút B được nhấn");
      Serial.println("Góc của Servo 1: ");
      Serial.print(angles[angle1Index]);
      setServoAngle(Servo1, angles[angle1Index]); 
    }
    case(1 && 1):
    {
      angle1Index++;
      Serial.println("Nút B được nhấn lại");
      Serial.println("Góc của Servo 1: ");
      Serial.print(angles[angle1Index]);
      setServoAngle(Servo1, angles[angle1Index]);
    }
    Servo1Direction = !Servo1Direction;
  } 

  //Press B button to adjust servo 2 angle
  switch(ps2x.ButtonPressed(PSB_CROSS) && Servo2Direction)
  {
    case(1 && 0):
    {
      angle2Index=0;
      Serial.println("Nút A được nhấn");
      Serial.println("Góc của Servo 2: ");
      Serial.print(angles[angle2Index]);
      setServoAngle(Servo2, angles[angle2Index]); 
    }
    case(1 && 1):
    {
      angle2Index++;
      Serial.println("Nút A được nhấn lại");
      Serial.println("Góc của Servo 2: ");
      Serial.print(angles[angle2Index]);
      setServoAngle(Servo2, angles[angle2Index]);
    }
    Servo2Direction = !Servo2Direction;
  } 
  
  //Press Y button to use servo 3
  switch(ps2x.ButtonPressed(PSB_TRIANGLE) && Servo3Direction)
  {
    case(1 && 0):
    {
      Serial.println("Nút Y được nhấn");
      Serial.println("Servo 3 đang quay cùng chiều kim đồng hồ");
      setServo360(Servo3, 205);      
    }
    case(1 && 1):
    {
      Serial.println("Nút Y được nhấn");
      Serial.println("Servo 3 đang quay ngược chiều kim đồng hồ"); 
      setServo360(Servo3, 410);     
    }
    Servo3Direction = !Servo3Direction;
  }
  //Press L1 to stop servo 3
  if(ps2x.ButtonPressed(PSB_L1))
    setServo360(Servo3, 307);

  //Press X button to use servo 4
  switch(ps2x.ButtonPressed(PSB_SQUARE) && Servo4Direction)
  {
    case(1 && 0):
    {
      Serial.println("Nút X được nhấn");
      Serial.println("Servo 4 đang quay cùng chiều kim đồng hồ");
      setServo360(Servo4, 205);      
    }
    case(1 && 1):
    {
      Serial.println("Nút X được nhấn");
      Serial.println("Servo 4 đang quay ngược chiều kim đồng hồ"); 
      setServo360(Servo4, 410);     
    }
    Servo4Direction = !Servo4Direction;
  }

  //Press L2 button to stop servo 4
  if(ps2x.ButtonPressed(PSB_L2))
    setServo360(Servo4, 307);
  return 1;
}
