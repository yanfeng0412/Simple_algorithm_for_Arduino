#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <math.h>
#include <PS2X_lib.h>  //for v1.6
#define PS2_DAT        13  //14    
#define PS2_CMD        11  //15
#define PS2_SEL        10  //16 CS
#define PS2_CLK        12  //17
#define pressures   true
#define rumble      true
// #include "PCA9684_32Servo_ESP32.h"
// #include <WiFi.h>
// #include <WiFiClient.h>
// #include <WebServer.h>
// #include <ESPmDNS.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x41);
#define SERVOMIN 102
#define SERVOMAX 500
// #include <Servo.h>
PS2X ps2x; // create PS2 Controller Class
int error = 0;
byte type = 0;
byte vibrate = 0;
void (* resetFunc) (void) = 0;
double a0; double a1; double a2; double a3;
int leg1[] = {0,1,2};int leg2[] = {3,4,5}; int leg3[] = {6,7,8};
int leg4[] = {9,10,11}; int leg5[] = {12,13,14}; int leg6[] = {15,0,1};


double theta1;
double theta2;
double theta3; 
double theta4;

/*
Start     = initial 
Square    = Control Left Hand Crawl
Circle    = Contorl Right Hand Crawl
Triangle  = Stand 
X         = Tail Attack 
PSB_Left  = 左偏 
PSB_Right = 右偏 
PSB_UP    = 上抬 
PSB_DOWM  = 下俯 
        |
    S   |    A
------------------       
    T   |    C
        |
L1/R1 + 0,128      = Moving forward;
L1/R1 + 0,0        = Moving Forward +135degree
L1/R1 + 127,0      = Moving Left
L1/R1 + 255,0      = MOving Backward (T Quadrant)
L1/R1 + 255,128    = Moving Backward (270)
L1/R1 + 255，255   = Moving Backward (C Quadrant)
L1/R1 + 127,255    = Moving Right ()
L1/R1 + 0,255      = Moving Forward(A Quadrant)

L2 + 0,128    = LeftHand Moving Up
L2 + 0,0      = LeftHand Rotate to Left
L2 + 127,0    = LeftHand Rotate to Left 
L2 + 255,0    = LeftHand Moving Down //暂时不用
L2 + 255,128  = LeftHand Moving Down 
L2 + 255,255  = LeftHand Moving Down  //暂时不用
L2 + 127,255  = LeftHand Rotate to Right 
L2 + 0,255    = LeftHand Rotate to Right

R2 + 0,128    = RightHand Moving Up
R2 + 0,0      = RightHand Rotate to Left
R2 + 127,0    = RightHand Rotate to Left 
R2 + 255,0    = RightHand Moving Down //暂时不用
R2 + 255,128  = RightHand Moving Down 
R2 + 255,255  = RightHand Moving Down  //暂时不用
R2 + 127,255  = RightHand Rotate to Right 
R2 + 0,255    = RightHand Rotate to Right


*/

void setup() {
  pwm.begin();
  pwm.setPWMFreq(50);
  pwm.setOscillatorFrequency(27000000);
  pwm1.begin();
  pwm1.setPWMFreq(50);
  pwm1.setOscillatorFrequency(27000000);
  Serial.begin(115200);
  
  //PS2 Config Setup
  delay(500);  
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  
  if(error == 0){
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
	if (pressures)
	  Serial.println("true ");
	else
	  Serial.println("false");
	Serial.print("rumble = ");
	if (rumble)
	  Serial.println("true)");
	else
	  Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }  
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
  type = ps2x.readType(); 
  switch(type) {
    case 0:
      Serial.println("Unknown Controller type found ");
      break;
    case 1:
      Serial.println("DualShock Controller found ");
      break;
    case 2:
      Serial.println("GuitarHero Controller found ");
      break;
	case 3:
      Serial.println("Wireless Sony DualShock Controller found ");
      break;
   }
}

