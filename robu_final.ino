// ================= PIN DEFINITIONS =================

// Ultrasonic Sensors
#define TRIG_F 10
#define ECHO_F 11

#define TRIG_L 6
#define ECHO_L 7

#define TRIG_R 8
#define ECHO_R 12

// Motor Driver
#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 13

#define ENA 5    // Left motor PWM
#define ENB 9    // Right motor PWM

// ================= CONTROL PARAMETERS =================

int baseSpeed = 120;

float Kp = 2.5;
float Kd = 3.5;

float prevError = 0;

// ================= ULTRASONIC FUNCTION =================

long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 25000);
  if (duration == 0) return 200;

  return duration * 0.034 / 2;
}

// ================= MOTOR CONTROL =================

void moveMotors(int leftSpeed, int rightSpeed) {
  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);

  // Left Motor
  if (leftSpeed >= 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    leftSpeed = -leftSpeed;
  }

  // Right Motor
  if (rightSpeed >= 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    rightSpeed = -rightSpeed;
  }

  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
}

// ================= SETUP =================

void setup() {
  pinMode(TRIG_F, OUTPUT);
  pinMode(ECHO_F, INPUT);

  pinMode(TRIG_L, OUTPUT);
  pinMode(ECHO_L, INPUT);

  pinMode(TRIG_R, OUTPUT);
  pinMode(ECHO_R, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(9600);
}

// ================= MAIN LOOP =================

void loop() {
  long dF = getDistance(TRIG_F, ECHO_F);
  long dL = getDistance(TRIG_L, ECHO_L);
  long dR = getDistance(TRIG_R, ECHO_R);

  // ---------- DEAD END (ABSOLUTE STOP) ----------
  if (dF < 10 && dL < 10 && dR < 10) {
    moveMotors(0, 0);
    Serial.println("DEAD END - ROBOT STOPPED");

    while (true) {
      delay(100);   // lock forever
    }
  }

  // ---------- FRONT BLOCK ----------
  if (dF < 12) {
    if (dL > dR)
      moveMotors(-80, 80);   // turn LEFT
    else
      moveMotors(80, -80);   // turn RIGHT

    delay(150);
    return;
  }

  // ---------- PD WALL CENTERING ----------
  float error = dL - dR;

  float correction = -(Kp * error + Kd * (error - prevError));
  correction = constrain(correction, -40, 40);

  prevError = error;

  int leftSpeed  = baseSpeed + correction;
  int rightSpeed = baseSpeed - correction;

  moveMotors(leftSpeed, rightSpeed);

  // ---------- DEBUG ----------
  Serial.print("L:");
  Serial.print(dL);
  Serial.print(" F:");
  Serial.print(dF);
  Serial.print(" R:");
  Serial.print(dR);
  Serial.print(" Err:");
  Serial.println(error);

  delay(30);
}
