#pragma once
#include <arduino.h>
#include "Buffer.h"
#include "Definitions.h"
#include "Functions.h"
#include "PhysicalLayer.h"
#include "Packet.h"
#include <LiquidCrystal.h>


class Buffer{
  private:

  class Node{
    public:
    
    // PACKET TO BE STORED
    Packet* packet;
    
    // POINTER TO NEXT NODE
    Node* next;
  
  
    // CONSTRUCTOR
    // IN: NEW VALUE AS UNSIGNED LONG INT AND POINTER TO NEXT NODE
    // OUT: CREATES THIS NEW NODE
    Node(const Packet &_packet, Node* _next);
  
    // DESTRUCTOR
    ~Node();
    
  };

  // LENGTH OF THE BUFFER
  unsigned int len = 0;

  // POINTER TO THE FIRST NODE
  Node* first;


  public: 

  // CONSTRUCTOR (EMPTY BUFFER) 
  // IN: NONE
  // OUT: A NEW BUFFER, len = 0, frist = NULL
  Buffer();

  // RETURNS LEN AS INT
  // IN: NONE
  // OUT: LENGTH OF BUFFER AS INT
  int return_len(void);

  // RETURNS TRUE IF DATA IS BUFFERED
  // IN: NONE
  // OUT: BOOL
  bool available(void);

  // PUSH A NEW VALUE TO THE FRONT OF THE BUFFER
  // IN: VALUE TO BE STORED AS PACKET OBJECT
  // OUT: VALUE IS STORED TO THE FRONT OF THE BUFFER
  void push(const Packet &_packet);

  // READ AND DELETE THE LAST ELEMENT TO THE BACK OF THE BUFFER
  // IN: PACKET OBJECT WHERE RESULT IS STORED IN
  // OUT: THE LAST ELEMENT TO THE BACK OF THE BUFFER
  void pop(Packet &store_element);

  // ONLY READ THE LAST ELEMENT TO THE BACK OF THE BUFFER
  // IN: PACKET OBJECT WHERE RESULT IS STORED IN
  // OUT: THE LAST ELEMENT TO THE BACK OF THE BUFFER INSIDE STORE_ELEMENT
  void lastpeek(Packet &store_element);

  // ONLY READ THE FIRST ELEMENT TO THE FRONT OF THE BUFFER
  // IN: PACKET OBJECT WHERE RESULT IS STORED IN
  // OUT: THE FIRST ELEMENT TO THE FRONT OF THE BUFFER INSIDE STORE_ELEMENT
  //void firstpeek(Packet &store_element);
  
};
