#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x41);
#define SERVOMIN 102
#define SERVOMAX 500
#include <Servo.h>

Servo myservo;  

double a0; double a1; double a2; double a3;
int leg1[] = {0,1,2};int leg2[] = {3,4,5}; int leg3[] = {6,7,8};
int leg4[] = {9,10,11}; int leg5[] = {12,13,14}; int leg6[] = {15,0,1};


double theta1;
double theta2;
double theta3; 
double theta4;

// 更改tf k
void setup() {
  pwm.begin();
  pwm.setPWMFreq(50);
  pwm.setOscillatorFrequency(27000000);
  pwm1.begin();
  pwm1.setPWMFreq(50);
  pwm1.setOscillatorFrequency(27000000);

  Serial.begin(115200);
}

void loop(){
  if (Serial.available()>0){
    int input = Serial.read();
    switch(input){

      case '0': //站立
        initial(); 
      break;

      case '1': //原速运动
        stand(); 
      break;
      
      case '2'://加速运动
        movingstate2();
      break;
      
      case '3': //倒退运动
        reverse1();
      break;

      case '4':
        reverse2(); //加速倒退
      break;

      case '5':
        turnleft(); //加速倒退
      break;
    }
  }
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

void movingstate1() {
  double timestep = 0.01; //原速
  double tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(45,15,tf,t); //小腿1,3,5
    theta2 =cubicsp(80,100,tf,t);// 大腿1,3,5
    theta3 = cubicsp(15,45,tf,t); //小腿2,4,6
    theta4 =cubicsp(100,80,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(0));   
    pwm.setPWM(6,0,angle2pulse(0)); 
    pwm.setPWM(12,0,angle2pulse(0)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta1)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));   
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(180-theta2)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(0));   
    pwm.setPWM(9,0,angle2pulse(0)); 
    pwm.setPWM(15,0,angle2pulse(0)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta3));   
    pwm.setPWM(10,0,angle2pulse(theta3)); 
    pwm1.setPWM(0,0,angle2pulse(theta3)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta4));   
    pwm.setPWM(11,0,angle2pulse(theta4)); 
    pwm1.setPWM(1,0,angle2pulse(180-theta4)); 

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
    for (double t = 0; t <= tf; t +=timestep) { // goes from 0 degrees to 180 degrees 
    theta1 =cubicsp(15,45,tf,t);
    theta2 =cubicsp(100,80,tf,t);
    theta3 = cubicsp(45,15,tf,t); //小腿2,4,6
    theta4 =cubicsp(80,100,tf,t);// 大腿2,4,6
     //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(0));   
    pwm.setPWM(6,0,angle2pulse(0)); 
    pwm.setPWM(12,0,angle2pulse(0)); 
    //小腿1,3,5  
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta1));
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2)); 
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(180-theta2));  
       
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(0));   
    pwm.setPWM(9,0,angle2pulse(0)); 
    pwm.setPWM(15,0,angle2pulse(0));    
     //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta3));   
    pwm.setPWM(10,0,angle2pulse(theta3)); 
    pwm1.setPWM(0,0,angle2pulse(theta3)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta4));   
    pwm.setPWM(11,0,angle2pulse(theta4)); 
    pwm1.setPWM(1,0,angle2pulse(180-theta4)); 
    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
}

void movingstate2() {
  double timestep = 0.02;//加速2
  double tf = 0.3;
  for (double t = 0; t <= tf; t +=timestep) { // goes from 0 degrees to 180 degrees 
    theta1 = cubicsp(45,0,tf,t); //小腿1,3,5
    theta2 =cubicsp(70,100,tf,t);// 大腿1,3,5
    theta3 = cubicsp(0,45,tf,t); //小腿2,4,6
    theta4 =cubicsp(100,70,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(0));   
    pwm.setPWM(6,0,angle2pulse(0)); 
    pwm.setPWM(12,0,angle2pulse(0)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta1)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));   
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(180-theta2)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(0));   
    pwm.setPWM(9,0,angle2pulse(0)); 
    pwm.setPWM(15,0,angle2pulse(0)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta3));   
    pwm.setPWM(10,0,angle2pulse(theta3)); 
    pwm1.setPWM(0,0,angle2pulse(theta3)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta4));   
    pwm.setPWM(11,0,angle2pulse(theta4)); 
    pwm1.setPWM(1,0,angle2pulse(180-theta4)); 

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
    for (double t = 0; t <= tf; t +=timestep) { // goes from 0 degrees to 180 degrees 
    theta1 =cubicsp(0,45,tf,t);
    theta2 =cubicsp(100,70,tf,t);
    theta3 = cubicsp(45,0,tf,t); //小腿2,4,6
    theta4 =cubicsp(70,100,tf,t);// 大腿2,4,6
     //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(0));   
    pwm.setPWM(6,0,angle2pulse(0)); 
    pwm.setPWM(12,0,angle2pulse(0)); 
    //小腿1,3,5  
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta1));
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2)); 
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(180-theta2));  
       
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(0));   
    pwm.setPWM(9,0,angle2pulse(0)); 
    pwm.setPWM(15,0,angle2pulse(0));    
     //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta3));   
    pwm.setPWM(10,0,angle2pulse(theta3)); 
    pwm1.setPWM(0,0,angle2pulse(theta3)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta4));   
    pwm.setPWM(11,0,angle2pulse(theta4)); 
    pwm1.setPWM(1,0,angle2pulse(180-theta4)); 
    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
}

