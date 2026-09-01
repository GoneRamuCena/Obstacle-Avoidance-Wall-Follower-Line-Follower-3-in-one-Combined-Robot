#include <Servo.h>

// Motor driver pins
const int ENA = 13;             // left motor enable
const int IN1 = 12;             // left motor IN1
const int IN2 = 11;             // left motor IN2
const int IN3 = 10;             // right motor IN1
const int IN4 = 9;              // right motor IN2
const int ENB = 8;              // right motor enable

// IR sensor pin defination
const int r_irpin = 50;         // right side IR pin
const int l_irpin = 51;         // left side IR pin


// Ultrasonic Sensors Pin definitions
const int trigPinSide = 30;     // side Ultrasonic sensor trigger
const int echoPinSide = 31;     // side Ultrasonic sensor echo
const int trigPinFront = 34;    // front Ultrasonic sensor trigger
const int echoPinFront = 35;    // front Ultrasonic sensor echo


// Servo motor
const int servoPin = 22;
Servo myServo;

// Switch pins 
const int SW = 52;

// Parameters For wall follower
const float TARGET_DISTANCE = 15.0;     // Target distance from wall in cm
const float DISTANCE_TOLERANCE = 3.0;   // Tolerance range (±cm)
const float TOO_CLOSE = 5.0;            // Dangerously close distance (cm)
const float MAX_DISTANCE = 30.0;        // Max distance to still consider wall present
const float NO_WALL = 50.0;             // No wall detected threshold (cm)
const int LEFT_BASE_SPEED = 110;             // Base motor speed (0-255)
const int RIGHT_BASE_SPEED = 110;             // Base motor speed (0-255)
const int TURN_SPEED = 20;              // Speed difference for turning

const float FRONT_DISTANCE_LIMIT=5.0;


// Parameters For Obstacle Avoidance
// long duration;
// int distance;

unsigned long straightStartTime = 0;
bool movingStraight = false;
const unsigned long STRAIGHT_DURATION = 300000; // 3 minutes


float front_ultrasonic_distance;
float side_ultrasonic_distance;

void setup() 
  {
  // Motor control pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // IR sensor pins
  pinMode(r_irpin, INPUT);
  pinMode(l_irpin, INPUT);

  // Ultrasonic sensor pins
  pinMode(trigPinSide, OUTPUT);
  pinMode(echoPinSide, INPUT);
  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);

  // Attach servo
  myServo.attach(servoPin);

  // Switch pins using pull up mecanism
  pinMode(SW, INPUT_PULLUP);

  // delaying for 3 seconds
  // wall_stopMotors();
  // delay(3000);


  // Start Serial
  Serial.begin(9600);
  }


// Motor control functions for line follower.
void line_goForward() 
  {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); analogWrite(ENA,105);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); analogWrite(ENB,105);
  }

void  line_turn_right() 
  {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); analogWrite(ENA,105);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); analogWrite(ENB,0);
  }

void line_turn_left() 
  {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); analogWrite(ENA,0);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); analogWrite(ENB,105);
  }


void line_stop_motors() 
  {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW); 
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW); 
  }

void line_turn_left_quick() 
  {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); analogWrite(ENA,0);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); analogWrite(ENB,110);
  }


// Motor control functions for Wall follower.

void wall_goForward()
  {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, LEFT_BASE_SPEED );
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, RIGHT_BASE_SPEED );
  }

void wall_turnRight() 
  {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, LEFT_BASE_SPEED + TURN_SPEED);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, RIGHT_BASE_SPEED - TURN_SPEED);
  }

void wall_turnLeft()  
  {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, LEFT_BASE_SPEED - TURN_SPEED);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, RIGHT_BASE_SPEED + TURN_SPEED);
  }

void wall_turnRightStronger() 
  {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, LEFT_BASE_SPEED + (2*TURN_SPEED ) );
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); analogWrite(ENB, RIGHT_BASE_SPEED - (2*TURN_SPEED ));
  }

void wall_turnLeftStronger() 
  {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH) ; analogWrite(ENA, LEFT_BASE_SPEED - (2*TURN_SPEED ));
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, RIGHT_BASE_SPEED + (2*TURN_SPEED ) );
  }


void wall_stopMotors() 
  {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW); analogWrite(ENA, 0);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW); analogWrite(ENB, 0);
  }




// Motor control functions for obstacle avoidance
  void obstacle_moveForward() 
  {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 140);  // left tire speed  // speed: 0-255
  analogWrite(ENB, 140);  // right tire speed
  }

void obstacle_moveBackward() 
  {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 210);
  analogWrite(ENB, 210);
  }

