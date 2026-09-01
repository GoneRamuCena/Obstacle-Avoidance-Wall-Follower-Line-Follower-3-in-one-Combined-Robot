
const int r_irpin = 50;
const int l_irpin = 51;

const int ena = 13;        // Right motor enable
const int in1 = 12;        // Right motor IN1
const int in2 = 11;        // Right motor IN2
const int in3 = 10;        // Left motor IN1
const int in4 = 9;        // Left motor IN2
const int enb = 8;       // Left motor enable


void setup()

{
  pinMode(r_irpin, INPUT);
  pinMode(l_irpin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  pinMode(ena, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enb, OUTPUT);

}


void loop()
{

  int r_irvalue = digitalRead(r_irpin);
  int l_irvalue = digitalRead(l_irpin);


  if ( (l_irvalue == HIGH) && (r_irvalue == HIGH) )
  {
    Serial.println("objected Detected by both sensors, going forward");
    goForward();
    delay(100); 
  }

  else if ( (l_irvalue == LOW) && (r_irvalue == HIGH) )
  {
    Serial.println("Turn right");
    stop_motors();
    turn_right(); 
    delay(100);
  }

  else if ( (l_irvalue == HIGH) && (r_irvalue == LOW) )
  {
    Serial.println("Turn left");
    stop_motors();
    turn_left() ;
    delay(100);
  }

  else if ( (l_irvalue == LOW) && (r_irvalue == LOW) )
  {  
    turn_left_quick() ;
    delay(2000);

    goForward();
    delay(2000);

    Serial.println("No objected detected stop");
    stop_motors(); 
  }


}


// Motor control functions 

void goForward() 
{
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH); analogWrite(ena,95);
  digitalWrite(in3, LOW); digitalWrite(in4, HIGH); analogWrite(enb,95);
}

void turn_right() 
{
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH); analogWrite(ena,0);
  digitalWrite(in3, LOW); digitalWrite(in4, HIGH); analogWrite(enb,95);

}

void turn_left() 
{
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH); analogWrite(ena,95);
  digitalWrite(in3, LOW); digitalWrite(in4, HIGH); analogWrite(enb,0);

}


void stop_motors() 
{
  digitalWrite(in1, LOW); digitalWrite(in2, LOW); 
  digitalWrite(in3, LOW); digitalWrite(in4, LOW); 
}

void turn_left_quick() 
{
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH); analogWrite(ena,0);
  digitalWrite(in3, LOW); digitalWrite(in4, HIGH); analogWrite(enb,80);

}
