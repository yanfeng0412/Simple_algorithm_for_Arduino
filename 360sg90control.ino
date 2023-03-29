#include <Servo.h> 
Servo myservo;
void setup() 
{
     
  myservo.attach(3);
} 

void loop() 
{
    
    myservo.writeMicroseconds(500);  // 舵机顺时针旋转  
    delay(2000);
    myservo.writeMicroseconds(1500);// 舵机stop  
    delay(2000);
    myservo.writeMicroseconds(2500);// 舵机逆时针旋转  
    delay(2000);
    myservo.writeMicroseconds(1500);// 舵机stop  
    delay(2000);
    //360度舵机正反转
    //控制pwm(开发板上带有波浪线的数字引脚)
    //可以修改myservo.writeMicroseconds(1500); 中的值控制舵机，1500时控制舵机停止旋转。
    //500~1500控制顺时针旋转，值越小，旋转速度越大
    //1500~2500控制逆时针旋转，值越大，旋转速度越大
} 