void reverse1() {
  double timestep = 0.01; //原速
  double tf = 0.3;
  for (double t = 0; t <= tf; t +=timestep) { // goes from 0 degrees to 180 degrees 
    theta1 = cubicsp(45,0,tf,t); //小腿1,3,5
    theta2 =cubicsp(110,70,tf,t);// 大腿1,3,5
    theta3 = cubicsp(0,45,tf,t); //小腿2,4,6
    theta4 =cubicsp(70,110,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(0));   
    pwm.setPWM(6,0,angle2pulse(0)); 
    pwm.setPWM(12,0,angle2pulse(0)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta1)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));   
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(180-theta2)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(0));   
    pwm.setPWM(9,0,angle2pulse(0)); 
    pwm.setPWM(15,0,angle2pulse(0)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta3));   
    pwm.setPWM(10,0,angle2pulse(theta3)); 
    pwm1.setPWM(0,0,angle2pulse(theta3)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta4));   
    pwm.setPWM(11,0,angle2pulse(theta4)); 
    pwm1.setPWM(1,0,angle2pulse(180-theta4)); 

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
    for (double t = 0; t <= tf; t +=timestep) { // goes from 0 degrees to 180 degrees 
    theta1 =cubicsp(0,45,tf,t);
    theta2 =cubicsp(70,110,tf,t);
    theta3 = cubicsp(45,0,tf,t); //小腿2,4,6
    theta4 =cubicsp(110,70,tf,t);// 大腿2,4,6
     //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(0));   
    pwm.setPWM(6,0,angle2pulse(0)); 
    pwm.setPWM(12,0,angle2pulse(0)); 
    //小腿1,3,5  
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta1));
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2)); 
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(180-theta2));  
       
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(0));   
    pwm.setPWM(9,0,angle2pulse(0)); 
    pwm.setPWM(15,0,angle2pulse(0));    
     //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta3));   
    pwm.setPWM(10,0,angle2pulse(theta3)); 
    pwm1.setPWM(0,0,angle2pulse(theta3)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta4));   
    pwm.setPWM(11,0,angle2pulse(theta4)); 
    pwm1.setPWM(1,0,angle2pulse(180-theta4)); 
    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
}

void reverse2() {
  double timestep = 0.02;//加速2
  double tf = 0.3;
  for (double t = 0; t <= tf; t +=timestep) { // goes from 0 degrees to 180 degrees 
    theta1 = cubicsp(45,0,tf,t); //小腿1,3,5
    theta2 =cubicsp(110,70,tf,t);// 大腿1,3,5
    theta3 = cubicsp(0,45,tf,t); //小腿2,4,6
    theta4 =cubicsp(70,110,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(0));   
    pwm.setPWM(6,0,angle2pulse(0)); 
    pwm.setPWM(12,0,angle2pulse(0)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta1)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));   
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(180-theta2)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(0));   
    pwm.setPWM(9,0,angle2pulse(0)); 
    pwm.setPWM(15,0,angle2pulse(0)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta3));   
    pwm.setPWM(10,0,angle2pulse(theta3)); 
    pwm1.setPWM(0,0,angle2pulse(theta3)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta4));   
    pwm.setPWM(11,0,angle2pulse(theta4)); 
    pwm1.setPWM(1,0,angle2pulse(180-theta4)); 

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
    for (double t = 0; t <= tf; t +=timestep) { // goes from 0 degrees to 180 degrees 
    theta1 =cubicsp(0,45,tf,t);
    theta2 =cubicsp(70,110,tf,t);
    theta3 = cubicsp(45,0,tf,t); //小腿2,4,6
    theta4 =cubicsp(110,70,tf,t);// 大腿2,4,6
     //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(0));   
    pwm.setPWM(6,0,angle2pulse(0)); 
    pwm.setPWM(12,0,angle2pulse(0)); 
    //小腿1,3,5  
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta1));
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2)); 
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(180-theta2));  
       
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(0));   
    pwm.setPWM(9,0,angle2pulse(0)); 
    pwm.setPWM(15,0,angle2pulse(0));    
     //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta3));   
    pwm.setPWM(10,0,angle2pulse(theta3)); 
    pwm1.setPWM(0,0,angle2pulse(theta3)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta4));   
    pwm.setPWM(11,0,angle2pulse(theta4)); 
    pwm1.setPWM(1,0,angle2pulse(180-theta4)); 
    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
}

