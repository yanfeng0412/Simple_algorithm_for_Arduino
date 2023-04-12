// defines pins numbers

const int stepPin = 5; //PUL+ 每个输入的脉冲信号，转动一个步距角
const int dirPin  = 2; //DIR+ 方向控制，高电平顺时，低电平逆时
const int enPin  = 8;  //ENA + 使能端，低电平使能，高电平失能，调试/故障排除使用

// Button
const int b1 = A0;
const int b2 = A1;
const int b3 = A2;
const int b4 = A3;



int currentAngle = 0;
int angle = 0;
float stepPerAngle = 1.8; // full step = 1.8
int   numstep;



void setup() {

  Serial.begin(9600);

  // Sets the two pins as Outputs

  pinMode(stepPin,OUTPUT);
  pinMode(dirPin,OUTPUT);
  pinMode(enPin,OUTPUT);

  digitalWrite(enPin,LOW);
  digitalWrite(dirPin,HIGH);

  pinMode(b1, INPUT);
  pinMode(b2, INPUT);
  pinMode(b3, INPUT);

  pinMode(b4, INPUT);
}

void loop() {
    int n;
    if (Serial.available() > 0) {
        char command = Serial.read();
        if (command == '0') {
            angle = 0;
        } else if (command == '1') {
            angle = 45;
        } else if (command == '2') {
            angle = 90;
        } else if (command == '3') {
            angle = 135;
        }else if (command == '4') {
            angle = 180;
        } else if (command == '5') {
            angle = 225;
        } else if (command == '6') {
            angle = 270;
        }else if (command == '7') {
            angle = 315;
        } else if (command == '8') {
            angle = 360;
        } 
    }

    if( currentAngle != angle ){ //如果不相等==>执行步进电机旋转到目标角度
        if( currentAngle < angle){ //当前角度<目标角度，步进电机顺时针旋转
            digitalWrite(dirPin,HIGH);
             n = angle - currentAngle;  //计算点解所需旋转的角度偏差==>目标角度减去当前角度
            numstep = n / stepPerAngle; //步距角/旋转角度，计算电机所需要的旋转步数
        }
        else if( currentAngle > angle){ //当前角度>目标角度，步进电机逆时针旋转
            digitalWrite(dirPin,LOW);  
            n = currentAngle - angle; //计算角度偏差
            if( angle == 0){ //目标角度 = 0，偏差=0
               n =currentAngle;
            }
            numstep = n / stepPerAngle;
        }
        for(int x = 0; x < numstep; x++) { //通过输入，并根据上面计算的步数
           digitalWrite(stepPin,HIGH);
           delayMicroseconds(1000);
           digitalWrite(stepPin,LOW);
           delayMicroseconds(1000);
        }
        currentAngle = angle; //刷新目标角度，判断下次循环是否需要旋转
    }
   delay(500);
}