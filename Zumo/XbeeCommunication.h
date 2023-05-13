#ifndef XBEECOMMUNICATION_H
#define XBEECOMMUNICATION_H

#include <Arduino.h>

class XbeeCommunication {
  public:
    XbeeCommunication();
    void setup();
    void loop();
    void HandleSpecialMessages(const String &msg);
    bool gotMessage;
    String recievedVar;
    String recievedArg;
  private:
    String messagePart;
    bool receiving;
    bool beforesemicolon;
};

#endif
