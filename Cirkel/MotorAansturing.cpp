#include "USBAPI.h"
#include <Wire.h>
#include <Zumo32U4.h>
#include "MotorAansturing.h"


Zumo32U4Motors motors;

/*
 Constructor for the MotorAansturing class.
 Initializes private member variables.
*/
MotorAansturing::MotorAansturing(): snelheid_links(0), snelheid_rechts(0) {}

/*
 Method to start the motors.
 Initializes the speeds of both motors.
*/
void MotorAansturing::begin(int l, int r) {
  snelheid_links = l;
  snelheid_rechts = r;
  verander_snelheid(snelheid_links, snelheid_rechts);
}

/*
 Method to ride forwards.
 Makes sure the motors have the same positive speed.
*/
void MotorAansturing::vooruit() {
  if ((snelheid_links == 0) || (snelheid_rechts == 0)) { 
    snelheid_links = 250;
    snelheid_rechts = 250;
    verander_snelheid(snelheid_links, snelheid_rechts);
  } 
  else if ((snelheid_links < 0) || (snelheid_rechts < 0)) {
    if ((snelheid_links < 0) && (snelheid_rechts < 0)) {
      verander_snelheid(-snelheid_links, -snelheid_rechts);
    } 
    else if ((snelheid_links < 0) && (snelheid_rechts > 0)) {
      verander_snelheid(-snelheid_links, snelheid_rechts);
    }
    else {
      verander_snelheid(snelheid_links, -snelheid_rechts);
    }
  } 
  else {
    verander_snelheid(snelheid_links, snelheid_rechts);
  }
}

/*
 Method to ride backwards.
 Makes sure the motors have the same negative speed.
*/
void MotorAansturing::achteruit() {
  if ((snelheid_links == 0) || (snelheid_rechts == 0)) { 
    snelheid_links = 250;
    snelheid_rechts = 250;
    verander_snelheid(-snelheid_links, -snelheid_rechts);
  } 
  else if ((snelheid_links < 0) || (snelheid_rechts < 0)) { 
    if ((snelheid_links < 0) && (snelheid_rechts < 0)) {
      verander_snelheid(snelheid_links, snelheid_rechts);
    } 
    else if ((snelheid_links < 0) && (snelheid_rechts > 0)) {
      verander_snelheid(snelheid_links, -snelheid_rechts);
    }
    else {
      verander_snelheid(-snelheid_links, snelheid_rechts);
    }
  } 
  else {
    verander_snelheid(-snelheid_links, -snelheid_rechts);
  }
}

/*
 Method to turn the Zumo around itself to the left.
 Makes the speed of the left motor negative and the speed of the right motor positive.
*/
void MotorAansturing::links() {
  if ((snelheid_links == 0) || (snelheid_rechts == 0)) { 
    snelheid_links = 250;
    snelheid_rechts = 250;
    int l = -snelheid_links / 2;
    int r = snelheid_rechts / 2;
    verander_snelheid(l, r);
  } 
  else if ((snelheid_links < 0) || (snelheid_rechts < 0)) {
    if ((snelheid_links < 0) && (snelheid_rechts < 0)) {
      int l = snelheid_links / 2;
      int r = -snelheid_rechts / 2;
      verander_snelheid(l, r);
    } 
    else if ((snelheid_links < 0) && (snelheid_rechts > 0)) {
      int l = snelheid_links / 2;
      int r = snelheid_rechts / 2;
      verander_snelheid(l, r);
    }
    else {
      int l = -snelheid_links / 2;
      int r = -snelheid_rechts / 2;
      verander_snelheid(l, r);
    }
  } 
  else {
    int l = -snelheid_links / 2;
    int r = snelheid_rechts / 2;
    verander_snelheid(l, r);
  }
}

