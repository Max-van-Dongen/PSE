#ifndef XBEECOMMUNICATION_H
#define XBEECOMMUNICATION_H

#include <Arduino.h>
#include <Zumo32U4.h>
#include "LineFollowing.h"

class XbeeCommunication
{
  public:
    XbeeCommunication();
    void setup();
    void loop();
    void HandleSpecialMessages(const String &msg);
    bool gotMessage;
    String recievedVar;
    String recievedArg;
    void InternalFunctions(String &var, String &arg);

  private:
    Zumo32U4Motors motors;
    LineFollowing LineFollower;
    int ctr = 0;
    String messagePart;
    bool receiving;
    bool beforesemicolon;
};

#endif
