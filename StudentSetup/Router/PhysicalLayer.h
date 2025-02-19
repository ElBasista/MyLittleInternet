#pragma once
#include <arduino.h>
#include "Buffer.h"
#include "Definitions.h"
#include "Functions.h"
#include "Packet.h"
#include <LiquidCrystal.h>
class Buffer;


class PhysicalLayer{
  
  private:

  //TELLS YOU IF THERE CURRENTLY IS AN INCOMING FRAME AT ONE OF THE PORTS/PORTS
  bool receiving = false;
  
  //TELLS YOU ON WHICH PORT/PORT THERE IS AN INCOMING FRAME, NONE = 0
  int receiving_port = 0;

  //TELLS YOU IF THE STATE OF ANY PORT HAS CHANGED
  bool port_state_change = false; 
  
  //TELLS YOU WHICH PORT CHANGED STATE
  byte changed_port = 0; 

  //CONTAINS ALL THE RECEIVED ELEMENTS
  Buffer* received_buffer; 

  //CONTAINS ALL THE ELEMENTS TO BE SENT
  Buffer* send_buffer; 

  //HAS THE NEXT AVAILABLE SEND TIME
  unsigned long int timeout_break; 

  //LCD OBJECT, USED TO PRINT TO THE LCD SCREEN
  LiquidCrystal* lcd;

  //STORES ALL THE SENDING PORTS/PORTS
  int send_ports[NR_OF_PORTS] = {SEND_PORT_1, SEND_PORT_2, SEND_PORT_3, SEND_PORT_4, SEND_PORT_5};

  //STORES ALL THE RECEIVING PORTS/PORTS
  int receive_ports[NR_OF_PORTS] = {RECEIVE_PORT_1, RECEIVE_PORT_2, RECEIVE_PORT_3, RECEIVE_PORT_4, RECEIVE_PORT_5};

  //STORES NUMBER OF TIMEOUTS ON CHANNELS
  byte port_timeouts[NR_OF_PORTS] = {0, 0, 0, 0, 0};

  //STORES WHICH PORTS ARE ACTIVE
  bool active_ports[NR_OF_PORTS] = {false, false, false, false, false};



  

  //WAITS UNTIL THE INPUT PORT "port_pin" CHANGES FROM 0 TO 1
  //OUT: FALSE IF TIMEOUT, TRUE OTHERWISE
  bool await_positive_flank(int port_pin, unsigned long timeout);

  //WAITS UNTIL THE INPUT PORT "port_pin" CHANGES FROM 1 TO 0
  //OUT: FALSE IF TIMEOUT, TRUE OTHERWISE
  bool await_negative_flank(int port_pin, unsigned long timeout);

  //SEND FRAME TRANSMITTS THIS FRAME TO THE NEXT ROUTER ON THE GIVEN PORT/PORT
  //IN: PACKET TO BE SENT AS UNSIGNED LONG INT AND THE PORT/PORT AS INT
  //OUT: TRUE IF SUCCESS, FALSE IF FAILED TO TRANSMITT
  bool sendFrame(Packet frame, byte port);

  //RECEIVES THE FRAME ON "receiving_port=..."
  //IN: NONE
  //OUT: RETURNS THE RECEIVED PACKET IN RESULT PACKET, RETURNS TRUE IF MESSAGE IS VALID FALSE OTHERWISE
  bool receiveFrame(Packet &result);


  // RECEIVES STRING THAT IS RECEIVED FROM SERIAL:
  /*
  // THE STRING THAT IS SENT TO THE SERIAL PORT MUST BE IN HEX. AND 
  // MUST HAVE A SPACE AFTER EVERY BYTE: 
  // (FOR EXAMPLE: 0f ff 1a 0b fa ff 1a 0b 1a 0b fa ff)
  //
  // COMMANDS:
  // PING DEST (SRC)  - SENDS PING PACKET TO DEST (IN HEX) (AS SRC)
  // PIPO DEST (SRC)  - SENDS PING PONG PACKET TO DEST (IN HEX) (AS SRC)
  // TROU DEST (SRC)  - SENDS TRACEROUTE PACKETS 
  // GET
  */
  // IN: PACKET TO SAFE STRING INTO
  // OUT: THE RECEIVED STRING IS LOADED INTO A PACKET
  bool receiveSerial(Packet &result);

  
  //WHENEVER THIS FUNCTION IS CALLED, IT LISTENS TO ALL THE INPUT PORTS/PORTS AN IF THERE IS
  //AN INCOMMING FRAME ANYWHERE IT CHANGES: receiving = true, receiving_port = ...
  //IN: FUNCTION CALL
  //OUT: IF RECEIVING => receiving = true
  void updatePorts(void);

  //RECEIVES THE FRAME IF A PORT IS READY TO RECEIVE
  //IN: NONE
  //OUT: RETURNS THE RECEIVED PACKET IN RESULT PACKET, RETURNS TRUE IF MESSAGE IS VALID FALSE OTHERWISE
  void receiveHandle(void);

  //SENDS IF THERE IS SOMETHING IN THE BUFFER
  //IN: NONE
  //OUT: RETURNS THE RECEIVED PACKET IN RESULT PACKET, RETURNS TRUE IF MESSAGE IS VALID FALSE OTHERWISE
  void sendHandle(void); 


  public:

  byte myMAC; 

  //CONSTRUCTOR
  //IN: POINTER OF THE LCD OBJECT, USED TO PRINT TO THE LCD SCREEN
  //OUT: <INITIALIZES THE PhysicalLayer OBJECT>
  PhysicalLayer(LiquidCrystal* _lcd);

  //DESTRUCTOR
  ~PhysicalLayer();

  //WHENEVER THIS FUNCTION IS CALLED, IT LISTENS TO ALL THE INPUT PORTS AN IF THERE IS
  //AN INCOMMING FRAME IT RECEIVES IT AND SENDS AVAILABLE FRAMES
  //IN: FUNCTION CALL
  //OUT: IF RECEIVING IT UPDATES THE LOCAL VARIABLES
  void update(void);
  
  //RETURNS TRUE IF THERE IS A MESSAGE IN THE RECEIVED BUFFER
  //IN: NONE
  //OUT: BOOL
  bool available(void);

  //RETURNS THE LAST RECEIVED PACKET
  //IN: PACKET TO STORE 
  //OUT: STORES THE LAST RECEIVED PACKET IN RESULT, RETURNS TRUE IF THERE IS ONE AVAILABLE
  bool receive(Packet &result);

  //MOVES PACKET TO THE SEND_BUFFER
  //IN: PACKET TO BE SENT 
  //OUT: SENDS PACKET, RETURNS TRUE IF THAT HAS SUCCEEDED
  bool send(Packet &result); 

  //RETURNS TRUE IF THE PORT IS ACTIVE
  //IN: PORT NUMBER BETWEEN 1 - 5 
  //OUT: RETURNS TRUE IF THE PORT IS ACTIVE
  bool portActive(byte port);

  //RETURNS 0 IF NO PORT HAS CHANGED, RETURNS THE PORT NUMBER OTHERWISE
  //IN: NONE
  //OUT: RETURNS THE PORT WHICH HAS CHANGED ITS STATE, 0 OTHERWISE
  byte portChanged(void); 

};
