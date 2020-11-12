#pragma once
#include <arduino.h>
#include "Buffer.h"
#include "Definitions.h"
#include "Functions.h"
#include "PhysicalLayer.h"
#include "Packet.h"
#include <LiquidCrystal.h>


// INITIALIZES THE IO-PINS CORRECTLY
// IN: NONE
// OUT: NONE
void initialize_pins(void);

// CONVERTS CHARACTERS IN HEX TO INTEGERS
// IN: CHAR 0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F (FOR EXAMPLE 'F' or 'f' => 15)
// OUT: THIS CHAR AS AN INTEGER OR ZERO IF CHAR IS INVALID
int hexCharToInt(char c);