void turnleftXXX(){
  double timestep = 0.02;//加速2
  double tf = 0.3;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(45,0,tf,t); //小腿舵机抬起
    theta2 = cubicsp(90,120,tf,t); //大腿舵机转弯
    theta3 = cubicsp(0,45,tf,t); //小腿复位
    theta4 = cubicsp(120,0,tf,t); //大腿复位
    pwm.setPWM(1,0,angle2pulse(theta1)); // 小腿1 抬起
    pwm.setPWM(2,0,angle2pulse(theta2)); //大腿1 转动  //腿1悬空状态 
    //delay(timestep*1000);    
    pwm.setPWM(10,0,angle2pulse(theta1)); //小腿4抬起
    pwm.setPWM(11,0,angle2pulse(theta2)); //大腿4 转动 //腿4悬空
    pwm.setPWM(1,0,angle2pulse(theta3)); //小腿1放下 //腿1支撑状态
    //delay(timestep*1000);
    pwm.setPWM(4,0,angle2pulse(theta1)); //小腿2 抬起
    pwm.setPWM(5,0,angle2pulse(theta2)); //大腿2 转动 // 腿2悬空
    pwm.setPWM(10,0,angle2pulse(theta3));// 小腿4放下 //腿4支撑状态
    //delay(timestep*1000);
    pwm.setPWM(13,0,angle2pulse(theta1)); //小腿5
    pwm.setPWM(14,0,angle2pulse(theta2)); //大腿5 转动 // 腿5悬空
    pwm.setPWM(4,0,angle2pulse(theta3)); // 小腿2 放下
    pwm.setPWM(2,0,angle2pulse(theta4));
    pwm.setPWM(5,0,angle2pulse(theta4));
    pwm.setPWM(11,0,angle2pulse(theta4));
    pwm.setPWM(14,0,angle2pulse(theta4));
    //delay(timestep*1000);
    pwm.setPWM(7,0,angle2pulse(theta1)); ///小腿3
    pwm.setPWM(8,0,angle2pulse(theta2)); // 大腿3 转动 //腿3悬空
    pwm.setPWM(13,0,angle2pulse(theta3)); // 小腿5 放下
    //delay(timestep*1000);
    pwm.setPWM(15,0,angle2pulse(theta1)); //小腿6
    pwm1.setPWM(0,0,angle2pulse(theta2)); //大腿6 转动 //腿6悬空
    pwm.setPWM(7,0,angle2pulse(theta3));//小腿3放下
    //delay(timestep*1000);
    pwm.setPWM(8,0,angle2pulse(theta4)); //小腿6
    pwm1.setPWM(0,0,angle2pulse(theta4)); //大腿6 转动 //腿6悬空     
  }
}

