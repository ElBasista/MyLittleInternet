#pragma once
#include <arduino.h>
#include "Buffer.h"
#include "Definitions.h"
#include "Functions.h"
#include "PhysicalLayer.h"
#include "Packet.h"
#include <LiquidCrystal.h>



PhysicalLayer::PhysicalLayer(LiquidCrystal* _lcd)
{ 
  received_buffer = new Buffer(); 
  send_buffer = new Buffer(); 

  myMAC = analogRead(MAC_PIN)* 9 / 1024; 

  if(OVERRIDE_MAC)
  {
    myMAC = MY_MAC;
  }
  
  delay(200); 
  lcd = _lcd; 
  Serial.print("PhysicalLayer: Channels active: "); 
  for(int i = 0; i < NR_OF_PORTS; i++)
  {
    int a = analogRead(receive_ports[i]); 
    if(a > 1000 || a < 25)
    {
       Serial.print(i + 1); 
       Serial.print(" "); 
       active_ports[i] = true; 
    }
    else
    {
      active_ports[i] = false; 
    }
  }
  Serial.println(""); 
  timeout_break = millis(); 
}



PhysicalLayer::~PhysicalLayer(){  
}




void PhysicalLayer::update()
{   
    //UPDATE ALL THE PORTS
    updatePorts(); 

    //NOW RECEIVE ALL NEW MESSAGES (1 Message per call)
    receiveHandle(); 

    //NOW SEND ALL MESSAGES (1 Message per call)
    sendHandle();    
}



bool PhysicalLayer::available(void)
{
  bool res = received_buffer->return_len() > 0;
  return res; 
}



bool PhysicalLayer::receive(Packet &result)
{
  if(received_buffer->return_len() > 0)
  {
    received_buffer->pop(result); 
    return true;
  }
  else
  {
    return false; 
  }
}



bool PhysicalLayer::send(Packet &packet)
{
  send_buffer->push(packet); 
  return true; 
}



bool PhysicalLayer::portActive(byte port)
{
  if(port > NR_OF_PORTS || port == 0)
  {
    return false; 
  }
  return active_ports[port-1]; 
}



void PhysicalLayer::updatePorts()
{
    receiving_port = 0;
    receiving = false;
    port_state_change = false;
    changed_port = 0; 
    
    for(int i = 0; i < NR_OF_PORTS; i++)
    {
      int a = analogRead(receive_ports[i]); 
      bool old_port_state = active_ports[i];
      if(a > 1000)
      {
         active_ports[i] = true; 
         if(old_port_state == false)
         {
            port_state_change = true;
            changed_port = i + 1; 
         }
         receiving_port = i + 1;
         receiving = true;
      }
      else if(a < 25)
      {
         active_ports[i] = true; 
         if(old_port_state == false)
         {
            port_state_change = true;
            changed_port = i + 1;
         }
      }
      else
      {
        active_ports[i] = false; 
        if(old_port_state == true)
         {
            port_state_change = true;
            changed_port = i + 1;
         }
      }
    }
    
    if(port_state_change == true)
    {
      if(active_ports[changed_port - 1])
      {
        Serial.print("PhysicalLayer: Channel connected: ");
      }
      else
      {
        Serial.print("PhysicalLayer: Channel disconnected: ");
      }
      Serial.println(changed_port); 
    }
}


void PhysicalLayer::receiveHandle()
{
   //IF THERE IS A INCOMING FRAME RECEIVE IT
  if(receiving)
  {
    Packet received_packet; 
    
    //RECEIVE MESSSAGE
    lcd->clear();
    lcd->print("PHY: Receiving");
    bool valid = receiveFrame(received_packet);      
                                       
    //IF THE MESSAGE IS VALID, ADD IT TO THE PACKET BUFFER
    if(valid){
      received_buffer->push(received_packet);

      //PRINT
      lcd->clear();
      lcd->print("PHY: Received:");
      lcd->setCursor(0,1); 
      lcd->print(received_packet.toString());
      Serial.print("PhysicalLayer: Received: ");
      Serial.println(received_packet.toString());
      delay(READ_DELAY); 
    }
    return; 
  }


  //GET SERIAL INPUT IF AVAILABLE
  if(Serial.available() > 0)
  {
    Packet received_packet;

    //RECEIVE MESSSAGE
    bool valid = receiveSerial(received_packet);
    
    //IF THE MESSAGE IS VALID, ADD IT TO THE PACKET BUFFER
    if(valid)
    {
      received_buffer->push(received_packet);

      //PRINT
      lcd->clear();
      lcd->print("SERIAL:Received:");
      lcd->setCursor(0,1); 
      lcd->print(received_packet.toString());
      Serial.print("SERIAL: received: "); 
      Serial.println(received_packet.toString()); 
      delay(READ_DELAY);     
    }
    return; 
  }
}



