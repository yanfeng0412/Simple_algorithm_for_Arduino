#include <Servo.h>
Servo myservo;  
double timestep = 0.001;
double tf = 1;
static double theta0; 
double thetai; //起始角度 
double thetaf; //目标角度
double a0;
double a1;
double a2;
double a3;
// 更改tf k
void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
  //Serial.println(Call(cubicsp(0,135,tf)));
}
void loop() {
  //myservo.write(cubicsp(45,135,1));
  for (double t = 0; t <= tf; t +=timestep) { // goes from 0 degrees to 180 degrees 
    theta0 =cubicsp(thetai,thetaf,tf,t);
    myservo.write(theta0);
  }
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
 