void loop() {
  /* You must Read Gamepad to get new values and set vibration values
     ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
     if you don't enable the rumble, use ps2x.read_gamepad(); with no values
     You should call this at least once a second
   */  
  if(error == 1){ //skip loop if no controller found
    resetFunc();
  }
  
  if(type == 2){ //Guitar Hero Controller
    ps2x.read_gamepad();          //read controller 
   
    if(ps2x.ButtonPressed(GREEN_FRET))
      Serial.println("Green Fret Pressed");
    if(ps2x.ButtonPressed(RED_FRET))
      Serial.println("Red Fret Pressed");
    if(ps2x.ButtonPressed(YELLOW_FRET))
      Serial.println("Yellow Fret Pressed");
    if(ps2x.ButtonPressed(BLUE_FRET))
      Serial.println("Blue Fret Pressed");
    if(ps2x.ButtonPressed(ORANGE_FRET))
      Serial.println("Orange Fret Pressed"); 

    if(ps2x.ButtonPressed(STAR_POWER))
      Serial.println("Star Power Command");
    
    if(ps2x.Button(UP_STRUM))          //will be TRUE as long as button is pressed
      Serial.println("Up Strum");
    if(ps2x.Button(DOWN_STRUM))
      Serial.println("DOWN Strum");
 
    if(ps2x.Button(PSB_START))         //will be TRUE as long as button is pressed
      Serial.println("Start is being held");
     

    if(ps2x.Button(PSB_SELECT))
      Serial.println("Select is being held");
    
    if(ps2x.Button(ORANGE_FRET)) {     // print stick value IF TRUE
      Serial.print("Wammy Bar Position:");
      Serial.println(ps2x.Analog(WHAMMY_BAR), DEC); 
    } 
  }
  else { //DualShock Controller
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
    
    if(ps2x.Button(PSB_START)){         //will be TRUE as long as button is pressed
      Serial.println("Start is being held");}
    if(ps2x.Button(PSB_SELECT)){
      Serial.println("Select is being held");}
    if(ps2x.Button(PSB_PAD_UP)) {      //will be TRUE as long as button is pressed
      Serial.print("Up held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
      // LiftUp();
    }
    if(ps2x.Button(PSB_PAD_RIGHT)){
        Serial.print("Right held this hard: ");
        Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
        //RightLeaning();
        Serial.println("Robot Right Leaning");
      }
    if(ps2x.Button(PSB_PAD_LEFT)){
      Serial.print("LEFT held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
      //LeftLeaning();
      Serial.println("Robot Left Leaning");
      }
    if(ps2x.Button(PSB_PAD_DOWN)){
      Serial.print("DOWN held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
      //DownLeaning();
      Serial.println("Robot Down Leaning");
      }   

   
   
    if(ps2x.ButtonPressed(PSB_SQUARE)){           //will be TRUE if button was JUST pressed
      Serial.println("Square just pressed,Left Claw Close");
      //LeftClawClose();
      }
    if(ps2x.ButtonReleased(PSB_SQUARE)){          //will be TRUE if button was JUST released
      Serial.println("Square just released,Left Claw Open");
      //LeftClawOpen();
      }
    if(ps2x.ButtonPressed(PSB_CIRCLE)){               //will be TRUE if button was JUST pressed
      Serial.println("Circle just pressed,Right Claw Close");
      //RightClawClose();
      }
    if(ps2x.ButtonReleased(PSB_CIRCLE)){             //will be TRUE if button was JUST released
      Serial.println("Circle just released,Right Claw Open");
      //RightClawOpen();
      }
    if(ps2x.ButtonPressed(PSB_CROSS)){               //will be TRUE if button was JUST pressed
      Serial.println("Cross just pressed,Tail Attack On");
        //this will set the large motor vibrate speed based on how hard you press the blue (X) button//TailAttackOn();
     }
    if(ps2x.ButtonReleased(PSB_CROSS)){              //will be TRUE if button was JUST released
      Serial.println("Cross just released,Tail Attack Off"); 
      //TailAttackOff();
      }
    if(ps2x.ButtonPressed(PSB_TRIANGLE)){             //will be TRUE if button was JUST pressed
      Serial.println("Triangle just pressed,Initial State");
        initial();
      }


    if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { // print stick values if either is TRUE
      if (ps2x.Analog(PSS_LY) == 0 && ps2x.Analog(PSS_LX) == 128) {
        movingforward();
        Serial.println("Move Forward State");}
      if (ps2x.Analog(PSS_LY) == 255 && ps2x.Analog(PSS_LX) == 128) {
        movingbackward();
        Serial.println("Move Backward State");}
      if (ps2x.Analog(PSS_LY) == 127 && ps2x.Analog(PSS_LX) == 0) {
        movingleft();
        Serial.println("Move Left State"); }
      if (ps2x.Analog(PSS_LY) == 127 && ps2x.Analog(PSS_LX) == 255) {
        movingright();
        Serial.println("Move Right State");}
      // if (ps2x.Analog(PSS_LY) == 0 &&  ps2x.Analog(PSS_LX) == 0) {
      //   // movingright();
      //   Serial.println("Move Left Forward State");}
      // if (ps2x.Analog(PSS_LY) == 255 && ps2x.Analog(PSS_LX) == 0) {
      //  // movingleftbackward();
      //   Serial.println("Move Left Backward State");}
      // if ( ps2x.Analog(PSS_LY) == 0 && ps2x.Analog(PSS_LX) == 255) {
      //  // movingrightforward();
      //   Serial.println("Move Right Forward State");}
      // if (ps2x.Analog(PSS_LY) == 255 &&  ps2x.Analog(PSS_LX) == 255) {
      //  // movingrightbackward();
      //   Serial.println("Move Right Backward State");}
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if(ps2x.Button(PSB_L2)) { //Left Hand Rotate & Lift up down
        int y = ps2x.Analog(PSS_LY);
        int x = ps2x.Analog(PSS_LX);
        int angley = map(y,0,255,0,180);
        int anglex = map(x,0,255,0,180);
        pwm1.setPWM(3,0,angle2pulse(angley)); // Left Hand Lift Up & Down Servo /////!!!!!!!!1
        pwm1.setPWM(2,0,angle2pulse(anglex)); // Left Hand Rotate to Left and Right ////////!!!!!!!!!!11
      }

     if(ps2x.Button(PSB_R2)) { //print stick values if either is TRUE
        int y = ps2x.Analog(PSS_RY);
        int x = ps2x.Analog(PSS_RX);
        int angley = map(y,0,255,0,180);
        int anglex = map(x,0,255,0,180);
        pwm1.setPWM(6,0,angle2pulse(angley)); // Left Hand Lift Up & Down Servo /////!!!!!!!!1
        pwm1.setPWM(5,0,angle2pulse(anglex)); // Left Hand Rotate to Left and Right ////////!!!!!!!!!!11
     }
  }
    delay(50);    
}




int angle2pulse(int angle){
  int pulse ;
  if (angle <= 180){
    pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  }
  return pulse;
}

double cubicsp(double _thetai, double _thetaf,double _tf,double t){
      //a0 = 70;a1 = 0;a2 = 30 ;a3 = -10;
      //double thetai=45; double thetaf=135;
      double _theta0;
      a0 = _thetai;
      a1 = 0;
      a2 = 3/(pow(_tf,2))*(_thetaf - _thetai);
      a3 = -2/(pow(_tf,3))*(_thetaf - _thetai);
      _theta0 = a0 + a1*(t) +a2*(pow(t,2)) +a3*(pow(t,3));
    
      return _theta0; 
  }


void initial(){
   pwm.setPWM(0,0,angle2pulse(90));
  pwm.setPWM(1,0,angle2pulse(0));
  pwm.setPWM(2,0,angle2pulse(0));

  pwm.setPWM(3,0,angle2pulse(90));
  pwm.setPWM(4,0,angle2pulse(0));
  pwm.setPWM(5,0,angle2pulse(0));

  pwm.setPWM(6,0,angle2pulse(90));
  pwm.setPWM(7,0,angle2pulse(0));
  pwm.setPWM(8,0,angle2pulse(0));

  pwm.setPWM(9,0,angle2pulse(90));
  pwm.setPWM(10,0,angle2pulse(0));
  pwm.setPWM(11,0,angle2pulse(0));

  pwm.setPWM(12,0,angle2pulse(90));
  pwm.setPWM(13,0,angle2pulse(0));
  pwm.setPWM(14,0,angle2pulse(0));

  pwm.setPWM(15,0,angle2pulse(90));
  pwm1.setPWM(0,0,angle2pulse(0));
  pwm1.setPWM(1,0,angle2pulse(0));
}

void stand(){
  pwm.setPWM(0,0,angle2pulse(90));
  pwm.setPWM(1,0,angle2pulse(90));
  pwm.setPWM(2,0,angle2pulse(45));

  pwm.setPWM(3,0,angle2pulse(90));
  pwm.setPWM(4,0,angle2pulse(90));
  pwm.setPWM(5,0,angle2pulse(45));

  pwm.setPWM(6,0,angle2pulse(90));
  pwm.setPWM(7,0,angle2pulse(90));
  pwm.setPWM(8,0,angle2pulse(45));

  pwm.setPWM(9,0,angle2pulse(90));
  pwm.setPWM(10,0,angle2pulse(90));
  pwm.setPWM(11,0,angle2pulse(45));

  pwm.setPWM(12,0,angle2pulse(90));
  pwm.setPWM(13,0,angle2pulse(90));
  pwm.setPWM(14,0,angle2pulse(45));

  pwm.setPWM(15,0,angle2pulse(90));
  pwm1.setPWM(0,0,angle2pulse(90));
  pwm1.setPWM(1,0,angle2pulse(45));

}

void movingforward() {
  double timestep = 0.01;
  double tf = 0.4;
  for(double t = 0; t <= tf; t +=timestep) {
    theta1 = cubicsp(90,30,tf,t); //小腿1,3,5
    theta2 =cubicsp(60,120,tf,t);// 大腿1,3,5
    theta3 = cubicsp(30,90,tf,t); //小腿2,4,6
    theta4 =cubicsp(120,60,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(2,0,angle2pulse(45));
    pwm.setPWM(8,0,angle2pulse(45));
    pwm.setPWM(14,0,angle2pulse(45));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta1));
    //大腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta2));   
    pwm.setPWM(6,0,angle2pulse(theta2)); 
    pwm.setPWM(12,0,angle2pulse(180-theta2)); 
    //末腿2，4，6
    pwm.setPWM(5,0,angle2pulse(45));   
    pwm.setPWM(11,0,angle2pulse(45)); 
    pwm1.setPWM(1,0,angle2pulse(45)); 
    //小腿2，4，6
    pwm.setPWM(4,0,angle2pulse(theta3));   
    pwm.setPWM(10,0,angle2pulse(theta3)); 
    pwm1.setPWM(0,0,angle2pulse(theta3)); 
    //大腿2，4，6
    pwm.setPWM(3,0,angle2pulse(theta4));   
    pwm.setPWM(9,0,angle2pulse(180-theta4)); 
    pwm.setPWM(15,0,angle2pulse(180-theta4)); 
    delay(timestep*1000);
  }
  for(double t = 0; t <= tf; t +=timestep) {
    theta1 = cubicsp(30,90,tf,t); //小腿1,3,5
    theta2 =cubicsp(120,60,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90,30,tf,t); //小腿2,4,6
    theta4 =cubicsp(60,120,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(2,0,angle2pulse(45));
    pwm.setPWM(8,0,angle2pulse(45));
    pwm.setPWM(14,0,angle2pulse(45));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta1));
    //大腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta2));   
    pwm.setPWM(6,0,angle2pulse(theta2)); 
    pwm.setPWM(12,0,angle2pulse(180-theta2)); 
    //末腿2，4，6
    pwm.setPWM(5,0,angle2pulse(45));   
    pwm.setPWM(11,0,angle2pulse(45)); 
    pwm1.setPWM(1,0,angle2pulse(45)); 
    //小腿2，4，6
    pwm.setPWM(4,0,angle2pulse(theta3));   
    pwm.setPWM(10,0,angle2pulse(theta3)); 
    pwm1.setPWM(0,0,angle2pulse(theta3)); 
    //大腿2，4，6
    pwm.setPWM(3,0,angle2pulse(theta4));   
    pwm.setPWM(9,0,angle2pulse(180-theta4)); 
    pwm.setPWM(15,0,angle2pulse(180-theta4)); 
    delay(timestep*1000);
  }
}

void movingstate2() {
  double timestep = 0.01;
  double tf = 0.2;
  for(double t = 0; t <= tf; t +=timestep) {
    theta1 = cubicsp(90,30,tf,t); //小腿1,3,5
    theta2 =cubicsp(75,110,tf,t);// 大腿1,3,5
    theta3 = cubicsp(30,90,tf,t); //小腿2,4,6
    theta4 =cubicsp(110,75,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(2,0,angle2pulse(45));
    pwm.setPWM(8,0,angle2pulse(45));
    pwm.setPWM(14,0,angle2pulse(45));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta1));
    //大腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta2));   
    pwm.setPWM(6,0,angle2pulse(theta2)); 
    pwm.setPWM(12,0,angle2pulse(180-theta2)); 
    //末腿2，4，6
    pwm.setPWM(5,0,angle2pulse(45));   
    pwm.setPWM(11,0,angle2pulse(45)); 
    pwm1.setPWM(1,0,angle2pulse(45)); 
    //小腿2，4，6
    pwm.setPWM(4,0,angle2pulse(theta3));   
    pwm.setPWM(10,0,angle2pulse(theta3)); 
    pwm1.setPWM(0,0,angle2pulse(theta3)); 
    //大腿2，4，6
    pwm.setPWM(3,0,angle2pulse(theta4));   
    pwm.setPWM(9,0,angle2pulse(theta4)); 
    pwm.setPWM(15,0,angle2pulse(180-theta4)); 
    delay(timestep*1000);
  }
  for(double t = 0; t <= tf; t +=timestep) {
    theta1 = cubicsp(30,90,tf,t); //小腿1,3,5
    theta2 =cubicsp(110,75,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90,30,tf,t); //小腿2,4,6
    theta4 =cubicsp(75,110,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(2,0,angle2pulse(45));
    pwm.setPWM(8,0,angle2pulse(45));
    pwm.setPWM(14,0,angle2pulse(45));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta1));
    //大腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta2));   
    pwm.setPWM(6,0,angle2pulse(theta2)); 
    pwm.setPWM(12,0,angle2pulse(180-theta2)); 
    //末腿2，4，6
    pwm.setPWM(5,0,angle2pulse(45));   
    pwm.setPWM(11,0,angle2pulse(45)); 
    pwm1.setPWM(1,0,angle2pulse(45)); 
    //小腿2，4，6
    pwm.setPWM(4,0,angle2pulse(theta3));   
    pwm.setPWM(10,0,angle2pulse(theta3)); 
    pwm1.setPWM(0,0,angle2pulse(theta3)); 
    //大腿2，4，6
    pwm.setPWM(3,0,angle2pulse(theta4));   
    pwm.setPWM(9,0,angle2pulse(theta4)); 
    pwm.setPWM(15,0,angle2pulse(180-theta4)); 
    delay(timestep*1000);
  }
}

