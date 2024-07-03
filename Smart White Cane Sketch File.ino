#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// Defines pins numbers
const int trigPin = 9;
const int echoPin = 10;
const int buzzer = 11;

// Defines variables
long duration;
int distance;
int safetyDistance;
bool obstacleDetected = false;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);

  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  }
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance = duration * 0.034 / 2;

  safetyDistance = distance;
  if (safetyDistance <= 30) {
    obstacleDetected = true;
  } else {
    obstacleDetected = false;
  }

  // Read MPU6050 data
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Detect significant left or right movement using gyroscope's Y-axis data
  int movementThreshold = 15000;
  if (abs(gy) > movementThreshold) {
    for (int i = 0; i < 5; i++) {
      // Make a triple buzz sound when clear
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
      delay(100);
    }
  }

  // Handle ultrasonic sensor detection
  if (obstacleDetected) {
    // Make a siren sound when obstacle is detected
    for (int i = 0; i < 3; i++) {
      tone(buzzer, 1000, 200);
      delay(200);
      tone(buzzer, 1500, 200);
      delay(200);
    }
  } else {
    noTone(buzzer);
  }

  delay(70);
}
