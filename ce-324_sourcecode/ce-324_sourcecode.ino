#define IR_SENSOR_RIGHT 2
#define IR_SENSOR_LEFT 3
#define TRIG_PIN 11
#define ECHO_PIN 12
#define MOTOR_SPEED 220
#define OBSTACLE_THRESHOLD 20

// Motor control pins
int enableRightMotor = 10;
int rightMotorPin1 = 6;
int rightMotorPin2 = 7;
int enableLeftMotor = 5;
int leftMotorPin1 = 8;
int leftMotorPin2 = 9;

void setup()
{
  TCCR0B = TCCR0B & B11111000 | B00000010; // Set frequency as 7812.5 Hz

  pinMode(enableRightMotor, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  pinMode(enableLeftMotor, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);

  pinMode(IR_SENSOR_RIGHT, INPUT);
  pinMode(IR_SENSOR_LEFT, INPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  rotateMotor(0, 0);
}

void loop()
{
  int rightIRSensorValue = digitalRead(IR_SENSOR_RIGHT);
  int leftIRSensorValue = digitalRead(IR_SENSOR_LEFT);

  // Line following logic
  if (rightIRSensorValue == LOW && leftIRSensorValue == LOW)
  {
    rotateMotor(-MOTOR_SPEED, -MOTOR_SPEED);
  }
  else if (rightIRSensorValue == HIGH && leftIRSensorValue == LOW)
  {
    rotateMotor(-MOTOR_SPEED, MOTOR_SPEED); // Turn right
  }
  else if (rightIRSensorValue == LOW && leftIRSensorValue == HIGH)
  {
    rotateMotor(MOTOR_SPEED, -MOTOR_SPEED); // Turn left
  }
  else if (rightIRSensorValue == HIGH && leftIRSensorValue == HIGH)
  {
    rotateMotor(0, 0);
  }
  else
  {
    rotateMotor(0, 0);
  }

  // Obstacle avoidance logic
  if (checkObstacle())
  {
    // Stop and avoid obstacle
    rotateMotor(0, 0);
    delay(500);
    rotateMotor(-MOTOR_SPEED, MOTOR_SPEED); // Turn right
    delay(1000);
  }
}

void rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{
  if (rightMotorSpeed < 0)
  {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);
  }
  else if (rightMotorSpeed > 0)
  {
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
  }
  else
  {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, LOW);
  }

  if (leftMotorSpeed < 0)
  {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);
  }
  else if (leftMotorSpeed > 0)
  {
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
  }
  else
  {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, LOW);
  }

  analogWrite(enableRightMotor, abs(rightMotorSpeed));
  analogWrite(enableLeftMotor, abs(leftMotorSpeed));
}

bool checkObstacle()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2;

  return (distance < OBSTACLE_THRESHOLD);
}
