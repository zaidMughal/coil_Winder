/* Header.h
  This file includes the headers which may need to change for different default/working settings*/
#ifndef _HEADER_h
#define _HEADER_h
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif



#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

#define STEPS_PER_REVOLUTION 200.00
#define PITCH 1.50  //mm
#define DEFAULT_NUM_TURNS 500
#define DEFAULT_WIRE_THICKNESS 0.35   //mm
#define DEFAULT_WIDTH_TO_ALTER 30     //mm

#define ROWS 4 //four rows
#define COLS 4 //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'B'},
  {'.', '0', '#', 'D'}
};
byte rowPins[ROWS] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6, 7}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );     //Initialize the keypad with specfied pins
LiquidCrystal_I2C lcd(0x3F, 16, 2);      // set the LCD address to 0x27 for a 16 chars and 2 line display



char key = 0;
int num_turns = 0;
float wire_thickness = 0;
float Width_to_alter = 0;



#endif