void PhysicalLayer::sendHandle()
{
  if(millis() < timeout_break)
  {
    return; 
  }
   //IF THERE IS A PAKCET SEND IT
  if(send_buffer->available())
  {
    Packet send_packet; 
    send_buffer->pop(send_packet); 
    
    //SEND MESSAGE
    lcd->clear();
    lcd->print("PHY: Sending");
    lcd->setCursor(0,1); 
    lcd->print(send_packet.toString());
    Serial.print("PhysicalLayer: Sending: ");
    Serial.println(send_packet.toString());  
    for(int i = 0; i < NR_OF_PORTS; i++)
    {
      byte send_port = i + 1; 
      if(send_packet.returnSendPortActive(send_port))
      {
        if(!active_ports[send_port - 1])
        {
          Serial.print("PhysicalLayer: Inactive Channel: "); 
          Serial.println(send_port, DEC);
          continue;
        }
        bool success;
        success = sendFrame(send_packet, send_port);
        if(success)
        {
          port_timeouts[send_port - 1] = 0;
        } 
        else
        {
          port_timeouts[send_port - 1] += 1;
        }
      } 
    }                                             
  }
  return; 
}



/////////////////////////////////////////////////////////////////////////////////////////////
// BIT TRANSMISSION /////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


bool PhysicalLayer::sendFrame(Packet frame, byte port)
  {
      
      // CHECK IF PORT IS VALID (1,2,3,4,5)
      if(port > NR_OF_PORTS){
        Serial.println("ERROR: PhysicalLayer: Invalid Channel");
        return false; 
      }
      if(!active_ports[port - 1])
      {
        Serial.println("ERROR: PhysicalLayer: Inactive Channel"); 
        return false; 
      }
      
      int s_port = send_ports[port - 1];
      int r_port = receive_ports[port - 1];
      int frame_array[4 + 8 * MESSAGE_SIZE];

      //ADD PREAMP [1,0,1,0, ... frame ...]
      frame_array[0] = 1;
      frame_array[1] = 0;
      frame_array[2] = 1;
      frame_array[3] = 0;


      // CREATE FRAME ARRAY  [1,0,1,0 | BYTE 0 | BYTE 1 | BYTE 2 | ...]
      // [ BYTE ] = [MOST SIG. BIT, ... , LEAST SIG. BIT] (FOR EXAMPLE: 9 = 00001001)
      for(int i = 0; i < MESSAGE_SIZE; i++)
      {
        int power_of_two = 1; 
        for(int j = 0; j < 8; j++)
        {
            frame_array[(i + 1) * 8 - j + 3] = frame.returnByte(i) % (2 * power_of_two) / power_of_two;
            power_of_two = 2 * power_of_two; 
        }
      }

      // START TRANSMISSION
      /* 
       *  The sender will set send port to high. The receiver will notice it and
       *  responds by sending the clock for message transmission. 
       *  On every negative flank the sender will change the bit and on 
       *  every positive flank the receiver will read the bit.
      */
      digitalWrite(s_port, true);
 
      // WAIT FOR POSITIVE FLANK RESPONSE BY RECEIVER 
      // IF NO POSITIVE FLANK ARRIVES - RECEIVER NOT RESPONDING - TIMEOUT
      if(!await_positive_flank(r_port, PhysicalLayer_TIMEOUT))
      {
        //TIMEOUT
        digitalWrite(s_port, LOW);
        //OUTPUT
        lcd->clear();
        lcd->print("PHY: TIMEOUT");
        Serial.println("PhysicalLayer: TIMEOUT"); 
        delay(READ_DELAY);
        lcd->clear(); 
        return false;
      }

      // CHANGE DATA ON EVER NEGATIVE FLANK, RECEIVER READS ON POSITIVE FLANK
      for(int i = 1; i < 8 * MESSAGE_SIZE + 4; i++)
      {
        //WAIT FOR NEGATIV FLANK TO SET NEXT BIT
        if(await_negative_flank(r_port, PhysicalLayer_TIMEOUT))
        {
          digitalWrite(s_port, frame_array[i]);
        }
        else
        {
          // TIMEOUT
          digitalWrite(s_port, LOW);

          // OUTPUT
          lcd->clear();
          lcd->print("PHY: TIMEOUT"); 
          Serial.println("PhysicalLayer: TIMEOUT"); 
          delay(READ_DELAY);
          lcd->clear(); 
          return false;
        } 
      }
      await_negative_flank(r_port, MESSAGE_TIME/100);
      digitalWrite(s_port, LOW);
      lcd->clear();
      delay(MESSAGE_TIME / 10);
      return true; 
  }



