#pragma once
#include <arduino.h>
#include "Buffer.h"
#include "Definitions.h"
#include "Functions.h"
#include "PhysicalLayer.h"
#include "Packet.h"
#include <LiquidCrystal.h>



Buffer::Node::Node(const Packet &_packet, Node* _next)
{
  packet = new Packet(_packet); //COPY CONSTRUCTOR
  next = _next;
}



Buffer::Node::~Node()
{
  //Serial.print("BUFFER: Node Deleted: "); 
  //Serial.println(packet->toString()); 
  delete packet; 
}



Buffer::Buffer()
{
  //Serial.println("Buffer Created"); 
  len = 0;
  first = NULL;
}

int Buffer::return_len(void)
{
  return len; 
}


bool Buffer::available(void)
{
  return len > 0; 
}


void Buffer::push(const Packet &_packet)
{
  if(len == BUFFER_LIMIT)
  {
    Serial.println("BUFFER: Limit reached"); 
    return;
  }
  len++;
  Node* old_first = first;
  first = new Node(_packet, old_first);
  //Serial.print("BUFFER: Element Added: "); 
  //Serial.print(_packet.toString()); 
  //Serial.print(", len = "); 
  //Serial.println(len); 
  return; 
}



void Buffer::pop(Packet &store_element)
{
  if (len == 0){
    Serial.println("ERROR: BUFFER: Is empty"); 
    return; 
  } 
  
  Node* current_node = first;
  for(int i = 0; i < len - 1; i++)
  {
      current_node = current_node->next;
  }
  store_element = *current_node->packet;  
  len--;
  if(len == 0)
  {
    first = NULL;
  }
  //Serial.print("BUFFER: Element Removed: "); 
  //Serial.print(store_element.toString());
  //Serial.print(", len = "); 
  //Serial.println(len);
  delete current_node;
  return; 
}



void Buffer::lastpeek(Packet &store_element)
{
  if (len == 0){
    Serial.println("ERROR: BUFFER: Is empty");  
    return; 
  }
  
  Node* current_node = first;
  for(int i = 0; i < len - 1; i++)
  {
      current_node = current_node->next;
  }
  store_element = *current_node->packet;
  return; 
}



/*
void Buffer::firstpeek(Packet &store_element)
{
  if (len == 0){
    Serial.println("ERROR: BUFFER: Is empty");  
    return; 
  }
  
  store_element = *first->packet;
  return; 
}
*/
