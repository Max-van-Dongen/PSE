#include "XbeeCommunication.h"

XbeeCommunication::XbeeCommunication() : messagePart(""), receiving(false), beforesemicolon(true), recievedVar(""), recievedArg(""),gotMessage(false) {}

void XbeeCommunication::setup() {
  Serial1.begin(115200); // Start Xbee serial
}

void XbeeCommunication::HandleSpecialMessages(const String &msg) {
  if (msg.length() == 0) return;
  if (msg == "*") {
    receiving = !receiving;
    if (!beforesemicolon) {
      beforesemicolon = true;
      recievedArg = messagePart;
      messagePart = "";
      gotMessage = true;
      //            InternalFunctions(recievedVar, recievedArg);
    }
    return;
  }
  if (msg == ":") {
    beforesemicolon = !beforesemicolon;
    recievedVar = messagePart;
    messagePart = "";
    return;
  }
  if (receiving) {
    messagePart += msg;
  }
}

void XbeeCommunication::loop() {
  if (Serial1.available()) { // Check if there is any incoming data
    char incomingChar = Serial1.read(); // Read the incoming byte
    HandleSpecialMessages((String)incomingChar);
  }
}
