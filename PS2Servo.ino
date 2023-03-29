#include <PS2X_lib.h> // 导入 PS2 控制器库
#include <Servo.h>
Servo Servo1;


// 定义 PS2 控制器引脚
#define PS2_DAT        13  //14    
#define PS2_CMD        11  //15
#define PS2_SEL        10  //16 CS
#define PS2_CLK        12  //17
#define pressures   true
#define rumble      true

PS2X ps2x; // create PS2 Controller Class
int error = 0;
byte type = 0;
byte vibrate = 0;
void (* resetFunc) (void) = 0;

// 定义机械臂舵机的 PWM 输出引脚
#define SERVO_PIN      6

// 定义舵机的最大和最小角度值
#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 180

void setup() {
  Serial.begin(9600); // 初始化串口通信
  Servo1.attach(6);
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  
  if(error == 0){
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
	if (pressures)
	  Serial.println("true ");
	else
	  Serial.println("false");
	Serial.print("rumble = ");
	if (rumble)
	  Serial.println("true)");
	else
	  Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }  
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
  type = ps2x.readType(); 
  switch(type) {
    case 0:
      Serial.println("Unknown Controller type found ");
      break;
    case 1:
      Serial.println("DualShock Controller found ");
      break;
    case 2:
      Serial.println("GuitarHero Controller found ");
      break;
	case 3:
      Serial.println("Wireless Sony DualShock Controller found ");
      break;
   }
  
}

void loop() {
  ps2x.read_gamepad(); // 读取 PS2 控制器摇杆数据
  
  // 获取右侧摇杆的 Y 轴数值
  if(ps2x.Button(PSB_L2) || ps2x.Button(PSB_R2)) { //print stick values if either is TRUE
      int y = ps2x.Analog(PSS_LY);
      Serial.print("Stick Values:");
      Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_LX), DEC); 
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_RY), DEC); 
      Serial.print(",");
      Serial.println(ps2x.Analog(PSS_RX), DEC);
      // 将 Y 轴数值映射到机械臂需要的角度值
    int angle = map(y, 0, 255, 0, 180);
    Servo1.write(angle);

    // delay(15); 
  } 
   
 
}