/*
 Method to turn the Zumo around itself to the right.
 Makes the speed of the right motor negative and the speed of the left motor positive.
*/
void MotorAansturing::rechts() {
  if ((snelheid_links == 0) || (snelheid_rechts == 0)) { 
    snelheid_links = 250;
    snelheid_rechts = 250;
    int l = snelheid_links / 2;
    int r = -snelheid_rechts / 2;
    verander_snelheid(l, r);
  } 
  else if ((snelheid_links < 0) || (snelheid_rechts < 0)) {
    if ((snelheid_links < 0) && (snelheid_rechts < 0)) {
      int l = -snelheid_links / 2;
      int r = snelheid_rechts / 2;
      verander_snelheid(l, r);
    } 
    else if ((snelheid_links < 0) && (snelheid_rechts > 0)) {
      int l = -snelheid_links / 2;
      int r = -snelheid_rechts / 2;
      verander_snelheid(l, r);
    }
    else {
      int l = snelheid_links / 2;
      int r = snelheid_rechts / 2;
      verander_snelheid(l, r);
    }
  } 
  else {
    int l = snelheid_links / 2;
    int r = -snelheid_rechts / 2;
    verander_snelheid(l, r);
  }
}

/*
 Method to turn the Zumo a little to the left while still moving forwards.
 Makes the speed of the left motor 3/4 of the original speed. Speeds are positive.
*/
void MotorAansturing::vooruit_links() {
  if ((snelheid_links == 0) || (snelheid_rechts == 0)) { 
    snelheid_links = 250;
    snelheid_rechts = 250;
    int l = snelheid_links - (snelheid_links / 4);
    int r = snelheid_rechts;
    verander_snelheid(l, r);
  } 
  else if ((snelheid_links < 0) || (snelheid_rechts < 0)) {
    if ((snelheid_links < 0) && (snelheid_rechts < 0)) {
      int l = -(snelheid_links - (snelheid_links / 4));
      int r = -snelheid_rechts;
      verander_snelheid(l, r);
    } 
    else if ((snelheid_links < 0) && (snelheid_rechts > 0)) {
      int l = -(snelheid_links - (snelheid_links / 4));
      int r = snelheid_rechts;
      verander_snelheid(l, r);
    }
    else {
      int l = (snelheid_links - (snelheid_links / 4));
      int r = -snelheid_rechts;
      verander_snelheid(l, r);
    }
  } 
  else {
    int l = snelheid_links - (snelheid_links / 4);
    int r = snelheid_rechts;
    verander_snelheid(l, r);
  }
}

/*
 Method to turn the Zumo a little to the right while still moving forwards.
 Makes the speed of the right motor 3/4 of the original speed. Speeds are positive.
*/
void MotorAansturing::vooruit_rechts() {
  if ((snelheid_links == 0) || (snelheid_rechts == 0)) { 
    snelheid_links = 250;
    snelheid_rechts = 250;
    int l = snelheid_links;
    int r = snelheid_rechts - (snelheid_rechts / 4);
    verander_snelheid(l, r);
  } 
  else if ((snelheid_links < 0) || (snelheid_rechts < 0)) {
    if ((snelheid_links < 0) && (snelheid_rechts < 0)) {
      int l = -snelheid_links;
      int r = -(snelheid_rechts - (snelheid_rechts / 4));
      verander_snelheid(l, r);
    } 
    else if ((snelheid_links < 0) && (snelheid_rechts > 0)) {
      int l = -snelheid_links;
      int r = (snelheid_rechts - (snelheid_rechts / 4));
      verander_snelheid(l, r);
    }
    else {
      int l = snelheid_links;
      int r = -(snelheid_rechts - (snelheid_rechts / 4));
      verander_snelheid(l, r);
    }
  } 
  else {
    int l = snelheid_links;
    int r = snelheid_rechts - (snelheid_rechts / 4);
    verander_snelheid(l, r);
  }
}


/*
 Method to turn the Zumo a little to the left while still moving backwards.
 Makes the speed of the left motor 3/4 of the original speed. Speeds are negative.
*/
void MotorAansturing::achteruit_links() {
  if ((snelheid_links == 0) || (snelheid_rechts == 0)) { 
    snelheid_links = 250;
    snelheid_rechts = 250;
    int l = -snelheid_links + (snelheid_links / 4);
    int r = -snelheid_rechts;
    verander_snelheid(l, r);
  } 
  else if ((snelheid_links < 0) || (snelheid_rechts < 0)) {
    if ((snelheid_links < 0) && (snelheid_rechts < 0)) {
      int l = snelheid_links - (snelheid_links / 4);
      int r = snelheid_rechts;
      verander_snelheid(l, r);
    } 
    else if ((snelheid_links < 0) && (snelheid_rechts > 0)) {
      int l = snelheid_links - (snelheid_links / 4);
      int r = -snelheid_rechts;
      verander_snelheid(l, r);
    }
    else {
      int l = -snelheid_links - (snelheid_links / 4);
      int r = snelheid_rechts;
      verander_snelheid(l, r);
    }
  } 
  else {
    int l = -snelheid_links + (snelheid_links / 4);
    int r = -snelheid_rechts;
    verander_snelheid(l, r);
  }
}

