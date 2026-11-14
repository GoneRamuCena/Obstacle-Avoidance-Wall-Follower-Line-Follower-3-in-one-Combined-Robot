#include <Servo.h>
int pos = 0;

Servo servo;

void setup()
{
  servo.attach(22);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop()
{

  int angles_list[] = {0,90,180,120,60,30};
  for (int angle : angles_list)
  {
    servo_rotation(angle);

  }
 
  
}

void servo_rotation(int angle)
{
  servo.write(angle);
  Serial.print("angle = ");
  Serial.println(angle);  
  delay(3000);
  
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(50);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(50); 
}
