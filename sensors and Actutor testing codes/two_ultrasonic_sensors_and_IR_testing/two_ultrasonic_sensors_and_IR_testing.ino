const int side_trigpin = 30;
const int side_echopin = 31;

const int front_trigpin = 34;
const int front_echopin = 35;

int l_irpin = 50;
int r_irpin = 51;

float front_distance;
float side_distance;

void setup() 
{
  pinMode(front_trigpin, OUTPUT); // to send signal
  pinMode(front_echopin, INPUT);  // to recieve signal


  pinMode(side_trigpin, OUTPUT);
  pinMode(side_echopin, INPUT);  // put your setup code here, to run once:

  pinMode(LED_BUILTIN,OUTPUT);

  pinMode(l_irpin, INPUT);
  pinMode(r_irpin, INPUT);

  Serial.begin(9600);

}


void loop()
 {
  // put your main code here, to run repeatedly:

  front_distance = distance_measurement(front_trigpin,front_echopin,"Front Ultrasonic Sensor");

  // side_distance = distance_measurement(side_trigpin,side_echopin,"Side Ultrasonic Sensor");

  if((front_distance <= 10) )
  {
    digitalWrite(LED_BUILTIN, HIGH);

  }
  else
  {
    digitalWrite(LED_BUILTIN,LOW);

  }



}


float distance_measurement(int trigpin,int echopin,String sensor)
{
  
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);
  
  long duration = pulseIn(echopin,HIGH);
  float distance = (duration * .0343)/2;

  Serial.print(sensor);
  Serial.print(" Distance : ");
  Serial.println(distance);
  delay(150);
  return distance;
}


void ir_testing()
{
  int l_irvalue = digitalRead(l_irpin);
  int r_irvalue = digitalRead(r_irpin);

  if (l_irvalue == LOW && r_irvalue == LOW)
  {
    Serial.println("objected Detected");
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else if  (l_irvalue == HIGH && r_irvalue == HIGH)
  {
    Serial.println("objected Detected");
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    Serial.println("No object");
    digitalWrite(LED_BUILTIN, LOW);
  }
}
