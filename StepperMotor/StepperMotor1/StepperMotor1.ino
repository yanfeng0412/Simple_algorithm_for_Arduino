// //ArduinoMega with TB6600 with 24V 2A Voltage module
// int PUL = 7;  //定义脉冲引脚
// int DIR = 6; // 定义方向
// int ENA = 5;  //定义启用引脚
// int RESET ;// = 4;  //定义复位传感器

// void setup(){
//   pinMode(PUL,OUTPUT);
//   pinMode(DIR,OUTPUT);
//   pinMode(ENA,OUTPUT);
//   pinMode(RESET,INPUT);
//   Serial.begin(9600);
//   //初始化复位 转3圈
//   for(int i = 0 ; i<4800;i++){ //前进4800，SW1 = OFF,SW2 = ON, SW3 = OFF(每圈1600脉冲)
//     digitalWrite(DIR,LOW);// 定义正转
//     digitalWrite(ENA,HIGH);//启动
//     digitalWrite(PUL,HIGH);//输出脉冲
//     delayMicroseconds(2000);
//     digitalWrite(PUL,LOW);
//     //复位传感器；
//     int reset = Serial.read();
//     if(reset == 0){
//       break; //检测到达复位位置后停止转动
//     }
//   }
//   delayMicroseconds(2000);
// }

// void loop(){
//   for(int i = 0; i< 1600; i++){
//     digitalWrite(DIR,LOW);
//     digitalWrite(ENA,HIGH);
//     digitalWrite(PUL,HIGH);
//     delayMicroseconds(50);
//     digitalWrite(PUL,LOW);
//     delayMicroseconds(50);
//   }
//   delay(1000);
//   for(int i = 0; i< 1600; i++){
//     digitalWrite(DIR,HIGH);
//     digitalWrite(ENA,HIGH);
//     digitalWrite(PUL,HIGH);
//     delayMicroseconds(50);
//     digitalWrite(PUL,LOW);
//     delayMicroseconds(50);
//   }

// }

const int stepPin = 5;  //PUL+
const int dirPin = 2;  //DiR+
const int enPin = 8;  //ENA+

void setup() {
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,LOW);
  
}

void loop() {
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  for(int x = 0; x < 800; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(500); 
  }
  delay(1000); // One second delay
  digitalWrite(dirPin,LOW); //Changes the direction of rotation
  for(int x = 0; x < 800; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(500);
  }
  delay(1000); 
}