void RightclawClose(){
  pwm1.setPWM(3,0,angle2pulse(180));
  delay(50);
}

void RightclawOpen(){
  pwm1.setPWM(3,0,angle2pulse(0));
  delay(50);
}

void LeftclawClose(){
  pwm1.setPWM(4,0,angle2pulse(180));
  delay(50);
}

void LeftclawOpen(){
  pwm1.setPWM(4,0,angle2pulse(0));
  delay(50);
}

void TailAttackOn(){
  pwm1.setPWM(4,0,angle2pulse(180));
  delay(50);
}

void TailAttackOff(){
  pwm1.setPWM(4,0,angle2pulse(0));
  delay(50);
}

void movingbackward(){
  double timestep = 0.01;
  double tf = 0.4;
  for(double t = 0; t <= tf; t +=timestep) {
    theta1 = cubicsp(30,90,tf,t); //小腿1,3,5
    theta2 =cubicsp(120,60,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90,30,tf,t); //小腿2,4,6
    theta4 =cubicsp(60,120,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(2,0,angle2pulse(45));
    pwm.setPWM(8,0,angle2pulse(45));
    pwm.setPWM(14,0,angle2pulse(45));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta1));
    //大腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta2));   
    pwm.setPWM(6,0,angle2pulse(theta2)); 
    pwm.setPWM(12,0,angle2pulse(180-theta2)); 
    //末腿2，4，6
    pwm.setPWM(5,0,angle2pulse(45));   
    pwm.setPWM(11,0,angle2pulse(45)); 
    pwm1.setPWM(1,0,angle2pulse(45)); 
    //小腿2，4，6
    pwm.setPWM(4,0,angle2pulse(theta3));   
    pwm.setPWM(10,0,angle2pulse(theta3)); 
    pwm1.setPWM(0,0,angle2pulse(theta3)); 
    //大腿2，4，6
    pwm.setPWM(3,0,angle2pulse(theta4));   
    pwm.setPWM(9,0,angle2pulse(180-theta4)); 
    pwm.setPWM(15,0,angle2pulse(180-theta4)); 
    delay(timestep*1000);
  }
  for(double t = 0; t <= tf; t +=timestep) {
    theta1 = cubicsp(90,30,tf,t); //小腿1,3,5
    theta2 =cubicsp(60,120,tf,t);// 大腿1,3,5
    theta3 = cubicsp(30,90,tf,t); //小腿2,4,6
    theta4 =cubicsp(120,60,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(2,0,angle2pulse(45));
    pwm.setPWM(8,0,angle2pulse(45));
    pwm.setPWM(14,0,angle2pulse(45));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta1));
    //大腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta2));   
    pwm.setPWM(6,0,angle2pulse(theta2)); 
    pwm.setPWM(12,0,angle2pulse(180-theta2)); 
    //末腿2，4，6
    pwm.setPWM(5,0,angle2pulse(45));   
    pwm.setPWM(11,0,angle2pulse(45)); 
    pwm1.setPWM(1,0,angle2pulse(45)); 
    //小腿2，4，6
    pwm.setPWM(4,0,angle2pulse(theta3));   
    pwm.setPWM(10,0,angle2pulse(theta3)); 
    pwm1.setPWM(0,0,angle2pulse(theta3)); 
    //大腿2，4，6
    pwm.setPWM(3,0,angle2pulse(theta4));   
    pwm.setPWM(9,0,angle2pulse(180-theta4)); 
    pwm.setPWM(15,0,angle2pulse(180-theta4)); 
    delay(timestep*1000);
  }


}

