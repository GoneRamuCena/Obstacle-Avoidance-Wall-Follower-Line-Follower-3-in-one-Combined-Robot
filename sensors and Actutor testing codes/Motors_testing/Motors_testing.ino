

// Motor driver pins
const int ENA = 8;        // Right motor enable
const int IN1 = 9;        // Right motor IN1
const int IN2 = 10;        // Right motor IN2
const int IN3 = 11;        // Left motor IN1
const int IN4 = 12;        // Left motor IN2
const int ENB = 13;       // Left motor enable


void setup() {
  // Motor control pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);



  // Start Serial
  Serial.begin(9600);
}


void moveBackward() 
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);


  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 100);  // speed: 0-255
  analogWrite(ENB, 100);

}

void moveForward() 
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);


  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, 100);
  analogWrite(ENB, 100);

}

void turnRight()
 {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);  
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 100);
  analogWrite(ENB, 100);
}

void turnLeft() 
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, 100);
  analogWrite(ENB, 100);
}

void stopMoving() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}



void loop()
{
  moveForward();
  Serial.println("Forward");
  delay(5000);

  moveBackward();
  Serial.println("Backward");
  delay(5000);

  turnRight();
  Serial.println("Right");
  delay(5000);

  turnLeft();
  Serial.println("Left");
  delay(5000);


}
