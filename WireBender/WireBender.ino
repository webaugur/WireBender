#include <MultiStepper.h>
#include <AccelStepper.h>

/*
  Arduino 3D Wire Bending Machine
  by Dejan Nedelkovski
  www.HowToMechatronics.com
  Library - AccelStepper by Mike McCauley:
  http://www.airspayce.com/mikem/arduino/AccelStepper/index.html
*/

#include <AccelStepper.h>
#include <Servo.h>


#define limitSwitch 11

// Define the stepper motors and the pins the will use
AccelStepper feederStepper(1, 5, 6); // (Type:driver, STEP, DIR)
AccelStepper zAxisStepper(1, 7, 8);
AccelStepper benderStepper(1, 9, 10);

Servo servo01;
String dataIn = "";
String manualStatus = "";
int dist;

void manual(void);


void setup() {
  Serial.begin(9600);
  Serial.println("# Wirebender Setup Begins");

  // Attach Servo and Limit Switch
  pinMode(limitSwitch, INPUT_PULLUP);
  servo01.attach(2);
  servo01.write(40); // Initial position, bending pin up
  Serial.println("# Servo attached");

  // Stepper motors max speed
  feederStepper.setMaxSpeed(2000);
  zAxisStepper.setMaxSpeed(2000);
  benderStepper.setMaxSpeed(2000);
  Serial.println("# Set Stepper Speed");
  
  // Homing
  Serial.println("# Homing Stepper Begins");
  while (digitalRead(limitSwitch) != 0) {
    benderStepper.setSpeed(1200);
    benderStepper.runSpeed();
    benderStepper.setCurrentPosition(0); // When limit switch pressed set position to 0 steps
  }
  Serial.println("# Homing Stepper Complete");
  
  delay(40);
  
  Serial.println("# Wind Wire to Starting Position");
  // Move 1400 steps from the limit switch to starting position
  while (benderStepper.currentPosition() != -1400) {
    benderStepper.setSpeed(-1200); // if negative rotates anti-clockwise
    benderStepper.run();
  }
  Serial.println("# Set Start Position Here");
  benderStepper.setCurrentPosition(0);

  Serial.println("# Wirebender Setup Complete");

  // Print some instructions
  Serial.println("# Commands Available: manual");
}

void loop() {
  String mode = Serial.readString();
  if (mode.startsWith("manual")) {
    manual();
  }
/*  
 *   Reimplement this as an array of commands
 *   if (mode.startsWith("star")) {
    star();
  }
  if (mode.startsWith("cube")) {
    cube();
  }
  if (mode.startsWith("stand")) {
    stand();
  }*/
}


void manual() {
  int sign;
  String dataInS;
  int angle;
  int angleConst;
  Serial.println("# Entering Manual Mode");
  Serial.println("# f### | Y Feed  📏 |  mm | f123");
  Serial.println("# x### | X Angle 📐 | deg | x-120");
  Serial.println("# z### | Z Angle 📐 | deg | z-90");
  while (!dataIn.startsWith("end")) {
    servo01.write(130);
    delay(200);
    dataIn = Serial.readString();
    if (dataIn.startsWith("f")) {
      dataInS = dataIn.substring(1, dataIn.length()); // reads the feed value
      dist = dataInS.toInt();
      Serial.print("Feed 📏 ");
      Serial.print(dist);
      Serial.println("mm wire.");
      dist = dist * 48;
      while (feederStepper.currentPosition() != dist) {
        feederStepper.setSpeed(1200);
        feederStepper.run();
      }
      feederStepper.setCurrentPosition(0);
      delay(100);
    }
    if (dataIn.startsWith("x")) { 
      if (dataIn.charAt(1) == '-') { 
        dataInS = dataIn.substring(2, dataIn.length()); ///reads the angle value
        angle = dataInS.toInt();
        Serial.print("Bend X-Axis 📐 -");
        Serial.print(angle);
        Serial.println(" degrees.");
        angleConst = 16;
        // Set "negative" bending initial position
        while (benderStepper.currentPosition() != -43 * angleConst) {
          benderStepper.setSpeed(-1200);
          benderStepper.run();
        }
        benderStepper.setCurrentPosition(0);
        delay(100);
        servo01.write(40);
        delay(200);
        // Bend the wire
        while (benderStepper.currentPosition() != angle * angleConst) {
          benderStepper.setSpeed(700);
          benderStepper.run();
        }
        benderStepper.setCurrentPosition(0);
        delay(100);
        while (benderStepper.currentPosition() != (-1) * angle * angleConst) {
          benderStepper.setSpeed(-1200);
          benderStepper.run();
        }
        benderStepper.setCurrentPosition(0);
        delay(100);
        servo01.write(130);
        delay(200);
        // Get back to original "positive" bending initial poistion
        while (benderStepper.currentPosition() != 43 * angleConst) {
          benderStepper.setSpeed(1200);
          benderStepper.run();
        }
        benderStepper.setCurrentPosition(0);
        delay(100);
      }
      else {
        dataInS = dataIn.substring(1, dataIn.length());
        angle = dataInS.toInt();
        Serial.print("Bend ");
        Serial.print(angle);
        Serial.println(" degrees.");
        angleConst = 16;
        servo01.write(40);
        delay(200);
        while (benderStepper.currentPosition() != (-1) *angle * angleConst) {
          benderStepper.setSpeed(-700);
          benderStepper.run();
        }
        benderStepper.setCurrentPosition(0);
        delay(100);
        while (benderStepper.currentPosition() != angle * angleConst) {
          benderStepper.setSpeed(1200);
          benderStepper.run();
        }
        benderStepper.setCurrentPosition(0);
        delay(100);
      }
      dataInS = dataIn.substring(2, dataIn.length());
      angle = dataInS.toInt();
      angleConst = 16;
      while (benderStepper.currentPosition() != sign * angle * angleConst) {
        benderStepper.setSpeed(-700);
        benderStepper.run();
      }
      benderStepper.setCurrentPosition(0);
      delay(100);
      while (benderStepper.currentPosition() != sign * angle * angleConst) {
        benderStepper.setSpeed(1200);
        benderStepper.run();
      }
      benderStepper.setCurrentPosition(0);
      delay(100);
    }
    // Z-Axis Control
    if (dataIn.startsWith("z")) {
      if (dataIn.charAt(1) == '-') {
        dataInS = dataIn.substring(2, dataIn.length());
        angle = dataInS.toInt();
        Serial.print("Bend Z-Axis 📐 -");
        Serial.print(angle);
        Serial.println(" degrees.");
        angleConst = 16;
        while (zAxisStepper.currentPosition() != angle * angleConst) {
          zAxisStepper.setSpeed(500);
          zAxisStepper.run();
        }
        zAxisStepper.setCurrentPosition(0);
        delay(100);
      }
      else {
        dataInS = dataIn.substring(1, dataIn.length());
        angle = dataInS.toInt();
        Serial.print("Bend Z-Axis 📐 ");
        Serial.print(angle);
        Serial.println(" degrees.");
        angleConst = 16;
        while (zAxisStepper.currentPosition() != (-1) *angle * angleConst) {
          zAxisStepper.setSpeed(-500);
          zAxisStepper.run();
        }
        zAxisStepper.setCurrentPosition(0);
        delay(100);
      }
    }
    manualStatus = dataIn;
  }
}
