#pragma once
#include <arduino.h>
#include "Buffer.h"
#include "Definitions.h"
#include "Functions.h"
#include "PhysicalLayer.h"
#include "Packet.h"
#include <LiquidCrystal.h>



void initialize_pins(void)
{
  //PORTS
  pinMode(SEND_PORT_1, OUTPUT);
  pinMode(SEND_PORT_2, OUTPUT);
  pinMode(SEND_PORT_3, OUTPUT);
  pinMode(SEND_PORT_4, OUTPUT);
  pinMode(SEND_PORT_5, OUTPUT);
  pinMode(RECEIVE_PORT_1, INPUT);
  pinMode(RECEIVE_PORT_2, INPUT);
  pinMode(RECEIVE_PORT_3, INPUT);
  pinMode(RECEIVE_PORT_4, INPUT);
  pinMode(RECEIVE_PORT_5, INPUT);
  digitalWrite(SEND_PORT_1, LOW);
  digitalWrite(SEND_PORT_2, LOW);
  digitalWrite(SEND_PORT_3, LOW);
  digitalWrite(SEND_PORT_4, LOW);
  digitalWrite(SEND_PORT_5, LOW);
}


int hexCharToInt(char c)
{
  if(c == '0'){
    return 0; 
  }
  else if(c == '1'){
    return 1; 
  }
  else if(c == '2'){
    return 2; 
  }
  else if(c == '3'){
    return 3; 
  }
  else if(c == '4'){
    return 4; 
  }
  else if(c == '5'){
    return 5; 
  }
  else if(c == '6'){
    return 6; 
  }
  else if(c == '7'){
    return 7; 
  }
  else if(c == '8'){
    return 8; 
  }
  else if(c == '9'){
    return 9; 
  }
  else if(c == 'a' || c == 'A'){
    return 10; 
  }
  else if(c == 'b' || c == 'B'){
    return 11; 
  }
  else if(c == 'c' || c == 'C'){
    return 12; 
  }
  else if(c == 'd' || c == 'D'){
    return 13; 
  }
  else if(c == 'e' || c == 'E'){
    return 14; 
  }
  else if(c == 'f' || c == 'F'){
    return 15; 
  }
  else{
    Serial.println("Non Hex Value"); 
    return 0; 
  }
  
}