bool PhysicalLayer::receiveFrame(Packet &result)
  {
      int s_port = send_ports[receiving_port - 1];
      int r_port = receive_ports[receiving_port - 1];
      int frame_array[8 * MESSAGE_SIZE + 4];
      frame_array[0] = true;


      //START TRANSMISSION
      /* 
       *  The sender will set send port to high. The receiver will notice it and
       *  responds by sending the pulse for message transmission. 
       *  On ever negative flank the sender will change the bit and on 
       *  every positive flank the receiver will read the bit.
      */
      digitalWrite(s_port, HIGH);
      delayMicroseconds(MESSAGE_TIME);
      for(int i = 1; i < 8 * MESSAGE_SIZE + 4; i++)
      {
        digitalWrite(s_port, LOW);
        delayMicroseconds(MESSAGE_TIME);
        
        //READ ON POSITIVE FLANK
        if(digitalRead(r_port) == true)
        {
          frame_array[i] = 1;
        }
        else
        {
          frame_array[i] = 0;
        }
        digitalWrite(s_port, HIGH);
        delayMicroseconds(MESSAGE_TIME);
      }
      digitalWrite(s_port, LOW);


      //CHECK IF THE PREAMP IS CORRECT, IF NOT IT MUST BE AN TRANSMISSION ERROR
      if(frame_array[0] == 0 || frame_array[1] == 1|| frame_array[2] == 0|| frame_array[3] == 1)
      { 
        // PRINT
        lcd->clear(); 
        lcd->print("PHY: Invalid");
        Serial.println("ERROR: PhysicalLayer: Invalid frame received");
        delay(READ_DELAY); 
        lcd->clear();     
        return false;
      }

      // EXTRACT FRAME ARRAY
      for(int i = 0; i < MESSAGE_SIZE; i++)
      {
        int power_of_two = 1; 
        byte b = 0; 
        for(int j = 0; j < 8; j++)
        {
            b = b + frame_array[(i + 1) * 8 - j + 3] * power_of_two;
            power_of_two = 2 * power_of_two; 
        }
        result.changeByte(i, b); 
      }

      result.changeReceivePort(receiving_port); 

      delay(MESSAGE_TIME / 10);
      return true;
  }



bool PhysicalLayer::await_positive_flank(int port_pin, unsigned long timeout)
  {
      unsigned long int t_start = millis();
      unsigned long int t = millis();
      while(digitalRead(port_pin) == 1)
      {
        delayMicroseconds(FLANK_AWAIT_TIME);
        t = millis();
        if (t > t_start + timeout)
        {
          return false; 
        }
      }
      while(digitalRead(port_pin) == 0)
      {
        delayMicroseconds(FLANK_AWAIT_TIME);
        t = millis();
        if (t > t_start + timeout)
        {
          return false; 
        }
      }
      return true;
  }



bool PhysicalLayer::await_negative_flank(int port_pin, unsigned long timeout)
  {
      unsigned long int t_start = millis();
      unsigned long int t = millis();
      while(digitalRead(port_pin) == 0)
      {
        delayMicroseconds(FLANK_AWAIT_TIME);
        t = millis();
        if (t > t_start + timeout)
        {
          return false; 
        }
      }
      while(digitalRead(port_pin) == 1)
      {
        delayMicroseconds(FLANK_AWAIT_TIME);
        t = millis();
        if (t > t_start + timeout)
        {
          return false; 
        }
      }
      return true;
  }



bool PhysicalLayer::receiveSerial(Packet &result)
{
    char incomingByte = 0;
    String resp = ""; 
    if (Serial.available() > 0) {
      delay(100);   
      while(Serial.available() > 0)
      {
          incomingByte = Serial.read();
          if(incomingByte == ' ')
          {
            continue; 
          }
          resp += incomingByte;
      }
      Serial.print(resp); 
      int len = resp.length();

      if (len < 2 * MESSAGE_SIZE - 1)
      {
        Serial.print("ERROR: SERIAL: Message to short"); 
        return false; 
      }

      for(int i = 0; i < MESSAGE_SIZE; i++)
      { 
          char c1 = resp[2*i];
          char c2 = resp[2*i + 1];  

          int n1 = hexCharToInt(c1); 
          int n2 = hexCharToInt(c2); 

          result.changeByte(i, 16*n1 + n2); 
      }
      result.changeReceivePort(0);
      return true; 
      
  }
}
