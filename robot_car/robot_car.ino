#include <AFMotor.h>
#include <NewPing.h>
#include <Servo.h>

#define TURN_ON_BUTTON_PIN A5
#define TURN_ON_LED_PIN A4

#define TRIG_PIN A0
#define ECHO_PIN A1
#define MAX_DISTANCE 220
#define MAX_SPEED 50 // sets speed of DC  motors
#define MAX_SPEED_OFFSET 20

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);
Servo myservo;

boolean goesForward = false;
int distance = 100;
int speedSet = 0;
int pos = 0; // variable to store the servo position

int lookCenterPosition = 90;
int lookLeftPosition = lookCenterPosition + 45;
int lookRightPosition = lookCenterPosition - 45;;
int servoStep = 2;

bool isTurnedOn = false;

void setup() {
  pinMode(TURN_ON_BUTTON_PIN, INPUT);
  pinMode(TURN_ON_LED_PIN, OUTPUT);

  Serial.begin(9600);
  myservo.attach(10);
  lookAround();  

  goToPosition(lookCenterPosition);
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
}

void lookAround(){
    lookRight();
    delay(100);
    lookLeft();  
    delay(100);
}

void loop() {
    int buttonState  = digitalRead(TURN_ON_BUTTON_PIN);
    Serial.println(buttonState);
    if(buttonState == HIGH){
      isTurnedOn = !isTurnedOn;
      digitalWrite(TURN_ON_LED_PIN, isTurnedOn ? HIGH : LOW);
    }

    if(isTurnedOn){
      digitalWrite(TURN_ON_LED_PIN, HIGH);
      delay(1000);
      lookAround();  
    
    
    int distanceR = 0;
    int distanceL =  0;
    delay(40);
  
    if (distance <= 20)
    {
      moveStop();
      delay(100);
      moveBackward();
      delay(300);
      moveStop();
      delay(200);
      distanceR = lookRight();
      delay(200);
      distanceL = lookLeft();
      delay(200);
  
      if (distanceR >= distanceL)
      {
        turnRight();
        moveStop();
      } else
      {
        turnLeft();
        moveStop();
      }
    } else
    {
      moveForward();
    }
    distance = readPing();
  }
}

void goToPosition(int desiredPosition){
    int currentPosition = myservo.read();

    if(currentPosition > desiredPosition){
        for (pos = currentPosition; pos >= desiredPosition; pos -= servoStep)
            {
                myservo.write(pos);
                delay(15);
            }
    } else {
    for (pos =currentPosition; pos <= desiredPosition; pos += servoStep)
        {
            myservo.write(pos);
            delay(15);
        }
    }
}

int lookRight()
{
    goToPosition(lookRightPosition);
    delay(50);
    int distance = readPing();
    delay(50);

    goToPosition(lookCenterPosition);

    return distance;
}


int lookLeft()
{
    goToPosition(lookLeftPosition);

    delay(50);
    int distance = readPing();
    delay(50);

    goToPosition(lookCenterPosition);

    return distance;
}

int readPing() {
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0)
  {
    cm = 250;
  }
  Serial.println(cm);
  return cm;
}

void moveStop() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void moveForward() {

  if (!goesForward)
  {
    goesForward = true;
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    motor3.run(FORWARD);
    motor4.run(FORWARD);
    for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 1) // slowly bring the speed up to avoid loading down the batteries too quickly
    {
      motor1.setSpeed(speedSet);
      motor2.setSpeed(speedSet);
      motor3.setSpeed(speedSet);
      motor4.setSpeed(speedSet);
      delay(5);
    }
  }
}

void moveBackward() {
  goesForward = false;
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 1) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
  }
}

void turnRight() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  delay(500);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void turnLeft() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  delay(500);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}
