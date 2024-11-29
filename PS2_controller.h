#include <PS2X_lib.h>
PS2X ps2x;  // Create PS2 Controller Class object

#define PS2_DAT 12 
#define PS2_CMD 13 
#define PS2_SEL 15 
#define PS2_CLK 14 

#define CW_usec 1  //Value to make servo 360-degree spin clockwise
#define STOP_usec 1.5  //Value to stop servo 360-degree
#define CCW_usec 2 //Value to make servo 360-degree spin counter-clockwise 

//Calibration for PS2
#define LJ 128 
#define RJ 127

int speed = 3000; //Declare motor speed

void setupPS2controller() //Keep checking until the receiver is connected
{
  int err = -1;
  while (err != 0)
  {
    err = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, true, true);
  }

}

int D_PAD_CHECK() //Declare D_PAD Button checking function
{
  if(ps2x.Button(PSB_PAD_UP))
    return 1;
  else if(ps2x.Button(PSB_PAD_DOWN))
    return 2;
  else if(ps2x.Button(PSB_PAD_RIGHT))
    return 3;
  else if(ps2x.Button(PSB_PAD_LEFT))
    return 4;
  return 0;
}

int RL_CHECK() //Declare R1, R2, L1, L2 button checking function
{
  if(ps2x.Button(PSB_R1))
    return 1;
  else if(ps2x.Button(PSB_R2))
    return 2;
  else if(ps2x.Button(PSB_L1))
    return 3;
  else if(ps2x.Button(PSB_L2))
    return 4;
  return 0;
}

int GM_CHECK()  //Declare Geomatry button checking function
{
  if(ps2x.Button(PSB_TRIANGLE))
    return 1;
  else if(ps2x.Button(PSB_CIRCLE))
    return 2;
  else if(ps2x.Button(PSB_CROSS))
    return 3;
  else if(ps2x.Button(PSB_SQUARE))
    return 4;
  return 0;
}
bool PS2control()
{   
  //In case the PS2 is not connected or lost connection with the receiver
  if (ps2x.Analog(PSS_LY) == 128 && ps2x.Analog(PSS_RX) == 128) 
  {
    setPWMMotor(0, 0, 0, 0, 0, 0);
    setPWMLinear(0, 0);
    setPWMIntake(0, 0);
    return 0;
  }

  //Declare variables for calculation
  int M1A = 0;
  int M1B = 0;
  int M2A = 0;
  int M2B = 0;
  int XL = 0;
  int XR = 0;

  //Check the position of the joystick left and right
  bool Y = (LJ - ps2x.Analog(PSS_LY)) > 0; 
  bool X = (RJ - ps2x.Analog(PSS_RX)) > 0;
  
  switch(Y)
  {
    //The case where the left joystick is up
    case(true):
      {
        M1A = map(ps2x.Analog(PSS_LY), 128, 0, 0, 2048);
        M2A = map(ps2x.Analog(PSS_LY), 128, 0, 0, 2048);
        
        Serial.printf("M1A: %d M2A: %d", M1A, M2A);

        break;
      }
    
    //The case where the left joystick is down
    case(false):
      {
        M1B = map(ps2x.Analog(PSS_LY), 128, 255, 0, 2048);
        M2B = map(ps2x.Analog(PSS_LY), 128, 255, 0, 2048);

        Serial.printf("M1B: %d M2B: %d", M1B, M2B);    

        break;
      }
  }
  switch(X)
  {
    //The case where the right joystick is left
    case(true):
      {
        XL = map(ps2x.Analog(PSS_RX), 127, 0, 0, 2048);
      
        Serial.println("XL: "); Serial.print(XL);

        break;
      }
    
    //The case where the left joystick is right 
    case(false):
      {
        XR = map(ps2x.Analog(PSS_RX), 127, 255, 0, 2048);

        Serial.println("XR: "); Serial.print(XR);

        break;
      }
  }
  setPWMMotor(M1A, M1B, M2A, M2B, XL, XR); //Set PWM for drivertrain's motors

  switch(D_PAD_CHECK()) //Control Linear motor by D_PAD button
  {
    case(1):
      {
        setPWMLinear(speed, 1);
        break;
      }
    case(2):
      {
        setPWMLinear(speed, 0);
        break;
      }
    case(3):
      {
        setPWMIntake(speed, 1);
        break;
      }
    case(4):
      {
        setPWMIntake(speed, 0);
        break;
      }
    case(0):
      {
        setPWMLinear(0, 0);
        setPWMIntake(0, 0);
        break;
      }
  }

  switch(RL_CHECK()) //Control 360-degree servo by R1, R2, L1, L2 button 
  {
    case(1): 
    {
      Serial.println("Servo 1 quay cùng chiều kim đồng hồ");
      setServo360(Servo_1, CW_usec);
      break;
    }
    case(2):
    {
      Serial.println("Servo 1 quay ngược chiều kim đồng hồ");
      setServo360(Servo_1, CCW_usec);
      break;      
    }
    case(3):
    {
      Serial.println("Servo 2 quay cùng chiều kim đồng hồ");
      setServo360(Servo_2, CW_usec);
      break; 
    }
    case(4):
    {
      Serial.println("Servo 2 quay ngược chiều kim đồng hồ");
      setServo360(Servo_2, CCW_usec);
      break;
    }
    case(0):
    {
      setServo360(Servo_1, STOP_usec);
      setServo360(Servo_2, STOP_usec);
      break;
    }
  }

  switch(GM_CHECK()) //Control 180-degree servo by Geomatry button
  {
    case(1):
    {
      Serial.println("Quay Sero 3 đến góc 90");
      setServoAngle(Servo_3, 90);
      break;
    }
    case(4):
    {
      Serial.println("Quay Sero 3 đến góc 0");
      setServoAngle(Servo_3, 0);    
      break;  
    }
    case(2):
    {
      Serial.println("Quay Sero 4 đến góc 90");
      setServoAngle(Servo_4, 90);     
      break; 
    }
    case(3):
    {
      Serial.println("Quay Sero 4 đến góc 0");
      setServoAngle(Servo_4, 0);
      break;
    }
  }
  
  return 1;
}
