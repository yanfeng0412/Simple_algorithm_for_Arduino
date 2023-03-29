#include <Servo.h>
Servo myservo;
Servo myservo1;
int analogPin = 0;
int analogPin1 = 1;
int val,val1;
void setup()
{
  myservo.attach(9);  // 9号引脚输出电机控制信号
  myservo1.attach(10);
  Serial.begin(9600);         //仅能使用9、10号引脚
}
void loop()
{
  Serial.print("servo:");
  Serial.println(val);
  delay(15);
  Serial.print("servo1:");
  Serial.println(val1);
  val = analogRead(analogPin);
  val1 = analogRead(analogPin1);
 // 读取来自可变电阻的模拟值（0到1023之间）
  val = map(val, 0, 1023, 0, 179);     // 利用“map”函数缩放该值，得到伺服电机需要的角度（0到180之间）
  val1 = map(val1, 0, 1023, 0, 179);
  myservo.write(val);     // 设定伺服电机的位置
  myservo1.write(val1);
  delay(15);             // 等待电机旋转到目标角度
}
