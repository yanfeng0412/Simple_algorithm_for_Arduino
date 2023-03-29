const int SW_pin = 2 ;
const int X_pin  = 0; 
const int Y_pin =  1;

void setup() {
  pinMode(SW_pin,INPUT) ; // put your setup code here, to run once:
  digitalWrite(SW_pin,HIGH);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Switch:");
  Serial.print(digitalRead(SW_pin));
  // Serial.print("\n");

  Serial.print("  X- Axis :");
  Serial.print(analogRead(X_pin));
  // Serial.print("\n");

  Serial.print("  Y- Axis :");
  Serial.print(analogRead(X_pin));
  Serial.print("\n");
  delay(500);



}
