#pragma once
#include <arduino.h>
#include "Buffer.h"
#include "Definitions.h"
#include "Functions.h"
#include "PhysicalLayer.h"
#include "Packet.h"
#include <LiquidCrystal.h>


Packet::Packet()
{
  //Serial.println("Empty Packet Created"); 
  for(int i = 0; i < MESSAGE_SIZE; i++)
  {
    bytes[i] = 0; 
  }
}



Packet::Packet(byte* array_pointer, unsigned int array_len)
{
  //Serial.println("Packet Created from Array"); 
  for(int i = 0; i < MESSAGE_SIZE; i++)
  {
    bytes[i] = 0; 
  }
  for(int i = 0; i < array_len && i < MESSAGE_SIZE; i++)
  {
    bytes[i] = array_pointer[i]; 
  }
}



Packet::Packet(const Packet &p)
{
  for(int i = 0; i < MESSAGE_SIZE; i++)
  {
    bytes[i] = p.returnByte(i); 
  }
}



Packet::~Packet()
{
  //Serial.println("Packet Deleted"); 
  //delete[] bytes; 
}

void Packet::operator = (const Packet &p)
{
  for(int i = 0; i < MESSAGE_SIZE; i++)
  {
    bytes[i] = p.returnByte(i); 
  }
}



byte Packet::returnByte(unsigned int bytenr) const
{
  if(bytenr > MESSAGE_SIZE)
  {
    Serial.println("ERROR: PACKET: Byte Nr. too high"); 
    return 0;  
  }
  else
  {
    return bytes[bytenr];
  }
}



void Packet::changeByte(unsigned int bytenr, byte value)
{
  if(bytenr > MESSAGE_SIZE)
  {
    Serial.println("ERROR: PACKET: Byte Nr. too high");   
  }
  else
  {
    bytes[bytenr] = value;
  }
}


byte Packet::returnReceivePort(void) const
{
  return receivePort;
}



void Packet::changeReceivePort(byte newReceivePort)
{
  if(newReceivePort > NR_OF_PORTS)
  {
    Serial.println("ERROR: PACKET: Receive Port too large");
    return; 
  }
  receivePort = newReceivePort; 
  return;
}

bool Packet::returnSendPortActive(byte portNr)
{
  if(portNr == 0 || portNr > NR_OF_PORTS)
  {
    Serial.println("ERROR: PACKET: Send Port too large or zero");
    return false; 
  }
  return sendPorts[portNr - 1]; 
}



String Packet::returnAllSendPorts(void)
{
  String result = ""; 
  for(int i = 0; i < NR_OF_PORTS; i++)
  {
    if(sendPorts[i])
    {
      result += String(i + 1, DEC); 
      result += ", "; 
    }
  }
  return result; 
}



void Packet::addSendPort(byte portNr)
{
  if(portNr == 0 || portNr > NR_OF_PORTS)
  {
    Serial.println("ERROR: PACKET: Send Port too large or zero");
    return; 
  }
  sendPorts[portNr - 1] = true;
  return; 
}



void Packet::removeAllSendPorts(void)
{
  for(int i = 0; i < NR_OF_PORTS; i++)
  {
    sendPorts[i] = false; 
  }
  return; 
}


String Packet::toString()
{
  String result = ""; 
  for(int i = 0; i < MESSAGE_SIZE; i++)
  {
    byte b = bytes[i];
    String s; 
    if (b == 0)
    {
      s = "00"; 
    }
    else
    {
      s = String(b, HEX); 
    }
    if(s.length() == 1)
    {
      s = '0' + s; 
    } 
    result += s;
  } 
  return result;
}
