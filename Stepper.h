// Stepper.h

#ifndef _STEPPER_h
#define _STEPPER_h
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#define MAXSPEED 1100//The more the value, the lesser tyhe speed. Define least delay.This will be used by delayMicroseconds() and by goto_point() function
#define SPEED_MULTIPLYCATION_FACTOR 3.00
class StepperClass
{
    int motor_pin_1, motor_pin_2, motor_pin_3, motor_pin_4;
    int speeds = MAXSPEED;//Maximum imit in which stepper could move freely and speed for delay
    unsigned long limit;
    unsigned long steps;//current steps
  public:
    StepperClass(int pin1, int pin2, int pin3, int pin4, long l);//Constructer
    void increaseSteps(unsigned int s);
    void increaseSteps_withoutDelay(unsigned int s);//Normally increase/decrease functions wait till motor turns. But this function does not
    void decreaseSteps(int s);
    void decreaseSteps_withoutDelay(int s);
    void all_outputsLow(void);//It is to avoid power loss
    unsigned long get_limit(void);
    unsigned long getSteps();
    void setSpeeds(int s);
    void gotostep(int s);
    void forcedResetSteps(void);
    friend void goto_point(unsigned long x, unsigned long y, StepperClass&, StepperClass&, bool should_we_take_speed_input); //Go fromm current position to designated position in continuous motion
    ///////////////THIS FUNCTION AUTOMETICALLY TAKES SPEED INPUT FROM PIN A0
};

extern StepperClass Stepper;

#endif