void obstacle_turnLeft()
 {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);  
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 210);
  analogWrite(ENB, 210);
  }

void obstacle_turnRight() 
  {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 210);
  analogWrite(ENB, 210);
  }

void obstacle_stopMoving() 
  {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  }



//**********************************************
// Line follower logic code
void line_follower()
  {

  int r_irvalue = digitalRead(r_irpin);
  int l_irvalue = digitalRead(l_irpin);


  if ( (l_irvalue == HIGH) && (r_irvalue == HIGH) )
    {
    Serial.println("objected Detected by both sensors, going forward");
    // delay(100);
    line_goForward();
    }

  else if ( (l_irvalue == LOW) && (r_irvalue == HIGH) )
    {
    Serial.println("Turn right");
    line_turn_right(); 
    delay(100);
    }

  else if ( (l_irvalue == HIGH) && (r_irvalue == LOW) )
    {
    Serial.println("Turn left");
    line_turn_left() ;
    delay(100);
    }

  else if ( (l_irvalue == LOW) && (r_irvalue == LOW) )
    {  
    line_turn_left_quick() ;
    delay(2000);

    line_goForward();
    delay(2000);

    Serial.println("No objected detected stop");
    line_stop_motors(); 
    }
  }

//*********************************************
// wall follower functions 

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


// wall follower functions code logic 
// distance is replaced by side distance in wall follower in this code.
void wall_follower() 
  {
  // Check if 2 minutes have passed
  // wall_stopMotors();
  // delay(3000);
  
  // float front_distance = measureDistance(trigPinFront, echoPinFront);
  // float side_distance = measureDistance(trigPinSide, echoPinSide);


  float front_distance = front_ultrasonic_distance;
  float side_distance = side_ultrasonic_distance;
  
  Serial.println("front distance = " + String(front_distance));
  Serial.println("side distance = " + String(side_distance));
  
  if (front_distance > (TARGET_DISTANCE + 25)) //TARGET_DISTANCE = 15.0
    {
    if (side_distance <= TOO_CLOSE ) //TOO_CLOSE = 5.0
      {
      wall_turnLeft();
      Serial.println("Action: STRONG LEFT - Too close to right wall!");
      // delay(100);
      }
    else if (side_distance < (TARGET_DISTANCE - DISTANCE_TOLERANCE)) // TARGET_DISTANCE = 15.0, DISTANCE_TOLERANCE = 3.0
      {
      wall_turnLeft();
      Serial.println("Action: Turn left");
      // delay(100);
      }

    else if ( (side_distance > MAX_DISTANCE) && (side_distance < NO_WALL)) // MAX_DISTANCE = 30.0, NO_WALL = 50.0
      {
      wall_turnRight();
      Serial.println("Action: Turn Right - right Wall is too far!");
      // delay(100);
      }
    else if ((side_distance >= NO_WALL) ) // NO_WALL = 50.0
      {
      wall_turnRight();
      Serial.println("Action: SEARCHING - No wall detected!");
      // delay(100);
      }
    else 
      {
      // goForward(LEFT_BASE_SPEED,RIGHT_BASE_SPEED);
      wall_goForward();
      Serial.println("Action: Forward");
      }
    }
  else if (front_distance <= (TARGET_DISTANCE + 25) ) //TARGET_DISTANCE = 15.0 + 5 = 20 
    { 
    wall_stopMotors();
    // delay(100);
    Serial.println("Action: Stop motors for 500 milli seconds");
    if (side_distance > MAX_DISTANCE + 20   ) //MAX_DISTANCE = 30.0
      {
      wall_turnRightStronger(); 
      Serial.println("Action: Turn Rightstrong . front wall: close. Side wall: No wall.  ");
      delay(550);
      }
    else if (side_distance <= MAX_DISTANCE + 20   ) //MAX_DISTANCE = 30.0
      {
      wall_turnLeftStronger(); 
      Serial.println("Action: Turn Leftstrong . front wall: close. Side wall: present.  ");
      delay(550);
      }
      
    }
  delay(50);
  }


//*********************************************
// Obstacle Avoidance functions 

