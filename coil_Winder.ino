#include "Stepper.h"
#include "Headers.h"

extern StepperClass Primery;
extern StepperClass Secondry;

void setup() {
  for (int n = 0; n <= 7; n++)
    pinMode(n, INPUT);
  for (int n = 8; n <= 13; n++)
    pinMode(n, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);

  digitalWrite(13, HIGH);          //Use it as Vcc for lcd
  lcd.init();                      // initialize the lcd     //SDA to A4  &   SCL to A5
  lcd.backlight();                 //Turn on the led on back of lcd
}




void loop() {
  setData();                            //Input variables (Default are hard coded)

  StepperClass Primery(9, 10, 11, 12, num_turns * STEPS_PER_REVOLUTION);   //Objects created and pins initialized
  StepperClass Secondry(8, A1, A2, A3, STEPS_PER_REVOLUTION * (Width_to_alter / PITCH));

start:   wait_before_start();           //Start

  wind(Primery, Secondry);              //Startig menu and wind process
  End(Secondry);                        //Ending menu and other tasks

  lcd.clear();
  lcd.print("A->Restart");
  lcd.setCursor(0, 1);
  lcd.print("B->Reset");

  key = 0;
  while (!key)
    key = keypad.getKey();
  if (key == 'A') {
    Primery.forcedResetSteps();
    Secondry.forcedResetSteps();
    goto start;
  }
  else if (key == 'B') {
    loop();//Reset or start from start
  }
}

void End(StepperClass& Secondry) {
  lcd.clear();
  lcd.println("Press any key to");
  lcd.setCursor(0, 1);
  lcd.println("end");
  unsigned long previousMillis = millis();
  key = 0;
  while (!key && millis() - previousMillis < 10000)    //10000milliseconds = 10seconds
    key = keypad.getKey();
  lcd.clear();
  Secondry.decreaseSteps(Secondry.getSteps());   //Go back to start position
}


void wind(StepperClass& Primery, StepperClass& Secondry) {
  lcd.clear();
  lcd.print("Currently");
  lcd.setCursor(0, 1);
  lcd.print("       Winding");
  const long PrimeryStepsPerWidthToAlter = (int)(STEPS_PER_REVOLUTION * Width_to_alter / wire_thickness);
  do {
    goto_point(  Secondry.get_limit()  ,  Primery.getSteps() + PrimeryStepsPerWidthToAlter ,  Secondry  ,  Primery  , 1 ); //Secondry in forward direction
    delay(500);       //Give a break times for wires to cool off
    //Reduce it if wires can bear heat
    if (Primery.getSteps() >= Primery.get_limit()) break; // Break if number of turns is complete

    goto_point(  0  ,  Primery.getSteps() + PrimeryStepsPerWidthToAlter  ,  Secondry  ,  Primery  , 1 ); //Secondry in reverse direction
    delay(500);       //Give a break times for wires to cool off
    //Reduce it if wires can bear heat
  } while (Primery.getSteps() < Primery.get_limit());
}

void setData(void) {
  lcd.clear();
  lcd.print("A->Default Input");
  lcd.setCursor(0, 1);
  lcd.print("B->Custom Input");

  key = 0;
  while (!key)
    key = keypad.getKey();
  if (key == 'A') {                       //EEPROM storage can also be used
    num_turns = DEFAULT_NUM_TURNS;
    wire_thickness = DEFAULT_WIRE_THICKNESS;
    Width_to_alter = DEFAULT_WIDTH_TO_ALTER;
  }
  else if (key == 'B') {
    num_turns = (int)input("num turns");
    wire_thickness = input("wire dia");   //Wire diameter
    Width_to_alter = input("width");
  }
  else {
    key = 0;
    setData();//Take input again
  }
}

float input(String str) {
  float number = 0.0000; //It stores our input
  lcd.clear();
  lcd.print("Enter ");
  lcd.print(str);
  lcd.setCursor(0, 1);

  int multiple = 0;
  bool fractionality = 0;
  do {
    key = 0;
    while (!key)
      key = keypad.getKey();

    lcd.print(key);
    delay(100);//Debounce delay
    if (key == '.')
    {
      fractionality = 1;
      multiple = 1;
    }
    else if (key != '#' && key != 'A' && key != 'B' && key != 'C' && key != 'D')
    {
      number *= pow(10.0, !fractionality);
      number += (float)(key - 48) * pow(10.0, -1.0 * multiple * fractionality);
      multiple++;
    }
    else if (key == 'D')
    {
      number *= pow(10.0, !fractionality);
      number += (float)(key - 48) * pow(10.0, -1.0 * multiple * fractionality);
      multiple--;
    }
    else if (key == 'C')
    { loop();//Restart all program
    }
  } while (key != '#');

  return number;
}

void wait_before_start() {
  lcd.clear();
  lcd.print("Press # to start");

  key = 0;
  while (!key)
    key = keypad.getKey();

  if (key != '#') {
    lcd.setCursor(0, 1);
    lcd.print("Invalid Input");
    delay(200);
    wait_before_start();
  }
}
