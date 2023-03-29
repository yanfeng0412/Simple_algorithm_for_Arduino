// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x41);
#define SERVOMIN 102
#define SERVOMAX 500
#include <Servo.h>
int leg1[] = {0,1,2};int leg2[] = {3,4,5}; int leg3[] = {6,7,8};
int leg4[] = {9,10,11}; int leg5[] = {12,13,14}; int leg6[] = {15,0,1};
double currentheta[18]={};
Adafruit_MPU6050 mpu;

double roll;
double pitch;
//double error;double last error; int duty;
float Acc_X; float Acc_Y; float Acc_Z;



void setup(void) {
  Serial.begin(115200);
  pwm.begin();
  pwm.setPWMFreq(50);
  pwm.setOscillatorFrequency(27000000);
  pwm1.begin();
  pwm1.setPWMFreq(50);
  pwm1.setOscillatorFrequency(27000000);


}

void loop() {
  sensors_event_t a, g, temp;s
  mpu.getEvent(&a, &g, &temp);
  double mpu_theta0x =  0.1*pow(a.acceleration.x,3)+0.25*pow(a.acceleration.x,2)+2*a.acceleration.x+45;
  double mpu_theta1x =  0.05*pow(a.acceleration.x,3)+0.10*pow(a.acceleration.x,2)+2*a.acceleration.x+45;
  double mpu_theta0y =  0.1*pow(a.acceleration.y,3)+0.25*pow(a.acceleration.y,2)+2*a.acceleration.y+45;
  double mpu_theta1y =  0.05*pow(a.acceleration.y,3)+0.10*pow(a.acceleration.y,2)+2*a.acceleration.y+45; 
 // roll = atan2(Acc_Y,Acc_Z)*deg;
 // pitch = atan2(-1*Acc_X,Acc_Z)*deg;
 //通过调节小腿的角度，暂时没写反馈的东西

  // if(a.acceleration.x < 0 ){ //假设x<0时候往左倾斜
  //    pwm.setPWM(leg1[1],0,angle2pulse(mpu_theta0x)); //i++
  //    pwm.setPWM(leg2[1],0,angle2pulse(mpu_theta0x)); //i++
  //    pwm.setPWM(leg3[1],0,angle2pulse(mpu_theta0x)); //i++
  //    pwm.setPWM(leg4[1],0,angle2pulse(-mpu_theta0x)); //i--
  //    pwm.setPWM(leg5[1],0,angle2pulse(-mpu_theta0x)); //i--
  //    pwm1.setPWM(leg6[1],0,angle2pulse(-mpu_theta0x)); //i--
  // }
  // if(a.acceleration.x > 0  ){ //假设>0 时候往右边倾斜
  //    double thetaroll; //假设偏转角sub进angle2pulse里面 for pid control ，目前想法
  //    pwm.setPWM(leg1[1],0,angle2pulse(-mpu_theta0x)); //i--
  //    pwm.setPWM(leg2[1],0,angle2pulse(-mpu_theta0x)); //i--
  //    pwm.setPWM(leg3[1],0,angle2pulse(-mpu_theta0x)); // i--
  //    pwm.setPWM(leg4[1],0,angle2pulse(mpu_theta0x)); //i++
  //    pwm.setPWM(leg5[1],0,angle2pulse(mpu_theta0x); //i++
  //    pwm.setPWM(leg6[1],0,angle2pulse(mpu_theta0x)); //i++
  // }
  // if(a.acceleration.y <0){ //假设y<0时候往前倾斜,前腿转角++,后退转角--，中间腿不变
    
  //    pwm.setPWM(leg1[1],0,angle2pulse(mpu_theta0y)); //I++
  //    pwm.setPWM(leg3[1],0,angle2pulse(mpu_theta0y)); //i++
  //    pwm.setPWM(leg4[1],0,angle2pulse(-mpu_theta0y)); //i--
  //    pwm.setPWM(leg6[1],0,angle2pulse(-mpu_theta0y)); //i--
  //    pwm.setPWM(leg2[1],0,angle2pulse(45)); 
  //    pwm.setPWM(leg5[1],0,angle2pulse(45));
  // }
  // if(a.acceleration.y > 0) {//假设y>0时候往后倾斜，前腿--，后腿++,中间不变
     
  //    pwm.setPWM(leg1[1],0,angle2pulse(-mpu_theta0y)); //i--
  //    pwm.setPWM(leg3[1],0,angle2pulse(-mpu_theta0y-));//i--
  //    pwm.setPWM(leg4[1],0,angle2pulse(mpu_theta0y)); //i++
  //    pwm.setPWM(leg6[1],0,angle2pulse(mpu_theta0y)); //i++
  //    pwm.setPWM(leg2[1],0,angle2pulse(45));
  //    pwm.setPWM(leg5[1],0,angle2pulse(45));
  // }
  if(a.acceleration.x < 0 && a.acceleration.y<0){ //向左+前倾斜, 左腿1,2++,右腿4,5--,后腿3+，后腿6-
    if(a.acceleration.x > a.acceleration.y){
     pwm.setPWM(leg1[1],0,angle2pulse(mpu_theta0x)); //i++
     pwm.setPWM(leg2[1],0,angle2pulse(mpu_theta0x)); //i++
     pwm.setPWM(leg3[1],0,angle2pulse(mpu_theta1x)); //i+
     pwm.setPWM(leg4[1],0,angle2pulse(-mpu_theta0x)); //i--
     pwm.setPWM(leg5[1],0,angle2pulse(-mpu_theta0x));//i--
     pwm1.setPWM(leg6[1],0,angle2pulse(-mpu_theta1x)); //i-
    }
    else
    {
      pwm.setPWM(leg1[1],0,angle2pulse(mpu_theta0y)); //i++
      pwm.setPWM(leg2[1],0,angle2pulse(mpu_theta0y)); //i++
      pwm.setPWM(leg3[1],0,angle2pulse(mpu_theta1y)); //i+
      pwm.setPWM(leg4[1],0,angle2pulse(-mpu_theta0y)); //i--
      pwm.setPWM(leg5[1],0,angle2pulse(-mpu_theta0y));//i--
      pwm1.setPWM(leg6[1],0,angle2pulse(-mpu_theta1y)); //i-
    }
  }
  if(a.acceleration.x < 0 && a.acceleration.y > 0){ //向左+后倾斜, 后腿2,3++, 右后腿5,6--，前腿1+, 后腿4-
    //y起主导作用
     pwm.setPWM(leg1[1],0,angle2pulse(mpu_theta1y)); //i+
     pwm.setPWM(leg2[1],0,angle2pulse(mpu_theta0y)); //i++
     pwm.setPWM(leg3[1],0,angle2pulse(mpu_theta0y)); //i++
     pwm.setPWM(leg4[1],0,angle2pulse(-mpu_theta1y)); //i-
     pwm.setPWM(leg5[1],0,angle2pulse(-mpu_theta0y)); //i--
     pwm1.setPWM(leg6[1],0,angle2pulse(-mpu_theta0y)); //i--
  }
  if(a.acceleration.x > 0 && a.acceleration.y < 0){ // 向右+向前倾斜，前右腿4,5++，后右腿6+， 前左腿1,2 --，后左腿3-
    //x主导
     pwm.setPWM(leg1[1],0,angle2pulse(-mpu_theta0x)); //i--
     pwm.setPWM(leg2[1],0,angle2pulse(-mpu_theta0x)); //i--
     pwm.setPWM(leg3[1],0,angle2pulse(-mpu_theta1x)); //i-
     pwm.setPWM(leg4[1],0,angle2pulse(mpu_theta0x)); //i++
     pwm.setPWM(leg5[1],0,angle2pulse(mpu_theta0x)); //i++
     pwm1.setPWM(leg6[1],0,angle2pulse(mpu_theta1x)); //i+
  }
  if (a.acceleration.x > 0 &&a.acceleration.y > 0 ){//向右+向后倾斜，后右腿5,6++,前右腿4+，后左腿2,3--，前左腿1- 
    if (a.acceleration.x > a.acceleration.y){
      pwm.setPWM(leg1[1],0,angle2pulse(-mpu_theta1x)); //i-
      pwm.setPWM(leg2[1],0,angle2pulse(-mpu_theta0x)); //i--
      pwm.setPWM(leg3[1],0,angle2pulse(-mpu_theta0x)); //i--
      pwm.setPWM(leg4[1],0,angle2pulse(mpu_theta1x)); //i+
      pwm.setPWM(leg5[1],0,angle2pulse(mpu_theta0x)); //i++
      pwm1.setPWM(leg6[1],0,angle2pulse(mpu_theta0x)); //i++
    }
    else{
      pwm.setPWM(leg1[1],0,angle2pulse(-mpu_theta1y)); //i-
      pwm.setPWM(leg2[1],0,angle2pulse(-mpu_theta0y)); //i--
      pwm.setPWM(leg3[1],0,angle2pulse(-mpu_theta0y)); //i--
      pwm.setPWM(leg4[1],0,angle2pulse(mpu_theta1y)); //i+
      pwm.setPWM(leg5[1],0,angle2pulse(mpu_theta0y)); //i++
      pwm1.setPWM(leg6[1],0,angle2pulse(mpu_theta0y)); //i++
    }
  }
  // else 
  // {
  //   pwm.setPWM(leg1[1],0,angle2pulse(0));
  //    pwm.setPWM(leg2[1],0,angle2pulse(0));
  //    pwm.setPWM(leg3[1],0,angle2pulse(0));
  //    pwm.setPWM(leg4[1],0,angle2pulse(0));
  //    pwm.setPWM(leg5[1],0,angle2pulse(0));
  //    pwm1.setPWM(leg6[1],0,angle2pulse(0));
  
    Serial.print("GyroX:");
    Serial.print(g.gyro.x);
    Serial.print(",");
    Serial.print("GyroY:");
    Serial.print(g.gyro.y);
    Serial.print(",");
    Serial.print("GyroZ:");
    Serial.print(g.gyro.z);
    Serial.println("");
    delay(10);
    

}

int angle2pulse(int angle){
  int pulse ;
  if (angle <= 180){
    pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  }
  return pulse;
}