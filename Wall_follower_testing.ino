// Wall Following Robot
// Uses ultrasonic sensor to maintain constant distance from right wall
// Stops automatically after 2 minutes

// Pin definitions
const int trigPinSide = 30;    // Ultrasonic sensor trigger
const int echoPinSide = 31;    // Ultrasonic sensor echo
const int trigPinFront = 34;    // Ultrasonic sensor trigger
const int echoPinFront = 35;    // Ultrasonic sensor echo

// Motor driver pins
const int ENA = 13;        // left motor enable
const int IN1 = 12;        // left motor IN1
const int IN2 = 11;        // left motor IN2
const int IN3 = 10;        // right motor IN1
const int IN4 = 9;        // right motor IN2
const int ENB = 8;       // right motor enable

// Parameters
const float TARGET_DISTANCE = 15.0;     // Target distance from wall in cm
const float DISTANCE_TOLERANCE = 3.0;   // Tolerance range (±cm)
const float TOO_CLOSE = 5.0;            // Dangerously close distance (cm)
const float MAX_DISTANCE = 30.0;        // Max distance to still consider wall present
const float NO_WALL = 50.0;             // No wall detected threshold (cm)
const int LEFT_BASE_SPEED = 160;             // Base motor speed (0-255)
const int RIGHT_BASE_SPEED = 170;             // Base motor speed (0-255)
const int TURN_SPEED = 30;              // Speed difference for turning

const float FRONT_DISTANCE_LIMIT=5.0;

void setup() {
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(trigPinSide, OUTPUT);
  pinMode(echoPinSide, INPUT);
  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);

  stopMotors();
  delay(3000);
  
}

void loop() {
  // Check if 2 minutes have passed
  float front_distance = measureDistance(trigPinFront, echoPinFront);
  float side_distance = measureDistance(trigPinSide, echoPinSide);

  Serial.print("front_Distance: ");
  Serial.print(front_distance);
  Serial.println(" cm ");

  Serial.print("side_Distance: ");
  Serial.print(side_distance);
  Serial.println(" cm ");
  
  if(front_distance <= FRONT_DISTANCE_LIMIT) // FRONT_DISTANCE_LIMIT = 5.0
  {
    stopMotors();
    Serial.println("Action: stop - Too close to front wall!");
    delay(2000);
  }
  
  else if (side_distance <= TOO_CLOSE) // TOO_CLOSE = 5.0
  {
    turnLeftStronger();
    Serial.println("Action: STRONG LEFT - Too close to right wall!");
    delay(500);
  }
  
  else if (side_distance < TARGET_DISTANCE - DISTANCE_TOLERANCE) // TARGET_DISTANCE = 15.0, DISTANCE_TOLERANCE = 3.0
  {
    turnLeft();
    Serial.println("Action: Turn left");
    delay(500);
  }
  
  else if ( (side_distance > MAX_DISTANCE) && (side_distance < NO_WALL) ) // MAX_DISTANCE = 30.0, NO_WALL = 50.0
  {
    turnRightStronger();
    Serial.println("Action: STRONG RIGHT - right Wall is too far!");
    delay(500);
  }
  
  else if (side_distance >= NO_WALL) // NO_WALL = 50.0
  {
    turnRightStronger();
    Serial.println("Action: SEARCHING - No wall detected!");
    delay(500);
  }
  
  else if (side_distance > TARGET_DISTANCE + DISTANCE_TOLERANCE) // TARGET_DISTANCE = 15.0, DISTANCE_TOLERANCE = 3.0
  {
    turnRight();
    Serial.println("Action: Turn right");
    delay(500);
  }
  
  else {
    goForward();
    Serial.println("Action: Forward");
    delay(500);
  }

  delay(500);
}

// Measure distance using ultrasonic sensor
float measureDistance(int trigPin, int echoPin) 
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float dist = (duration * 0.0343) / 2;

  if (dist > 400) dist = 400;

  return dist;
}
 
// Motor control functions
void goForward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, LEFT_BASE_SPEED);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, RIGHT_BASE_SPEED);
}

void turnRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, LEFT_BASE_SPEED);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, RIGHT_BASE_SPEED - TURN_SPEED);
}

void turnLeft() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, LEFT_BASE_SPEED - TURN_SPEED);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, RIGHT_BASE_SPEED);
}

void turnRightStronger() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, LEFT_BASE_SPEED);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, RIGHT_BASE_SPEED - (TURN_SPEED * 2));
}

void turnLeftStronger() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, LEFT_BASE_SPEED - (TURN_SPEED * 2));
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, RIGHT_BASE_SPEED);
}


void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
