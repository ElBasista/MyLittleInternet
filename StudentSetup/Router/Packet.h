#pragma once
#include <arduino.h>
#include "Buffer.h"
#include "Definitions.h"
#include "Functions.h"
#include "PhysicalLayer.h"
#include "Packet.h"
#include <LiquidCrystal.h>

class Packet{
  private: 

  //ARRAY IN WICH BYTES FOR PACKET ARE STORED 
  byte bytes[MESSAGE_SIZE];

  //CONTAINS INFORMATION ON WICH PORT THE PACKET WAS RECEIVED
  byte receivePort = 0;

  //CONTAINS TRUE IF MESSAGE SHOULD BE SENT ON THIS PORT, FALSE IF NOT
  bool sendPorts[NR_OF_PORTS];
  
  public: 

  // CONSTRUCTOR
  // IN: NONE
  // OUT: CREATES OBJECT WITH ALL BYTES TO ZERO
  Packet();

  // CONSTRUCTOR
  // IN: 1) AN ARRAY, 2) THE LENGTH OF THIS ARRAY
  // OUT: CREATES OBJECT WITH BYTES SET TO ARRAY VALUES 
  Packet(byte* array_pointer, unsigned int array_len);

  // COPY-CONSTRUCTOR
  // IN: AN OTHER PACKET
  // OUT: A NEW PACKET WITH THE SAME BYTE VALUES
  Packet(const Packet &p);

  // DESTRUCTOR
  // IN: NONE
  // OUT: DELETES PACKET
  ~Packet(); 

  // EQUAL OPERATOR
  // IN: PACKET2 = PACKET1
  // OUT: CHANGES ALL VALUES OF PACKET2 TO VALUES OF PACKET2
  void operator = (const Packet &p);

  // RETURNS THE REQUESTED BYTE
  // IN: BYTE WICH SHOULD BE RETURNED (bytenr < MESSAGE_SIZE)
  // OUT: RETURNS THE REQUESTED BYTE
  byte returnByte(unsigned int bytenr) const; 

  // CHANGES ONE BYTE IN PACKET
  // IN: BYTE WICH SHOULD BE CHANGED (bytenr < MESSAGE_SIZE)
  // OUT: CHANGES THE REQUESTED BYTE
  void changeByte(unsigned int bytenr, byte value); 

  // RETURNS THE PORT ON WICH PACKET HAS BEEN RECEIVED
  // IN: BYTE WICH SHOULD BE RETURNED (bytenr < MESSAGE_SIZE)
  // OUT: RETURNS THE REQUESTED BYTE
  byte returnReceivePort(void) const;

  // RETURNS THE PORT ON WICH PACKET HAS BEEN RECEIVED
  // IN: BYTE WICH SHOULD BE RETURNED (bytenr < MESSAGE_SIZE)
  // OUT: RETURNS THE REQUESTED BYTE
  void changeReceivePort(byte newReceivePort);

  // RETURNS STRING WITH ALL ADDED SEND PORTS, zB: "1, 3,"
  // IN: NONE
  // OUT: STRING
  bool returnSendPortActive(byte portNr); 

  // RETURNS STRING WITH ALL ADDED SEND PORTS, zB: "1, 3,"
  // IN: NONE
  // OUT: STRING
  String returnAllSendPorts(void); 

  // ADDS PORT TO THE LIST OF SEND PORTS, ON WICH THE PACKET WILL BE SENT
  // IN: BYTE WITH PORTNR [1,2,3,4,5]
  // OUT: NONE
  void addSendPort(byte portNr); 

  // REMOVES ALL ADDED SEND PORTS
  // IN: NONE
  // OUT: NONE
  void removeAllSendPorts(void); 

  // PRINTS THE ENTIRE PACKET AS ONE HEX STRING (USE WITH CARE, EXPENSIVE FUNCTION)
  // IN: NONE
  // OUT: STRING THAT CONTAINS ALL THE BYTES AS HEX: (FOR EXAMPLE: 0A12FF32...)
  String toString();
  
};
