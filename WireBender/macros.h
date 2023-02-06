// DLN - Remove this junk from the main file.
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

#ifndef WIREBEND_MACRO_H
#define WIREBEND_MACRO_H 1

void star() {
  int count = 0;
  while (count != 5) {
    int feed = 38; //  mm
    int feedDistance = feed * 48; // 48- constats that map the mm value to number of steps the stepper show move
    while (feederStepper.currentPosition() != feedDistance) { // run until it reaches the distance value
      feederStepper.setSpeed(1200);
      feederStepper.run();
    }
    feederStepper.setCurrentPosition(0); // reset the current position to 0
    servo01.write(40); // Set the bender pin up
    delay(200);
    int angleConst = 18; // angle constant
    // Bend the wire 52 degrees
    while (benderStepper.currentPosition() != -52 * angleConst) {
      benderStepper.setSpeed(-700);
      benderStepper.run();
    }
    benderStepper.setCurrentPosition(0);
    delay(100);
    // Go back 52 degrees to initial position
    while (benderStepper.currentPosition() != 52 * angleConst) {
      benderStepper.setSpeed(1200);
      benderStepper.run();
    }
    benderStepper.setCurrentPosition(0);
    delay(100);
    // Feed the same distance again
    while (feederStepper.currentPosition() != feedDistance) {
      feederStepper.setSpeed(1200);
      feederStepper.run();
    }
    feederStepper.setCurrentPosition(0);
    delay(100);
    servo01.write(130); // Set the bender pin down
    delay(200);
    // Set bender to new initial position, for bending in the other direction
    while (benderStepper.currentPosition() != -42 * angleConst) {
      benderStepper.setSpeed(-1200);
      benderStepper.run();
    }
    benderStepper.setCurrentPosition(0);
    delay(200);
    servo01.write(40); // Bender pin up
    delay(200);
    while (benderStepper.currentPosition() != 105 * angleConst) {
      benderStepper.setSpeed(700);
      benderStepper.run();
    }
    benderStepper.setCurrentPosition(0);
    delay(50);
    while (benderStepper.currentPosition() != -63 * angleConst) {
      benderStepper.setSpeed(-1200);
      benderStepper.run();
    }
    delay(100);
    servo01.write(130);
    benderStepper.setCurrentPosition(0);
    count++;
  }
}

void cube() {
  int count = 0;
  int feed = 40; //  mm
  int feedDistance = feed * 48;
  int angleConst = 16;
  // Step 1
  while (count != 3) {
    while (feederStepper.currentPosition() != feedDistance) {
      feederStepper.setSpeed(1200);
      feederStepper.run();
    }
    feederStepper.setCurrentPosition(0);
    delay(100);
    while (benderStepper.currentPosition() != -90 * angleConst) {
      benderStepper.setSpeed(-700);
      benderStepper.run();
    }
    benderStepper.setCurrentPosition(0);
    delay(100);
    while (benderStepper.currentPosition() != 90 * angleConst) {
      benderStepper.setSpeed(1200);
      benderStepper.run();
    }
    benderStepper.setCurrentPosition(0);
    delay(100);
    count++;
  }
  count = 0;
  // Step 2
  while (zAxisStepper.currentPosition() != 88 * angleConst) {
    zAxisStepper.setSpeed(500);
    zAxisStepper.run();
  }
  zAxisStepper.setCurrentPosition(0);
  delay(100);
  //Step 3
  while (count != 2) {
    while (feederStepper.currentPosition() != feedDistance) {
      feederStepper.setSpeed(1200);
      feederStepper.run();
    }
    feederStepper.setCurrentPosition(0);
    delay(100);
    while (benderStepper.currentPosition() != -90 * angleConst) {
      benderStepper.setSpeed(-700);
      benderStepper.run();
    }
    benderStepper.setCurrentPosition(0);
    delay(100);
    while (benderStepper.currentPosition() != 90 * angleConst) {
      benderStepper.setSpeed(1200);
      benderStepper.run();
    }
    benderStepper.setCurrentPosition(0);
    delay(100);
    count++;
  }
  count = 0;
  // Step 4
  while (zAxisStepper.currentPosition() != 85 * angleConst) {
    zAxisStepper.setSpeed(500);
    zAxisStepper.run();
  }
  zAxisStepper.setCurrentPosition(0);
  delay(100);
  // Step 5
  servo01.write(130);
  delay(200);
  while (benderStepper.currentPosition() != -42 * angleConst) {
    benderStepper.setSpeed(-1200);
    benderStepper.run();
  }
  benderStepper.setCurrentPosition(0);
  while (count != 3) {
    delay(100);
    servo01.write(40);
    delay(200);
    // Step 6
    while (feederStepper.currentPosition() != feedDistance) {
      feederStepper.setSpeed(1200);
      feederStepper.run();
    }
    feederStepper.setCurrentPosition(0);
    delay(100);
    while (benderStepper.currentPosition() != 90 * angleConst) {
      benderStepper.setSpeed(700);
      benderStepper.run();
    }
    benderStepper.setCurrentPosition(0);
    delay(100);
    while (benderStepper.currentPosition() != -90 * angleConst) {
      benderStepper.setSpeed(-1200);
      benderStepper.run();
    }
    benderStepper.setCurrentPosition(0);
    delay(100);
    count++;
  }
  count = 0;
}

