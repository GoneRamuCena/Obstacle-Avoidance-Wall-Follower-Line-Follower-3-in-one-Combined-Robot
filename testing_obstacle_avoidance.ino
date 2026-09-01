#include <Servo.h>

// Motor driver pins
const int ENA = 13;        // Right motor enable
const int IN1 = 12;        // Right motor IN1
const int IN2 = 11;        // Right motor IN2
const int IN3 = 10;        // Left motor IN1
const int IN4 = 9;        // Left motor IN2
const int ENB = 8;       // Left motor enable

// Ultrasonic Sensor
const int trigPin = 34;
const int echoPin = 35;

// Servo motor
const int servoPin = 22;
Servo myServo;

long duration;
int distance;


unsigned long straightStartTime = 0;
bool movingStraight = false;
const unsigned long STRAIGHT_DURATION = 300000; // 3 minutes

void setup() {
  // Motor control pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Attach servo
  myServo.attach(servoPin);

  // Start Serial
  Serial.begin(9600);
}

int readDistance() 
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  return distance;
}

void moveForward() 
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 100);  // speed: 0-255
  analogWrite(ENB, 100);

}

void moveBackward() 
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);

}

void turnLeft()
 {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);  
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
}

void turnRight() 
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
}

void stopMoving() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void loop() {
  myServo.write(90); // look forward
  //delay(500);
  int frontDist = readDistance();

  if (frontDist < 45)
  {
    stopMoving();
    Serial.println("distance < 30cm");
    delay(300);

    // Check left
    myServo.write(150);
    delay(500);
    int leftDist = readDistance();

    // Check right
    myServo.write(30);
    delay(500);
    int rightDist = readDistance();

    myServo.write(90);
    delay(200);

    if (leftDist > 30) 
    {
      turnLeft();
      Serial.println("distance < 30cm and choosen left");
      delay(400);
    } 
    
    else if (rightDist > 30) 
    {
      turnRight();
      Serial.println("distance < 30cm and choosen right");
      delay(400);
    } 
    
    else if (leftDist < 30 && rightDist < 30)
    {
      moveBackward();
      Serial.println("all side block going back");
      delay(500);
    }

    movingStraight = false;
  } 
  
  else 
  {
    moveForward();
    // delay(500);
    Serial.println("going forward");
    if (!movingStraight) 
    {
      // Started moving straight
      straightStartTime = millis();
      movingStraight = true;
    } 
    else 
    {
      // Already moving straight, check if 3 mins passed
      if (millis() - straightStartTime >= STRAIGHT_DURATION)
      {
        stopMoving();
        Serial.println("Stopped after 3 minutes of straight movement");
        while (1); // Stop forever
      }
    }
    delay(100);
  }
}