/*
 Method to turn the Zumo a little to the left while still moving backwards.
 Makes the speed of the right motor 3/4 of the original speed. Speeds are negative.
*/
void MotorAansturing::achteruit_rechts() {
  if ((snelheid_links == 0) || (snelheid_rechts == 0)) { 
    snelheid_links = 250;
    snelheid_rechts = 250;
    int l = -snelheid_links;
    int r = -snelheid_rechts + (snelheid_rechts / 4);
    verander_snelheid(l, r);
  } 
  else if ((snelheid_links < 0) || (snelheid_rechts < 0)) {
    if ((snelheid_links < 0) && (snelheid_rechts < 0)) {
      int l = snelheid_links;
      int r = snelheid_rechts - (snelheid_rechts / 4);
      verander_snelheid(l, r);
    } 
    else if ((snelheid_links < 0) && (snelheid_rechts > 0)) {
      int l = snelheid_links;
      int r = -snelheid_rechts - (snelheid_rechts / 4);
      verander_snelheid(l, r);
    }
    else {
      int l = -snelheid_links;
      int r = snelheid_rechts - (snelheid_rechts / 4);
      verander_snelheid(l, r);
    }
  } 
  else {
    int l = -snelheid_links;
    int r = -snelheid_rechts + (snelheid_rechts / 4);
    verander_snelheid(l, r);
  }
}

/*
 Method to slowly stop the Zumo.
 slowly reduce the speeds of the motors until it reaches 0.
*/
void MotorAansturing::langzaam_stop() {
  if ((snelheid_links >= 0) && (snelheid_rechts >= 0)) {
    vertragen_tot(0);
  } else {
    versnellen_tot(0);
  }
}

/*
 Method to immediately stop the Zumo.
 Set the speeds of the motors to 0.
*/
void MotorAansturing::noodstop() {
  snelheid_links = 0;
  snelheid_rechts = 0;
  verander_snelheid(snelheid_links, snelheid_rechts);
}

/*
 Method to change the speeds of the motors.
 Set the speeds of the motors to the speeds given in the parameters.
*/
void MotorAansturing::verander_snelheid(int l, int r) {
  snelheid_links = l;
  snelheid_rechts = r;
  motors.setSpeeds(snelheid_links, snelheid_rechts);
}

/*
 Method to increase the speeds of the motors.
 slowly increase the speeds of the motors until it reaches the speed given in the parameter.
*/
void MotorAansturing::versnellen_tot(int s) {
  if ((snelheid_links >= s) && (snelheid_rechts >= s)) {
    Serial.begin(9600);
    Serial.println("Kan niet versnellen");
  } else {
    while ((snelheid_links < s) && (snelheid_rechts < s)) {
      snelheid_links++;
      snelheid_rechts++;
      verander_snelheid(snelheid_links, snelheid_rechts);
    }
  }
}

/*
 Method to reduce the speeds of the motors.
 slowly reduce the speeds of the motors until it reaches the speed given in the parameter.
*/
void MotorAansturing::vertragen_tot(int s) {
  if ((snelheid_links <= s) && (snelheid_rechts <= s)) {
    Serial.begin(9600);
    Serial.println("Kan niet vertragen");
  } else {
    while ((snelheid_links > s) && (snelheid_rechts > s)) {
      snelheid_links--;
      snelheid_rechts--;
      verander_snelheid(snelheid_links, snelheid_rechts);
    }
  }
}

/*
 Method to ask for the speed of the left motor.
 Returns the current speed of the left motor.
*/
int MotorAansturing::geef_snelheid_links() const {
  return snelheid_links;
}

/*
 Method to ask for the speed of the right motor.
 Returns the current speed of the right motor.
*/
int MotorAansturing::geef_snelheid_rechts() const {
  return snelheid_rechts;
}