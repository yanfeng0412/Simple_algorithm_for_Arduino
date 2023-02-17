
#include <Servo.h>

Servo myservo;  
double timestep = 0.0009;
double a0; double a1; double a2; double a3;double a4; double a5;
double omegaf;double omegai; double acci; double accf;
double tf = 0.8;
// 更改tf k
void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
}

void loop() {
  
  for (double t = 0; t <= tf; t +=timestep) { // goes from 0 degrees to 180 degrees 
      //a0 = 70;a1 = 0;a2 = 30 ;a3 = -10;
      double thetai=45; double thetaf=135;
      double omegai = 40; double omegaf = 50;
      double acci = 5; double accf = 15;
      a0 = thetai;
      a1 = omegai;
      a2 = acci/2;
      a3 = (20*thetaf-20*thetai-(8*omegaf+12*omegai)*tf-(3*acci-accf)*(pow(tf,2))) / (2*(pow(tf,3)));
      a4 = (30*thetai-30*thetaf+(14*omegaf+16*omegai)*tf+(3*acci-2*accf)*(pow(tf,2))) / (2*(pow(tf,4)));
      a5 = (12*thetaf - 12*thetai - (6*omegaf+6*omegai)*tf-(acci-accf)*(pow(tf,2))) / (2*(pow(tf,5)));
     double theta0 = a0 +a1*t +a2*(pow(t,2))+ a3*(pow(t,3)) + a4*(pow(t,4)) + a5*(pow(t,5));
     double omega0 = a1 + 2*a2*t + 3*a3*(pow(t,2)) + 4*a4*(pow(t,3)) + 5*a5*(pow(t,4));
     double acc0 = 2*a2 + 6*a3*t + 12*a4*(pow(t,2)) + 20*a5*(pow(t,3));
    myservo.write(theta0);              // tell servo to go to position in variable 'pos'
    //Serial.print(theta0);
    //Serial.println(t);
    delay(timestep*1000);                       // waits 15 ms for the servo to reach the position
  }
   for (double t = 0; t <= tf; t +=timestep) { // goes from 0 degrees to 180 degrees 
      //a0 = 70;a1 = 0;a2 = 30 ;a3 = -10;
      double thetai=135; double thetaf=45;
      double omegai = 50; double omegaf = 40;
      double acci = 15; double accf = 5;
      a0 = thetai;
      a1 = omegai;
      a2 = acci/2;
      a3 = (20*thetaf-20*thetai-(8*omegaf+12*omegai)*tf-(3*acci-accf)*(pow(tf,2))) / (2*(pow(tf,3)));
      a4 = (30*thetai-30*thetaf+(14*omegaf+16*omegai)*tf+(3*acci-2*accf)*(pow(tf,2))) / (2*(pow(tf,4)));
      a5 = (12*thetaf - 12*thetai - (6*omegaf+6*omegai)*tf-(acci-accf)*(pow(tf,2))) / (2*(pow(tf,5)));
     double theta0 = a0 +a1*t +a2*(pow(t,2))+ a3*(pow(t,3)) + a4*(pow(t,4)) + a5*(pow(t,5));
     double omega0 = a1 + 2*a2*t + 3*a3*(pow(t,2)) + 4*a4*(pow(t,3)) + 5*a5*(pow(t,4));
     double acc0 = 2*a2 + 6*a3*t + 12*a4*(pow(t,2)) + 20*a5*(pow(t,3));
    myservo.write(theta0);              // tell servo to go to position in variable 'pos'
    //Serial.print(theta0);
    //Serial.println(t);
    delay(timestep*1000);                       // waits 15 ms for the servo to reach the position
  }
 
}




