// Maze Solving Robot Code
//
// This code is designed to control a maze-solving robot using the Adafruit Motor Shield and three infrared (IR) sensors to detect the robot's surroundings.
// The robot uses the IR sensors to sense the walls of the maze and determine the appropriate actions, such as moving forward, turning left or right, or performing a U-turn.
//
// Key Features:
// 1. IR Sensor Integration: Detects obstacles in front, left, and right of the robot.
// 2. Motor Control: Uses the Adafruit Motor Shield to control two DC motors for movement.
// 3. Decision-Making Logic: Implements a switch-case structure based on sensor states for navigation.
// 4. Customizable Parameters: Speed and delay values can be adjusted for smooth operation.
// 5. Debugging Capability: Serial output for monitoring the robot's behavior in real-time.
//
// The code is modular, with separate functions for forward movement, turning, stopping, and U-turns, ensuring clarity and ease of modification.

#include <AFMotor.h>  // Include the Adafruit Motor Shield library for motor control

// Motor Definitions
AF_DCMotor motorA(1);  // Motor A connected to terminal M1 on the motor shield
AF_DCMotor motorB(2);  // Motor B connected to terminal M2 on the motor shield

// Sensor Pin Definitions
const int leftSensor = A0;   // Left IR sensor pin
const int frontSensor = A1;  // Front IR sensor pin
const int rightSensor = A2;  // Right IR sensor pin

// Movement Parameters
const int forwardSpeed = 120;  // Speed for forward movement
const int TurningSpeed = 115;  // Speed for turning movements
const int turnDelay = 25;      // Delay for completing a turn
const int uTurnDelay = 50;    // Delay for completing a U-turn

void setup() {
  // Configure sensor pins as input
  pinMode(leftSensor, INPUT);
  pinMode(frontSensor, INPUT);
  pinMode(rightSensor, INPUT);

  // Initialize serial communication for debugging
  Serial.begin(9600);
}

void loop() {
  // Read sensor values (0 or 1)
  int leftValue = digitalRead(leftSensor);
  int frontValue = digitalRead(frontSensor);
  int rightValue = digitalRead(rightSensor);

  // Combine sensor states into a single value for switch-case logic
  int sensorState = (leftValue << 2) | (frontValue << 1) | rightValue;

  // Decision-making based on sensor states
  switch (sensorState) {
    case 0b000:  // No sensors detect a wall
      uTurn();   // Perform a U-turn
      Serial.println("Stop");
      break;
    case 0b010:       // Only the front sensor detects a wall
      moveForward();  // Move forward
      Serial.println("Move Forward");
      break;
    case 0b111:    // All sensors detect walls
      turnLeft();  // Turn left
      Serial.println("Turn Left");
      break;
    case 0b100:    // Only the left sensor detects a wall
      turnLeft();  // Turn left
      Serial.println("Turn Left");
      break;
    case 0b110:    // Front and left sensors detect walls
      turnLeft();  // Turn left
      Serial.println("Turn Left");
      break;
    case 0b001:     // Only the right sensor detects a wall
      turnRight();  // Turn right
      Serial.println("Turn Right");
      break;
    case 0b011:     // Front and right sensors detect walls
      turnRight();  // Turn right
      Serial.println("Turn Right");
      break;
    case 0b101:      // Left and right sensors detect walls
      stopMotors();  // Stop the motors
      Serial.println("Turn Left");
      break;
    default:         // Unknown sensor state
      stopMotors();  // Stop the motors as a safety measure
      Serial.println("Unknown State");
      break;
  }
}

// Function to move forward
void moveForward() {
  motorA.setSpeed(forwardSpeed);  // Set speed for motor A
  motorB.setSpeed(forwardSpeed);  // Set speed for motor B
  motorA.run(FORWARD);            // Move motor A forward
  motorB.run(FORWARD);            // Move motor B forward
}

// Function to turn left
void turnLeft() {
  motorA.setSpeed(TurningSpeed - 20);  // Reduce speed of motor A for smoother turn
  motorB.setSpeed(TurningSpeed);       // Set speed for motor B
  motorA.run(BACKWARD);                // Move motor A backward
  motorB.run(FORWARD);                 // Move motor B forward
  delay(turnDelay);                    // Delay to complete the turn
}

// Function to turn right
void turnRight() {
  motorA.setSpeed(TurningSpeed);       // Set speed for motor A
  motorB.setSpeed(TurningSpeed - 20);  // Reduce speed of motor B for smoother turn
  motorA.run(FORWARD);                 // Move motor A forward
  motorB.run(BACKWARD);                // Move motor B backward
  delay(turnDelay);                    // Delay to complete the turn
}

// Function to stop the motors
void stopMotors() {
  motorA.run(RELEASE);  // Release motor A
  motorB.run(RELEASE);  // Release motor B
}

// Function to perform a U-turn
void uTurn() {
  motorA.setSpeed(TurningSpeed);  // Set speed for motor A
  motorB.setSpeed(TurningSpeed);  // Set speed for motor B
  motorA.run(FORWARD);            // Move motor A forward
  motorB.run(BACKWARD);           // Move motor B backward
  delay(uTurnDelay);              // Delay to complete the U-turn
}
