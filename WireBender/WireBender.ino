#include <MultiStepper.h>
#include <AccelStepper.h>
#include <stdio.h>
#include <stdarg.h>
/*
  Arduino 3D Wire Bending Machine
  by Dejan Nedelkovski
  www.HowToMechatronics.com
  Library - AccelStepper by Mike McCauley:
  http://www.airspayce.com/mikem/arduino/AccelStepper/index.html
*/

#include <AccelStepper.h>
#include <Servo.h>

// Define the stepper motors and the pins the will use
AccelStepper feederStepper(1, 5, 6); // (Type:driver, STEP, DIR)
AccelStepper zAxisStepper(1, 7, 8);
AccelStepper benderStepper(1, 9, 10);
Servo servo01;
#define limitSwitch 11

/* Function Declarations */
void manual(void);
void set_pin_up(void);
void set_pin_down(void);
void y_axis_feed(int dist);
void print_user_msg(int num, ...);
void print_user_msg(String msg);
String read_command(void);
int parse_field_value(String dataIn);
void move_bender(int angle, int feed_rate);

/* Hardware Initialization */
void setup() {
  Serial.begin(9600);
  print_user_msg("# Wirebender Setup Begins");

  // Attach Servo and Limit Switch
  pinMode(limitSwitch, INPUT_PULLUP);
  servo01.attach(2);
  set_pin_up();
  print_user_msg("# Servo attached");

  // Stepper motors max speed
  feederStepper.setMaxSpeed(2000);
  zAxisStepper.setMaxSpeed(2000);
  benderStepper.setMaxSpeed(2000);
  print_user_msg("# Set Stepper Speed");
  
  // Homing
  print_user_msg("# Homing Stepper Begins");
  while (digitalRead(limitSwitch) != 0) {
    benderStepper.setSpeed(1200);
    benderStepper.runSpeed();
    benderStepper.setCurrentPosition(0); // When limit switch pressed set position to 0 steps
  }
  print_user_msg("# Homing Stepper Complete");
  
  delay(40);
  
  print_user_msg("# Wind Wire to Starting Position");
  // Move 1400 steps from the limit switch to starting position
  while (benderStepper.currentPosition() != -1400) {
    benderStepper.setSpeed(-1200); // if negative rotates anti-clockwise
    benderStepper.run();
  }
  print_user_msg("# Set Start Position Here");
  benderStepper.setCurrentPosition(0);

  print_user_msg("# Wirebender Setup Complete");

  // Print some instructions
  print_user_msg("# Commands Available: manual");
}

/* Main Loop */
void loop() {
  if (read_command().startsWith("manual")) {
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


// Pin Up = Set the Pin Position Up
void set_pin_up(void){
  delay(100);
  servo01.write(40); // 40 degrees
  delay(200);
}


// Pin Down = Set the Pin Position Down
void set_pin_down(void){
  delay(100);
  servo01.write(130); // 130 degrees
  delay(200);
}


// Read Input from User Terminal
String read_command(void){
  return Serial.readString();
}

int parse_field_value(String dataIn){
    int offset = 1; // Positive Int
    if (dataIn.charAt(1) == '-') offset = 2; // Negative Int
    return dataIn.substring(offset, dataIn.length()).toInt(); // reads the int value
}

// Move Y Axis by dist mm
void y_axis_feed(int dist){
      dist = dist * 48;
      while (feederStepper.currentPosition() != dist) {
        feederStepper.setSpeed(1200);
        feederStepper.run();
      }
      feederStepper.setCurrentPosition(0);
      delay(100);
}


/* print_user_msg(String msg) 
 *  String msg Message text
 * 
 * Print Multiple Messages to User Terminal
 *
 */
// Print Single Message to User Terminal
void print_user_msg(String msg){
   Serial.println(msg);
}

/* print_user_msg(int num, ...) 
 *  int num Total Number of Message Arguments to Follow
 *  String ... variable number of String arguments 
 * 
 * Print Multiple Messages to User Terminal
 *
 */
void print_user_msg(int num, ...){
   va_list valist;
   String msg;
   int i;

   /* initialize valist for num number of arguments */
   va_start(valist, num);

   /* access all the arguments assigned to valist */
   for (i = 0; i < num; i++) {
      msg += va_arg(valist, String);
   }
  
   /* clean memory reserved for valist */
   va_end(valist);

   // Print Message to Serial Port
   print_user_msg(msg);
}

// Change Bender Angle
void move_bender(int angle, int feed_rate){
        while (benderStepper.currentPosition() != angle) {
          benderStepper.setSpeed(feed_rate);
          benderStepper.run();
        }
        benderStepper.setCurrentPosition(0);
        delay(100);
}

void manual() {
  int sign = -1; // Default to Negative Sign
  String dataInS;
  int angle;
  int angleConst;
  String msg;
  String manualStatus = "";
  String dataIn = "";
  int dist;

  print_user_msg("# Entering Manual Mode");
  print_user_msg("# y### | Y Feed  📏 |  mm | f123");
  print_user_msg("# b### | B Angle 📐 | deg | x-120");
  print_user_msg("# c### | C Angle 📐 | deg | z-90");
  
  while (!dataIn.startsWith("end")) {
    set_pin_down();
    dataIn = read_command();

    
    // Y-Axis Control
    if (dataIn.startsWith("y")) {
      dist = parse_field_value(dataIn);
      print_user_msg(3, "Feed 📏 ", dist, "mm wire.");
      // Move Y Axis
      y_axis_feed(dist);
    }

    
    // B-Axis Control
    if (dataIn.startsWith("b")) {
      // Parse User Command
      angle = parse_field_value(dataIn);
      // negative int
      if (dataIn.charAt(1) == '-') sign = 1;
      print_user_msg(3,"Bend B-Axis 📐 ", (sign * -1) * angle, " degrees.");
      angleConst = 16;

      // Move to "negative" bending initial position
      if(sign == 1) move_bender(-43 * angleConst, -1200);

      // Bend Wire
      set_pin_up();
      move_bender(sign * angle * angleConst, (sign * -1) * 700);

      // Retract Bender
      move_bender(sign * angle * angleConst, (sign * -1) * 1200);
      set_pin_down();
      
      // Get back to original "positive" bending initial positon
      move_bender(43 * angleConst, 1200);

    }

    
    // C-Axis Control
    if (dataIn.startsWith("c")) {
      if (dataIn.charAt(1) == '-') {
        angle = parse_field_value(dataIn);
        print_user_msg(3,"Bend B-Axis 📐 ", angle, " degrees.");
        angleConst = 16;
        while (zAxisStepper.currentPosition() != angle * angleConst) {
          zAxisStepper.setSpeed(500);
          zAxisStepper.run();
        }
        zAxisStepper.setCurrentPosition(0);
        delay(100);
      }
      else {
        angle = parse_field_value(dataIn);
        print_user_msg(3,"Bend B-Axis 📐 ", angle, " degrees.");
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
