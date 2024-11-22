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

int speed = 2048; //Declare motor speed

void setupPS2controller() //Keep checking until the receiver is connected
{
  int err = -1;
  while (err != 0)
  {
    err = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, true, true);
  }

}

bool PS2control()
{   
  //In case the PS2 is not connected or lost connection with the receiver
  if (ps2x.Button(PSS_LY) == 128 && ps2x.Button(PSS_RX) == 128) 
  {
    setPWMMotor(0, 0, 0, 0, 0, 0);
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
        M1A = ps2x.Analog(PSS_LY);
        M2A = ps2x.Analog(PSS_LY);
        
        M1A = map(M1A, 128, 0, 0, speed);
        M2A = map(M2A, 128, 0, 0, speed);
        
        Serial.println("M1A: "); Serial.print(M1A);
        Serial.println("M2A: "); Serial.print(M2A);

        break;
      }
    
    //The case where the left joystick is down
    case(false):
      {
        M1B = ps2x.Analog(PSS_LY);
        M2B = ps2x.Analog(PSS_LY);

        M1B = map(M1B, 128, 255, 0, speed);
        M2B = map(M2B, 128, 255, 0, speed);

        Serial.println("M1B: "); Serial.print(M1B);
        Serial.println("M2B: "); Serial.print(M2B);        

        break;
      }
  }
  switch(X)
  {
    //The case where the right joystick is left
    case(true):
      {
        XL = ps2x.Analog(PSS_RX);
        
        XL = map(XL, 127, 0, 0, speed);
      
        Serial.println("XL: "); Serial.print(XL);

        break;
      }
    
    //The case where the left joystick is right 
    case(false):
      {
        XR = ps2x.Analog(PSS_RX);

        XR = map(XR, 127, 255, 0, speed);

        Serial.println("XR: "); Serial.print(XR);

        break;
      }
  }
  setPWMMotor(M1A, M1B, M2A, M2B, XL, XR); //Set PWM for drivertrain's motors

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
  setPWMLinear(n1, n2); //Set PWM for linear's motor

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
  setPWMIntake(s1, s2); //Set PWM for intake's motor

  //Press R1 to make servo 1 rotate clockwise
  switch(ps2x.Button(PSB_R1))
  {
    case(1):
    {
      Serial.println("Servo 1 quay cùng chiều kim đồng hồ");
      setServo360(Servo_1, CW_usec);
      break;
    }
    case(0):
    {
      setServo360(Servo_1, STOP_usec);
      break;
    }
  }
  
  //Press R2 to make servo 1 rotate counter-clockwise
  switch(ps2x.Button(PSB_R2))
  {
    case(1):
    {
      Serial.println("Servo 1 quay ngược chiều kim đồng hồ");
      setServo360(Servo_1, CCW_usec);
      break;
    }
    case(0):
    {
      setServo360(Servo_1, STOP_usec);
      break;
    }
  }

  //Press L1 to make servo 2 rotate clockwise  
  switch(ps2x.Button(PSB_L1))
  {
    case(1):
    {
      Serial.println("Servo 2 quay cùng chiều kim đồng hồ");
      setServo360(Servo_2, CW_usec);
      break;
    }
    case(0):
    {
      setServo360(Servo_2, STOP_usec);
      break;
    }
  }
  
  //Press L2 to make servo 2 rotate counter-clockwise
  switch(ps2x.Button(PSB_L2))
  {
    case(1):
    {
      Serial.println("Servo 2 quay ngược chiều kim đồng hồ");
      setServo360(Servo_2, CCW_usec);
      break;
    }
    case(0):
    {
      setServo360(Servo_2, STOP_usec);
      break;
    }
  }

  //Press Y to set Servo 3 angle to 90
  if(ps2x.Button(PSB_TRIANGLE))
  {
    Serial.println("Quay Sero 3 đến góc 90");
    setServoAngle(Servo_3, 90);
  }

  //Press X to set Servo 3 angle to 0
  if(ps2x.Button(PSB_SQUARE))
  {
    Serial.println("Quay Sero 3 đến góc 0");
    setServoAngle(Servo_3, 0);
  }  

  //Press B to set Servo 4 angle to 90
  if(ps2x.Button(PSB_CIRCLE))
  {
    Serial.println("Quay Sero 4 đến góc 90");
    setServoAngle(Servo_4, 90);
  } 

  //Press A to set Servo 3 angle to 0
  if(ps2x.Button(PSB_CROSS))
  {
    Serial.println("Quay Sero 4 đến góc 0");
    setServoAngle(Servo_4, 0);
  } 

  return 1;
}
