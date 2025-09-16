// Define the pins for Trigger and Echo for all Ultrasonic Sensors
const int US_l_trig = 25;   // Left ultrasonic trigger pin
const int US_l_echo = 24;   // Left ultrasonic echo pin
const int US_r_trig = 51;   // Right ultrasonic trigger pin
const int US_r_echo = 50;   // Right ultrasonic echo pin
const int US_f_trig = 38;   // Front ultrasonic trigger pin
const int US_f_echo = 39;   // Front ultrasonic echo pin

// Motor setup
#define PWM1 5
#define AIN1 7
#define AIN2 8
#define EN1 A0

#define PWM2 6
#define BIN1 4
#define BIN2 9
#define EN2 A1

// Pins defined for IR sensors
const int IR_l = 46; // Left IR sensor
const int IR_c = 48; // Center IR sensor
const int IR_r = 47; // Right IR sensor

// Color Sensor Pins (TCS3200)
const int S0 = 41;
const int S1 = 40;
const int S2 = 44;
const int S3 = 42;
const int OUT = 43;  // Output pin for the color sensor

// Blue detection range (calibrated values)
const int blueMin = 400;
const int blueMax = 600;

// LED direction indicators
const int yLED = 22;
const int gLED = 53;

// Motor speed configurations
int speed = 80;
const int rR_speed = 40;
const int lR_speed = 130;
const int rL_speed = 130;
const int lL_speed = 40;
const int brakeSpeed = 0; // Motor brake speed

// Define distance threshold for obstacle detection (in cm)
const int threshold = 17;

void setup() {
  // Set motor driver pins as output
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(A1, OUTPUT);

  // Set ultrasonic sensor pins as output (trigger) and input (echo)
  pinMode(US_l_trig, OUTPUT);
  pinMode(US_l_echo, INPUT);
  pinMode(US_r_trig, OUTPUT);
  pinMode(US_r_echo, INPUT);
  pinMode(US_f_trig, OUTPUT);
  pinMode(US_f_echo, INPUT);

  // Set TCS3200 color sensor control pins as output
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  // Set frequency scaling for color sensor to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  // Set IR sensor pins as input
  pinMode(IR_l, INPUT);
  pinMode(IR_c, INPUT);
  pinMode(IR_r, INPUT);

  // Set LED pins as output
  pinMode(gLED, OUTPUT);
  pinMode(yLED, OUTPUT);

  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Start with moving forward
  forward_D();
  delay(700); // Give it some time before starting
  speed = 70; // Set default speed
}

void loop() {
  int c = digitalRead(IR_c); // Read center IR sensor
  int l = digitalRead(IR_l); // Read left IR sensor
  int r = digitalRead(IR_r); // Read right IR sensor

  // Check if robot is on the track, if not, use ultrasonic sensors and color sensor
  if (c == 0 && r == 0 && l == 0) {
    ultra_Sonic();  // Use ultrasonic sensors for obstacle detection
    color();        // Check for color (blue detection)
  } else {
    IR_sensor();    // Follow the track using IR sensors
    color();        // Check for color (blue detection)
  }
}

// Function to detect color using TCS3200
void color() {
  // Read RGB values from the color sensor
  int redValue = readColor(LOW, LOW);
  int greenValue = readColor(HIGH, HIGH);
  int blueValue = readColor(LOW, HIGH);

  // Print RGB values for debugging
  Serial.print("R: ");
  Serial.print(redValue);
  Serial.print(" G: ");
  Serial.print(greenValue);
  Serial.print(" B: ");
  Serial.println(blueValue);

  // Check if the color is blue within a specific range
  if (blueValue < 95 && redValue < 200 && redValue > 110 && greenValue < 170 && greenValue > 90) {
    brake_D();  // Stop the robot if blue is detected
    Serial.println("Blue detected. Stopping.");
    delay(20000);  // Stop for 20 seconds (or adjust as needed)
  }
}

// Function to read color values based on S2 and S3 pin states
int readColor(int S2State, int S3State) {
  digitalWrite(S2, S2State);
  digitalWrite(S3, S3State);
  delay(10);  // Small delay for stabilization
  return pulseIn(OUT, LOW);  // Measure the LOW pulse duration
}

