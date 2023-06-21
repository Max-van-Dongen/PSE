#include "XbeeCommunication.h"

/**
 * @brief Constructor for the XbeeCommunication class.
 * Initializes private member variables.
 */
XbeeCommunication::XbeeCommunication() : messagePart(""), receiving(false), beforesemicolon(true), recievedVar(""), recievedArg(""), gotMessage(false) {}

/**
 * @brief Set up the XBee communication.
 * Initializes the serial communication for XBee at a baud rate of 115200.
 */
void XbeeCommunication::setup()
{
  Serial1.begin(115200); // Start Xbee serial
}

/**
 * @brief Handles special messages from the XBee communication.
 * It toggles receiving mode on and off, switches before and after semicolon, and constructs the message part.
 *
 * @param msg The incoming message string to handle.
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

// INCOMING MESSAGES
void InternalFunctions(String &var, String &arg)
{
  if (var == "AAA")
  {
    buzzer.play("g32");
    delay(arg.toInt());
    buzzer.play("e32");
  }
  if (var == "BBB")
  {
    buzzer.play("e32");
    delay(arg.toInt());
    buzzer.play("f32");
  }
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
  if (var == "hb")
  { // HeartBeat
    Serial1.println("*hb:" + ((String)ctr) + "*");
    ctr++;
  }
  if (var == "init")
  {                                                                         // Init Sensors
    Serial1.println("*initls:" + ((String)LineFollower.runLines) + "*");    // Init Line Sensors
    Serial1.println("*initps:" + ((String)runDistance) + "*");              // Init Prox Sensors
    Serial1.println("*initfls:" + ((String)LineFollower.FollowLine) + "*"); // Init Prox Sensors
    Serial1.println("*initclb:" + ((String)LineFollower.calibrated) + "*"); // Init Prox Sensors
  }
  if (var == "|")
  {
    motors.setSpeeds(0, 0);
    resetFunc();
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
  if (var == "lspd")
  {
    LineFollower.LineSpeed = arg.toInt();
  }
  if (var == "fl")
  {
    LineFollower.FollowLine = !LineFollower.FollowLine;
    if (LineFollower.FollowLine)
    {
      Serial1.println("Start Line Following");
    }
    else
    {
      Serial1.println("Stop Line Following");
      motors.setSpeeds(0, 0);
    }
  }
  if (var == "l")
  {
    runDistance = !runDistance;
    if (runDistance)
    {
      Serial1.println("Start Distance Tracking");
    }
    else
    {
      Serial1.println("Stop Distance Tracking");
    }
  }
  if (var == "h")
  {
    buzzer.play("!T240 V10 L8 a gafaeada c+adaeafa >aa>bac#ada c#adaeaf4");
  }
}

/**
 * @brief Main loop for handling XBee communication.
 * Checks if there are any incoming messages, reads the incoming byte, and handles the message.
 */
void XbeeCommunication::loop()
{
  if (Serial1.available())
  {                                     // Check if there is any incoming data
    char incomingChar = Serial1.read(); // Read the incoming byte
    HandleSpecialMessages((String)incomingChar);
  }
  if (xbee.gotMessage)
  {
    xbee.gotMessage = false;
    InternalFunctions(xbee.recievedVar, xbee.recievedArg);
  }
}