void stand() {
  int feed = 20; // mm
  int feedDistance = feed * 48;
  int angleConst = 16;
  // Step 1
  while (feederStepper.currentPosition() != feedDistance) {
    feederStepper.setSpeed(1200);
    feederStepper.run();
  }
  feederStepper.setCurrentPosition(0);
  delay(100);
  while (benderStepper.currentPosition() != -90 * angleConst) {
    benderStepper.setSpeed(-700);
    benderStepper.run();
  }
  benderStepper.setCurrentPosition(0);
  delay(100);
  while (benderStepper.currentPosition() != 90 * angleConst) {
    benderStepper.setSpeed(1200);
    benderStepper.run();
  }
  benderStepper.setCurrentPosition(0);
  delay(100);

  // Step 2
  while (feederStepper.currentPosition() != feedDistance) {
    feederStepper.setSpeed(1200);
    feederStepper.run();
  }
  feederStepper.setCurrentPosition(0);
  delay(100);
  while (benderStepper.currentPosition() != -70 * angleConst) {
    benderStepper.setSpeed(-700);
    benderStepper.run();
  }
  benderStepper.setCurrentPosition(0);
  delay(100);
  while (benderStepper.currentPosition() != 70 * angleConst) {
    benderStepper.setSpeed(1200);
    benderStepper.run();
  }
  benderStepper.setCurrentPosition(0);
  delay(100);

  // Step 3
  feed = 80; // mm
  feedDistance = feed * 48;
  while (feederStepper.currentPosition() != feedDistance) {
    feederStepper.setSpeed(1200);
    feederStepper.run();
  }
  feederStepper.setCurrentPosition(0);
  delay(100);
  // Step 4
  servo01.write(130);
  delay(200);
  while (benderStepper.currentPosition() != -42 * angleConst) {
    benderStepper.setSpeed(-1200);
    benderStepper.run();
  }
  benderStepper.setCurrentPosition(0);
  delay(100);
  servo01.write(40);
  delay(200);
  while (benderStepper.currentPosition() != 108 * angleConst) {
    benderStepper.setSpeed(700);
    benderStepper.run();
  }
  benderStepper.setCurrentPosition(0);
  delay(100);
  while (benderStepper.currentPosition() != -66 * angleConst) {
    benderStepper.setSpeed(-1200);
    benderStepper.run();
  }
  benderStepper.setCurrentPosition(0);

  //Step 5
  servo01.write(130);
  delay(200);
  // Step 6
  feed = 80; // mm
  feedDistance = feed * 48;
  while (feederStepper.currentPosition() != feedDistance) {
    feederStepper.setSpeed(1200);
    feederStepper.run();
  }
  feederStepper.setCurrentPosition(0);
  servo01.write(40);
  delay(200);
  // Step 7
  while (zAxisStepper.currentPosition() != -90 * angleConst) {
    zAxisStepper.setSpeed(-500);
    zAxisStepper.run();
  }
  zAxisStepper.setCurrentPosition(0);
  delay(100);

  // Step 8
  while (benderStepper.currentPosition() != -90 * angleConst) {
    benderStepper.setSpeed(-700);
    benderStepper.run();
  }
  benderStepper.setCurrentPosition(0);
  delay(100);
  while (benderStepper.currentPosition() != 90 * angleConst) {
    benderStepper.setSpeed(1200);
    benderStepper.run();
  }
  benderStepper.setCurrentPosition(0);
  delay(100);
  // Step 6
  feed = 45; // mm
  feedDistance = feed * 48;
  while (feederStepper.currentPosition() != feedDistance) {
    feederStepper.setSpeed(1200);
    feederStepper.run();
  }
  feederStepper.setCurrentPosition(0);
  // Step 10
  while (benderStepper.currentPosition() != -90 * angleConst) {
    benderStepper.setSpeed(-700);
    benderStepper.run();
  }
  benderStepper.setCurrentPosition(0);
  delay(100);
  while (benderStepper.currentPosition() != 48 * angleConst) {
    benderStepper.setSpeed(1200);
    benderStepper.run();
  }
  benderStepper.setCurrentPosition(0);
  delay(100);

  // Step 11
  while (zAxisStepper.currentPosition() != 90 * angleConst) {
    zAxisStepper.setSpeed(500);
    zAxisStepper.run();
  }
  zAxisStepper.setCurrentPosition(0);
  delay(100);
  feed = 80; // mm
  feedDistance = feed * 48;
  while (feederStepper.currentPosition() != feedDistance) {
    feederStepper.setSpeed(1200);
    feederStepper.run();
  }
  feederStepper.setCurrentPosition(0);

  // Step 12
  while (benderStepper.currentPosition() != 110 * angleConst) {
    benderStepper.setSpeed(700);
    benderStepper.run();
  }
  benderStepper.setCurrentPosition(0);
  delay(100);
  while (benderStepper.currentPosition() != -68 * angleConst) {
    benderStepper.setSpeed(-1200);
    benderStepper.run();
  }
  benderStepper.setCurrentPosition(0);
  //Step 13
  servo01.write(130);
  delay(200);
  feed = 80; // mm
  feedDistance = feed * 48;
  while (feederStepper.currentPosition() != feedDistance) {
    feederStepper.setSpeed(1200);
    feederStepper.run();
  }
  feederStepper.setCurrentPosition(0);
  servo01.write(40);
  delay(200);

  // Step 14
  while (benderStepper.currentPosition() != -70 * angleConst) {
    benderStepper.setSpeed(-700);
    benderStepper.run();
  }
  benderStepper.setCurrentPosition(0);
  delay(100);
  while (benderStepper.currentPosition() != 70 * angleConst) {
    benderStepper.setSpeed(1200);
    benderStepper.run();
  }
  benderStepper.setCurrentPosition(0);
  delay(100);

  //Step 15
  feed = 25; // mm
  feedDistance = feed * 48;
  while (feederStepper.currentPosition() != feedDistance) {
    feederStepper.setSpeed(1200);
    feederStepper.run();
  }
  feederStepper.setCurrentPosition(0);
  delay(100);
  // Step 16
  while (benderStepper.currentPosition() != -90 * angleConst) {
    benderStepper.setSpeed(-700);
    benderStepper.run();
  }
  benderStepper.setCurrentPosition(0);
  delay(100);
  while (benderStepper.currentPosition() != 90 * angleConst) {
    benderStepper.setSpeed(1200);
    benderStepper.run();
  }
  benderStepper.setCurrentPosition(0);
  delay(100);

  // Step 17
  while (feederStepper.currentPosition() != feedDistance) {
    feederStepper.setSpeed(1200);
    feederStepper.run();
  }
  feederStepper.setCurrentPosition(0);
}

#endif 