// Function to handle ultrasonic sensor-based obstacle avoidance
void ultra_Sonic() {
  speed = 65;  // Set speed for obstacle avoidance

  // Measure distances from the ultrasonic sensors
  int distanceLeft = measureDistance(US_l_trig, US_l_echo);
  int distanceRight = measureDistance(US_r_trig, US_r_echo);
  int distanceFront = measureDistance(US_f_trig, US_f_echo);

  // Print distances for debugging
  Serial.print("Left Distance: ");
  Serial.print(distanceLeft);
  Serial.print(" cm, Front Distance: ");
  Serial.print(distanceFront);
  Serial.print(" cm, Right Distance: ");
  Serial.print(distanceRight);
  Serial.println(" cm");

  // Decision-making based on sensor readings
  if (distanceFront < threshold) {
    reverse_D();  // Reverse if obstacle is detected
    delay(600);    // Wait before deciding to turn

    // Turn depending on which side has more space
    if (distanceLeft < distanceRight) {
      right_D();
      delay(500);
    } else {
      left_D();
      delay(500);
    }
  } else if (distanceLeft < threshold) {
    right_D();  // Turn right if left side is blocked
  } else if (distanceRight < threshold) {
    left_D();   // Turn left if right side is blocked
  } else {
    forward_D();  // Move forward if no obstacles are detected
  }
}

// Function to measure distance using an ultrasonic sensor
int measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;  // Convert duration to distance in cm
  return distance;
}

// Function to move the robot right
void right_D() {
  // Right motor: forward
  digitalWrite(A0, HIGH);
  digitalWrite(AIN2, HIGH);
  digitalWrite(AIN1, LOW);
  analogWrite(PWM1, rR_speed);

  // Left motor: reverse
  digitalWrite(A1, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWM2, lR_speed);

  Serial.println("Turning Right");
  digitalWrite(yLED, LOW);
  digitalWrite(gLED, HIGH);
}

// Function to move the robot left
void left_D() {
  // Right motor: reverse
  digitalWrite(A0, HIGH);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWM1, rL_speed);

  // Left motor: forward
  digitalWrite(A1, HIGH);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWM2, lL_speed);

  Serial.println("Turning Left");
  digitalWrite(yLED, HIGH);
  digitalWrite(gLED, LOW);
}

// Function to move the robot forward
void forward_D() {
  // Right motor: forward
  digitalWrite(A0, HIGH);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWM1, speed);

  // Left motor: forward
  digitalWrite(A1, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWM2, speed);

  Serial.println("Moving Forward");
  digitalWrite(yLED, HIGH);
  digitalWrite(gLED, HIGH);
}

// Function to reverse the robot
void reverse_D() {
  // Right motor: reverse
  digitalWrite(A0, HIGH);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWM1, speed);

  // Left motor: reverse
  digitalWrite(A1, HIGH);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWM2, speed);

  Serial.println("Reversing");
  digitalWrite(yLED, LOW);
  digitalWrite(gLED, LOW);
}

// Function to read IR sensors and follow the line
void IR_sensor() {
  int l2 = digitalRead(IR_l);
  int c = digitalRead(IR_c);
  int r2 = digitalRead(IR_r);

  Serial.print("L2 Reading: ");
  Serial.print(l2);
  Serial.print("  C Reading: ");
  Serial.print(c);
  Serial.print("  R2 Reading: ");
  Serial.println(r2);

  if (l2 == 1) {
    left_D();  // Turn left if the left sensor detects the line
  } else if (r2 == 1) {
    right_D(); // Turn right if the right sensor detects the line
  } else if (l2 == 0 && r2 == 0) {
    forward_D(); // Move forward if both sensors detect the line
  }
}

// Function to stop the robot
void brake_D() {
  // Right motor: stop
  digitalWrite(A0, HIGH);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  analogWrite(PWM1, brakeSpeed);

  // Left motor: stop
  digitalWrite(A1, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  analogWrite(PWM2, brakeSpeed);

  Serial.println("Stopped");
  digitalWrite(yLED, LOW);
  digitalWrite(gLED, LOW);
}
