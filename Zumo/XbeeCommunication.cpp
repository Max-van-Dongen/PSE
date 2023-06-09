#include "XbeeCommunication.h"

/**
   @brief Constructor for the XbeeCommunication class.
   Initializes private member variables.
*/
XbeeCommunication::XbeeCommunication() : messagePart(""), receiving(false), beforesemicolon(true), recievedVar(""), recievedArg(""), gotMessage(false) {}

/**
   @brief Set up the XBee communication.
   Initializes the serial communication for XBee at a baud rate of 115200.
*/
void XbeeCommunication::setup()
{
  Serial1.begin(115200); // Start Xbee serial
}

/**
   @brief Handles special messages from the XBee communication.
   It toggles receiving mode on and off, switches before and after semicolon, and constructs the message part.

   @param msg The incoming message string to handle.
*/
void XbeeCommunication::HandleSpecialMessages(const String &msg)
{
  if (msg.length() == 0)
    return;
  if (msg == "*")
  {
    receiving = !receiving;
    if (!beforesemicolon)
    {
      beforesemicolon = true;
      recievedArg = messagePart;
      messagePart = "";
      gotMessage = true;
    }
    return;
  }
  if (msg == ":")
  {
    beforesemicolon = !beforesemicolon;
    recievedVar = messagePart;
    messagePart = "";
    return;
  }
  if (receiving)
  {
    messagePart += msg;
  }
}


/**
   @brief Handles internal function calls based on incoming commands from the XBee communication.

   The method controls the robot's motor speeds, enables/disables line tracking, and calibrates the line follower sensor based on specific command inputs.

   @param var The command input as a String. It can take values such as "w", "a", "s", "d", "e", "ls", "rs", "c", and "q".
   @param arg The argument for the command. For motor speed commands, it's the desired speed. For calibration and line tracking commands, it's not used.
*/
void XbeeCommunication::InternalFunctions(String &var, String &arg)
{
  if (var == "w")
  {
    motors.setSpeeds(arg.toInt(), arg.toInt());
  }
  if (var == "a")
  {
    motors.setSpeeds(-arg.toInt(), arg.toInt());
  }
  if (var == "s")
  {
    motors.setSpeeds(-arg.toInt(), -arg.toInt());
  }
  if (var == "d")
  {
    motors.setSpeeds(arg.toInt(), -arg.toInt());
  }
  if (var == "e")
  {
    motors.setSpeeds(0, 0);
  }
  if (var == "ls")
  { // LeftSpeed
    motors.setLeftSpeed(arg.toInt() ? arg.toInt() : 0);
  }
  if (var == "rs")
  { // RightSpeed
    motors.setRightSpeed(arg.toInt() ? arg.toInt() : 0);
  }
  if (var == "c")
  {
    Serial1.println("Sensors: Start");
    Serial1.println("Sensors: Calibrate");
    LineFollower.calibratee();
  }
  if (var == "q")
  {
    LineFollower.runLines = !LineFollower.runLines;
    if (LineFollower.runLines)
    {
      Serial1.println("Start Line Tracking");
    }
    else
    {
      Serial1.println("Stop Line Tracking");
    }
  }
}

/**
   @brief Main loop for handling XBee communication.
   Checks if there are any incoming messages, reads the incoming byte, and handles the message.
*/
void XbeeCommunication::loop()
{
  // Check if there is any data available from the serial port
  if (Serial1.available()) {
    char incomingChar = Serial1.read(); // Read the incoming character from the serial port

    // Construct a string from the incoming character
    String incomingString;
    incomingString = incomingChar;//Casts zijn eng >w<

    // Process special messages from the incoming string
    HandleSpecialMessages(incomingString);
  }

  // If a message has been received, then process it
  if (gotMessage) {
    // Reset the message flag
    gotMessage = false;

    // Execute internal functions based on the received variables and arguments
    InternalFunctions(recievedVar, recievedArg);
  }
}
