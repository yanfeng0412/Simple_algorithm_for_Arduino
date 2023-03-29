#include <Servo.h>
Servo myservo;

void setup() {
  // put your setup code here, to run once:
  myservo.attach(2);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // if(Serial.available()>0){
  //   int input = Serial.read();
  //   switch(input){
  //     case '0':
  //       myservo.write(0);
  //     break;

  //     case '2':
  //         myservo.write(180);
  //     break;
  //   }
  // }
  myservo.write(0);
  delay(3000);
  myservo.write(180);
  delay(3000);
  //myservo.write(90);

  //delay(500);

  //myservo.write(180);
  //myservo.write(360);
  //delay(2000);
  //myservo.write(90);
//当我们把程序烧录进去后可以看到360度舵机在周期性正反转，
//我们通过修改值来改变旋转方向和速度（小于90为顺时针，大于90为逆时针），
//不同的值代表不同的速度，靠近90速度越小（可能因为舵机不同而不同，多尝试，
//myservo.write(90);为停止
}