void movingleft(){
  double timestep = 0.01;
  double tf = 0.4;
  for(double t = 0; t <= tf; t +=timestep) {
    theta1 = cubicsp(0,45,tf,t); //小腿1,3,5
    theta2 =cubicsp(45,15,tf,t);// 末腿1,3,5
    theta3 = cubicsp(45,0,tf,t); //小腿2,4,6
    theta4 =cubicsp(15,45,tf,t);// 末腿2,4,6
    //末腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));
    pwm.setPWM(8,0,angle2pulse(theta2));
    pwm.setPWM(14,0,angle2pulse(theta2));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta1));
    //大腿1,3,5
    pwm.setPWM(0,0,angle2pulse(90));   
    pwm.setPWM(6,0,angle2pulse(90)); 
    pwm.setPWM(12,0,angle2pulse(90)); 
    //末腿2，4，6
    pwm.setPWM(5,0,angle2pulse(theta4));   
    pwm.setPWM(11,0,angle2pulse(theta4)); 
    pwm1.setPWM(1,0,angle2pulse(theta4)); 
    //小腿2，4，6
    pwm.setPWM(4,0,angle2pulse(theta3));   
    pwm.setPWM(10,0,angle2pulse(theta3)); 
    pwm1.setPWM(0,0,angle2pulse(theta3)); 
    //大腿2，4，6
    pwm.setPWM(3,0,angle2pulse(90));   
    pwm.setPWM(9,0,angle2pulse(90)); 
    pwm.setPWM(15,0,angle2pulse(90)); 
    delay(timestep*1000);
  }
  for(double t = 0; t <= tf; t +=timestep) {
    theta1 = cubicsp(45,0,tf,t); //小腿1,3,5
    theta2 =cubicsp(15,45,tf,t);// 末腿1,3,5
    theta3 = cubicsp(0,45,tf,t); //小腿2,4,6
    theta4 =cubicsp(45,15,tf,t);// 末腿2,4,6
    //末腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));
    pwm.setPWM(8,0,angle2pulse(theta2));
    pwm.setPWM(14,0,angle2pulse(theta2));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta1));
    //大腿1,3,5
    pwm.setPWM(0,0,angle2pulse(90));   
    pwm.setPWM(6,0,angle2pulse(90)); 
    pwm.setPWM(12,0,angle2pulse(90)); 
    //末腿2，4，6
    pwm.setPWM(5,0,angle2pulse(theta4));   
    pwm.setPWM(11,0,angle2pulse(theta4)); 
    pwm1.setPWM(1,0,angle2pulse(theta4)); 
    //小腿2，4，6
    pwm.setPWM(4,0,angle2pulse(theta3));   
    pwm.setPWM(10,0,angle2pulse(theta3)); 
    pwm1.setPWM(0,0,angle2pulse(theta3)); 
    //大腿2，4，6
    pwm.setPWM(3,0,angle2pulse(90));   
    pwm.setPWM(9,0,angle2pulse(90)); 
    pwm.setPWM(15,0,angle2pulse(90)); 
    delay(timestep*1000);
  }

}