void obstacle_avoidance() 
  {
  myServo.write(90); // look forward
  delay(120);

  // float frontDist = measureDistance(trigPinFront, echoPinFront);

  float frontDist = front_ultrasonic_distance;
  if (frontDist < 38)
    {
    Serial.println("***************** front Distance = " + String(frontDist));
    obstacle_stopMoving();
    // Serial.println("distance < 30cm");
    delay(200);

    // Check left
    myServo.write(150);
    delay(400);
    int leftDist = measureDistance(trigPinFront, echoPinFront);
    Serial.println("left Distance = " + String(leftDist));

    // Check right
    myServo.write(30);
    delay(400);
    int rightDist = measureDistance(trigPinFront, echoPinFront);
    Serial.println("Right Distance = " + String(rightDist));

    myServo.write(90);
    delay(400);

    if (leftDist > 30) 
      {
      obstacle_turnLeft();
      // Serial.println("distance < 30cm and choosen left");
      delay(500);
      } 
    
    else if (rightDist > 30) 
      {
      obstacle_turnRight();
      // Serial.println("distance < 30cm and choosen right");
      delay(500);
      } 
    
    else if (leftDist < 30 && rightDist < 30)
      {
      obstacle_moveBackward();
      // Serial.println("all side block going back");
      delay(500);
      }

    movingStraight = false;
    } 
  
  else 
    {
    obstacle_moveForward();

    // Serial.println("going forward");
    if (!movingStraight) 
      {
      // Started moving straight
      straightStartTime = millis();
      movingStraight = true;
      } 
    else if (millis() - straightStartTime >= STRAIGHT_DURATION)
      {
      // Already moving straight, check if 3 mins passed
        
      obstacle_stopMoving();
      // Serial.println("Stopped after 3 minutes of straight movement");
      while (1); // Stop forever
      }
    // delay(100);
    }
  }



void loop() 
  {
  // readSensors();

  front_ultrasonic_distance = measureDistance(trigPinFront, echoPinFront);
  side_ultrasonic_distance = measureDistance(trigPinSide, echoPinSide);

  // int r_irvalue = digitalRead(r_irpin);
  // int l_irvalue = digitalRead(l_irpin);

  // Serial.print("right ir value = ");
  // Serial.println(r_irvalue);

  // Serial.print("left ir value = ");
  // Serial.println(l_irvalue);

  // Serial.print("front ultrasonic distance = ");
  // Serial.println(front_ultrasonic_distance);
 
  // Serial.print("side ultrasonic distance = ");
  // Serial.println(side_ultrasonic_distance);

  Serial.println("front ultrasonic distance = " + String(front_ultrasonic_distance));
  Serial.println("side ultrasonic distance = " + String(side_ultrasonic_distance));
  //****************************************** 
  // if(front_ultrasonic_distance <= 2) 
  // {
  //   digitalWrite(LED_BUILTIN, HIGH);

  // }
  // else if (front_ultrasonic_distance > 2 && front_ultrasonic_distance <= 5 )
  // {
  //   digitalWrite(LED_BUILTIN,HIGH);

  // }
  // else
  // {
  //   digitalWrite(LED_BUILTIN,LOW);

  // }
  //**************************************
    // if (digitalRead(SW) == LOW)   // switch is pressed on
    //   {
    //     Serial.println("swtich pressed");
    //     digitalWrite(LED_BUILTIN, HIGH);

    //   }
    // else
    //   {
    //     Serial.println("swtich is off");
    //     digitalWrite(LED_BUILTIN, LOW);
    //   }
    // delay(200);
  //**************************************
  // if (front_ultrasonic_distance <= 2) 
  //   {
  //     // line_follower();
  //       digitalWrite(LED_BUILTIN, HIGH);
  //       Serial.println("line follower is on, two ultrasonic sensors off");
  //   } 
  // else
  //   {
  //     if (digitalRead(SW) == LOW)   // switch is pressed on
  //       {
  //         Serial.println("wall follower is on, two ultrasonic sensors on and swtich pressed");
  //         digitalWrite(LED_BUILTIN,LOW);
  //       }
  //     else
  //       {
  //         Serial.println("Obstacle avoidance is on, two ultrasonic sensors on and swtich is off");
  //         digitalWrite(LED_BUILTIN, HIGH);
  //       }
  //   } 

  // ********************************************************
  if (front_ultrasonic_distance <= 2) 
    {
      line_follower();
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("line follower is on, two ultrasonic sensors off");
    } 
  else
    {
      if (digitalRead(SW) == LOW)   // switch is pressed on
        {
          wall_follower();
          Serial.println("wall follower is on, two ultrasonic sensors on and swtich pressed");
          digitalWrite(LED_BUILTIN,LOW);
        }
      else
        { 
          obstacle_avoidance();
          Serial.println("Obstacle avoidance is on, two ultrasonic sensors on and swtich is off");
          digitalWrite(LED_BUILTIN, HIGH);
        }
    } 
  // ********************************************************
  // line_follower();
  // wall_follower();
  // obstacle_avoidance();
  // line_goForward();
  


  }







