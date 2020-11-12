#include "Definitions.h"
#include "Functions.h"
#include "Buffer.h"
#include "PhysicalLayer.h"
#include <LiquidCrystal.h>


void setup() {}

void loop() {

/////////////////////////////////////////////////////////////////////////////////////////////
// SETUP ////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


//INITIALIZE IN- AND OUTPUT PINS
initialize_pins();

//INITIALIZE SERIAL OUTPUT
Serial.begin(9600);

//CREATE LCD OBJECT FOR DISPLAY
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7); 
lcd.begin(16,2);
lcd.display();
lcd.clear();


//CREATE OBJECT FOR DATA TRANSMISSION
PhysicalLayer pLayer(&lcd);


//CONTAINS SEGMENTS WITH IP-HEADER TO BE TREATED BY TCP
Buffer segment_buffer; 

//CONTAINS SEGMENTS TO BE SENT 

//PRINT MY MAC FIRST
lcd.print("My MAC: ");
lcd.print(pLayer.myMAC, HEX);
Serial.print("My MAC address is: ");
Serial.println(pLayer.myMAC, HEX);
delay(1000); 


/////////////////////////////////////////////////////////////////////////////////////////////
// LOOOP ////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

while(true){
  //PHYSICAL - LAYER
 
  //LISTEN TO PORTS FOR INCOMING FRAME
  pLayer.update();


  //LINK - LAYER

  if(pLayer.available())
  {
      Packet thispacket;
      pLayer.receive(thispacket); 
      
      // START PROGRAMMING YOUR SWITCHES, HOSTS, ROUTERS AND NAT ROUTERS FROM HERE  ....
      // ...
      // ...

      //EXTRACT HEADER INFORMATION
      /*
      byte receivePort = thispacket.returnReceivePort();

      byte dest_mac = thispacket.returnByte(DEST_MAC);
      byte src_mac = thispacket.returnByte(SRC_MAC);
      byte type = thispacket.returnByte(TYPE);
      byte checksum = thispacket.returnByte(CHECKSUM);

      byte dest_ip = thispacket.returnByte(DEST_IP); 
      byte src_ip = thispacket.returnByte(SRC_IP); 
      byte ttl = thispacket.returnByte(TTL); 
      byte next_protocol = thispacket.returnByte(NEXT_PROTOCOL); 
      */
      
      //CHANGE SOME THINGS
      /*
      byte myIP = 0xf1;
      thispacket.changeByte(TTL, ttl - 1);
      thispacket.changeByte(DEST_IP, src_ip);
      thispacket.changeByte(SRC_IP, myIP);
      */
      
      //SPECIFY PORTS ON WHICH TO SEND
      /*
      thispacket.addSendPort(1); 
      thispacket.addSendPort(2); 
      */

      //SEND PACKET
      pLayer.send(thispacket); 
      
  }

  
}
}