void movingright(){
  double timestep = 0.01;
  double tf = 0.4;
  for(double t = 0; t <= tf; t +=timestep) {
    theta1 = cubicsp(45,0,tf,t); //小腿1,3,5
    theta2 =cubicsp(15,45,tf,t);// 末腿1,3,5
    theta3 = cubicsp(0,45,tf,t); //小腿2,4,6
    theta4 =cubicsp(45,15,tf,t);// 末腿2,4,6
    //末腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));
    pwm.setPWM(8,0,angle2pulse(theta2));
    pwm.setPWM(14,0,angle2pulse(theta2));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta1));
    //大腿1,3,5
    pwm.setPWM(0,0,angle2pulse(90));   
    pwm.setPWM(6,0,angle2pulse(90)); 
    pwm.setPWM(12,0,angle2pulse(90)); 
    //末腿2，4，6
    pwm.setPWM(5,0,angle2pulse(theta4));   
    pwm.setPWM(11,0,angle2pulse(theta4)); 
    pwm1.setPWM(1,0,angle2pulse(theta4)); 
    //小腿2，4，6
    pwm.setPWM(4,0,angle2pulse(theta3));   
    pwm.setPWM(10,0,angle2pulse(theta3)); 
    pwm1.setPWM(0,0,angle2pulse(theta3)); 
    //大腿2，4，6
    pwm.setPWM(3,0,angle2pulse(90));   
    pwm.setPWM(9,0,angle2pulse(90)); 
    pwm.setPWM(15,0,angle2pulse(90)); 
    delay(timestep*1000);
  }
  for(double t = 0; t <= tf; t +=timestep) {
    theta1 = cubicsp(0,45,tf,t); //小腿1,3,5
    theta2 =cubicsp(45,15,tf,t);// 末腿1,3,5
    theta3 = cubicsp(45,0,tf,t); //小腿2,4,6
    theta4 =cubicsp(15,45,tf,t);// 末腿2,4,6  
    //末腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));
    pwm.setPWM(8,0,angle2pulse(theta2));
    pwm.setPWM(14,0,angle2pulse(theta2));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta1));
    //大腿1,3,5
    pwm.setPWM(0,0,angle2pulse(90));   
    pwm.setPWM(6,0,angle2pulse(90)); 
    pwm.setPWM(12,0,angle2pulse(90)); 
    //末腿2，4，6
    pwm.setPWM(5,0,angle2pulse(theta4));   
    pwm.setPWM(11,0,angle2pulse(theta4)); 
    pwm1.setPWM(1,0,angle2pulse(theta4)); 
    //小腿2，4，6
    pwm.setPWM(4,0,angle2pulse(theta3));   
    pwm.setPWM(10,0,angle2pulse(theta3)); 
    pwm1.setPWM(0,0,angle2pulse(theta3)); 
    //大腿2，4，6
    pwm.setPWM(3,0,angle2pulse(90));   
    pwm.setPWM(9,0,angle2pulse(90)); 
    pwm.setPWM(15,0,angle2pulse(90)); 
    delay(timestep*1000);
  }

}

