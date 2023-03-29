#include <Servo.h>
Servo servo_1;
Servo servo_2;
int i;

void setup() {
  // put your setup code here, to run once:
  servo_1.attach(2);
  servo_2.attach(9);
  Serial.begin(9600);
}

void loop() {
    if (Serial.available()>0){
    int input = Serial.read();
    switch(input){
      case '1':
        servo_2.write(0);
      break;
      case '2':
        servo_2.write(45);
      break;
      case '3':
        servo_2.write(90);
      break;
      case '4':
        servo_2.write(135);
      break;
      case '5':
        servo_2.write(180);
      break;
    }
   }

}
