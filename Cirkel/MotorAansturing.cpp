#include "USBAPI.h"
#include <Wire.h>
#include <Zumo32U4.h>
#include "MotorAansturing.h"


Zumo32U4Motors motors;

MotorAansturing::MotorAansturing(): snelheid_links(0), snelheid_rechts(0) {}

void MotorAansturing::verander_richting(int l, int r) {
  motors.setSpeeds(l, r);
}

void MotorAansturing::begin(int l, int r) {
  snelheid_links = l;
  snelheid_rechts = r;
  verander_richting(snelheid_links, snelheid_rechts);
}
void MotorAansturing::vooruit() {
  if ((snelheid_links == 0) || (snelheid_rechts == 0)) {
    snelheid_links = 250;
    snelheid_rechts = 250;
    verander_richting(snelheid_links, snelheid_rechts);
  } else if ((snelheid_links < 0) && (snelheid_rechts < 0)) {
    verander_richting(-snelheid_links, -snelheid_rechts);
  } else {
    verander_richting(snelheid_links, snelheid_rechts);
  }
}

void MotorAansturing::achteruit() {
  if ((snelheid_links == 0) || (snelheid_rechts == 0)) {
    snelheid_links = 250;
    snelheid_rechts = 250;
    verander_richting(-snelheid_links, -snelheid_rechts);
  } else if ((snelheid_links < 0) && (snelheid_rechts < 0)) {
    verander_richting(snelheid_links, snelheid_rechts);
  } else {
    verander_richting(-snelheid_links, -snelheid_rechts);
  }
}

void MotorAansturing::links() {
  if ((snelheid_links == 0) || (snelheid_rechts == 0)) {
    snelheid_links = 250;
    snelheid_rechts = 250;
    int l = -snelheid_links / 2;
    int r = snelheid_rechts / 2;
    verander_richting(l, r);
  } else if ((snelheid_links < 0) && (snelheid_rechts < 0)) {
    int l = snelheid_links / 2;
    int r = -snelheid_rechts / 2;
    verander_richting(l, r);
  } else {
    int l = -snelheid_links / 2;
    int r = snelheid_rechts / 2;
    verander_richting(l, r);
  }
}

void MotorAansturing::rechts() {
  if ((snelheid_links == 0) || (snelheid_rechts == 0)) {
    snelheid_links = 250;
    snelheid_rechts = 250;
    int l = snelheid_links / 2;
    int r = -snelheid_rechts / 2;
    verander_richting(l, r);
  } else if ((snelheid_links < 0) && (snelheid_rechts < 0)) {
    int l = -snelheid_links / 2;
    int r = snelheid_rechts / 2;
    verander_richting(l, r);
  } else {
    int l = snelheid_links / 2;
    int r = -snelheid_rechts / 2;
    verander_richting(l, r);
  }
}

void MotorAansturing::vooruit_links() {
  if ((snelheid_links == 0) || (snelheid_rechts == 0)) {
    snelheid_links = 250;
    snelheid_rechts = 250;
    int l = snelheid_links - (snelheid_links / 4);
    int r = snelheid_rechts;
    verander_richting(l, r);
  } else if ((snelheid_links < 0) && (snelheid_rechts < 0)) {
    int l = -(snelheid_links - (snelheid_links / 4));
    int r = -snelheid_rechts;
    verander_richting(l, r);
  } else {
    int l = snelheid_links - (snelheid_links / 4);
    int r = snelheid_rechts;
    verander_richting(l, r);
  }
}

void MotorAansturing::vooruit_rechts() {
  if ((snelheid_links == 0) || (snelheid_rechts == 0)) {
    snelheid_links = 250;
    snelheid_rechts = 250;
    int l = snelheid_links;
    int r = snelheid_rechts - (snelheid_rechts / 4);
    verander_richting(l, r);
  } else if ((snelheid_links < 0) && (snelheid_rechts < 0)) {
    int l = -snelheid_links;
    int r = -(snelheid_rechts - (snelheid_rechts / 4));
    verander_richting(l, r);
  } else {
    int l = snelheid_links;
    int r = snelheid_rechts - (snelheid_rechts / 4);
    verander_richting(l, r);
  }
}

void MotorAansturing::achteruit_links() {
  if ((snelheid_links == 0) || (snelheid_rechts == 0)) {
    snelheid_links = 250;
    snelheid_rechts = 250;
    int l = -snelheid_links + (snelheid_links / 4);
    int r = -snelheid_rechts;
    verander_richting(l, r);
  } else if ((snelheid_links < 0) && (snelheid_rechts < 0)) {
    int l = snelheid_links - (snelheid_links / 4);
    int r = snelheid_rechts;
    verander_richting(l, r);
  } else {
    int l = -snelheid_links + (snelheid_links / 4);
    int r = -snelheid_rechts;
    verander_richting(l, r);
  }
}

void MotorAansturing::achteruit_rechts() {
  if ((snelheid_links == 0) || (snelheid_rechts == 0)) {
    snelheid_links = 250;
    snelheid_rechts = 250;
    int l = -snelheid_links;
    int r = -snelheid_rechts + (snelheid_rechts / 4);
    verander_richting(l, r);
  } else if ((snelheid_links < 0) && (snelheid_rechts < 0)) {
    int l = snelheid_links;
    int r = snelheid_rechts - (snelheid_rechts / 4);
    verander_richting(l, r);
  } else {
    int l = -snelheid_links;
    int r = -snelheid_rechts + (snelheid_rechts / 4);
    verander_richting(l, r);
  }
}

void MotorAansturing::langzaam_stop() {
  int s = 0;
  if ((snelheid_links >= s) && (snelheid_rechts >= s)) {
    vertragen_tot(s);
  } else {
    versnellen_tot(s);
  }
}

void MotorAansturing::noodstop() {
  snelheid_links = 0;
  snelheid_rechts = 0;
  verander_richting(snelheid_links, snelheid_rechts);
}

void MotorAansturing::verander_snelheid(int l, int r) {
  snelheid_links = l;
  snelheid_rechts = r;
  motors.setSpeeds(snelheid_links, snelheid_rechts);
}


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

int MotorAansturing::geef_snelheid_links() const {
  return snelheid_links;
}

int MotorAansturing::geef_snelheid_rechts() const {
  return snelheid_rechts;
}