void turnleft(){
  double timestep = 0.02;//加速2
  double tf = 0.3;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(45,0,tf,t); //小腿舵机抬起
    theta2 = cubicsp(90,120,tf,t); //大腿舵机转弯
    theta3 = cubicsp(0,45,tf,t); //小腿复位
    theta4 = cubicsp(120,0,tf,t); //大腿复位
    pwm.setPWM(1,0,angle2pulse(theta1)); // 小腿1 抬起
    pwm.setPWM(2,0,angle2pulse(theta2)); //大腿1 转动  //腿1悬空状态 
     
  }
    delay(1000);

  for(double t = 0; t <= tf; t +=timestep){
    theta1 = cubicsp(45,0,tf,t); //小腿舵机抬起
    theta2 = cubicsp(90,120,tf,t); //大腿舵机转弯
    theta3 = cubicsp(0,45,tf,t); //小腿复位
    theta4 = cubicsp(120,0,tf,t); //大腿复位
    pwm.setPWM(10,0,angle2pulse(theta1)); //小腿4抬起
    pwm.setPWM(11,0,angle2pulse(theta2)); //大腿4 转动 //腿4悬空
    pwm.setPWM(1,0,angle2pulse(theta3)); //小腿1放下 //腿1支撑状态 
  }
  delay(1000);

  for(double t = 0; t <= tf; t +=timestep){
    theta1 = cubicsp(45,0,tf,t); //小腿舵机抬起
    theta2 = cubicsp(90,120,tf,t); //大腿舵机转弯
    theta3 = cubicsp(0,45,tf,t); //小腿复位
    pwm.setPWM(4,0,angle2pulse(theta1)); //小腿2 抬起
    pwm.setPWM(5,0,angle2pulse(theta2)); //大腿2 转动 // 腿2悬空
    pwm.setPWM(10,0,angle2pulse(theta3));// 小腿4放下 //腿4支撑状态
  }
  delay(1000);

  for(double t = 0; t <= tf; t +=timestep){
   theta1 = cubicsp(45,0,tf,t); //小腿舵机抬起
   theta2 = cubicsp(90,120,tf,t); //大腿舵机转弯
   theta3 = cubicsp(0,45,tf,t); //小腿复位
   theta4 = cubicsp(120,0,tf,t); //大腿复位
    pwm.setPWM(13,0,angle2pulse(theta1)); //小腿5
    pwm.setPWM(14,0,angle2pulse(theta2)); //大腿5 转动 // 腿5悬空
    pwm.setPWM(4,0,angle2pulse(theta3)); // 小腿2 放下
    pwm.setPWM(2,0,angle2pulse(theta4));
    pwm.setPWM(5,0,angle2pulse(theta4));
    pwm.setPWM(11,0,angle2pulse(theta4));
    pwm.setPWM(14,0,angle2pulse(theta4));
  }
  delay(timestep*1000);
  for(double t = 0; t <= tf; t +=timestep){
    theta1 = cubicsp(45,0,tf,t); //小腿舵机抬起
    theta2 = cubicsp(90,120,tf,t); //大腿舵机转弯
    theta3 = cubicsp(0,45,tf,t); //小腿复位
     pwm.setPWM(7,0,angle2pulse(theta1)); ///小腿3
    pwm.setPWM(8,0,angle2pulse(theta2)); // 大腿3 转动 //腿3悬空
    pwm.setPWM(13,0,angle2pulse(theta3)); // 小腿5 放下
  }
  delay(1000);
  for(double t = 0; t <= tf; t +=timestep){
    theta1 = cubicsp(45,0,tf,t); //小腿舵机抬起
    theta2 = cubicsp(90,120,tf,t); //大腿舵机转弯
    theta3 = cubicsp(0,45,tf,t); //小腿复位
    pwm.setPWM(15,0,angle2pulse(theta1)); //小腿6
    pwm1.setPWM(0,0,angle2pulse(theta2)); //大腿6 转动 //腿6悬空
    pwm.setPWM(7,0,angle2pulse(theta3));//小腿3放下
  }
  delay(1000);
  for(double t = 0; t <= tf; t +=timestep){
    theta4 = cubicsp(120,0,tf,t); //大腿复位
    pwm.setPWM(8,0,angle2pulse(theta4)); //小腿6
    pwm1.setPWM(0,0,angle2pulse(theta4)); //大腿6 转动 //腿6悬空     
  }
  delay(1000);


}




/*
void moveright(){
  for (double t = 0; t <= tf; t +=timestep) {
    theta0 = cubicsp(0,135,tf,t); //末腿踢出
    theta0_0 = cubicsp(135,0,tf,t); //末腿复位
    theta1 = cubicsp(0,45,tf,t); //小腿踢出
    theta1_1 = cubicsp(45,0,tf,t);//小腿复位
    theta2 = 0;// 大腿状态
    theta2_2 = 0; //大腿状态
    pwm.setPWM(leg4[2],0,angle2pulse(theta0));
    pwm.setPWM(leg6[2],0,angle2pulse(theta0));
    pwm.setPWM(leg2[2],0,angle2pulse(45));
    pwm.setPWM(leg2[2],0,angle2pulse(45));




  }
}
*/