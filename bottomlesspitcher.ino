/*
* Copyright (c) 2014 FirstBuild
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:

* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.

* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

/**********************************************************
This is example code for using the Adafruit liquid flow meters. 

Tested and works great with the Adafruit plastic and brass meters
    ------> http://www.adafruit.com/products/828
    ------> http://www.adafruit.com/products/833

Connect the red wire to +5V, 
the black wire to common ground 
and the yellow sensor wire to pin #2

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above must be included in any redistribution
**********************************************************/

#include <FiniteStateMachine.h>
//#include <SimpleTimer.h>

#define MAX_FILL_TIME 180000
 
//state machine setup 
State stateFilling = State(stateFillingEnter, stateFillingUpdate, stateFillingExit);
State stateComplete = State(stateCompleteEnter, stateCompleteUpdate, stateCompleteExit);
State stateStandby = State(stateStandbyEnter, stateStandbyUpdate, stateStandbyExit);
FSM bpStateMachine = FSM(stateComplete);     //initialize state machine, start in state: On
//SimpleTimer timerFillExpire;
//int tickEvent;

static const uint8_t FILLREED = 1;
static const uint8_t RELAYOUT = 2;
static const uint8_t PROXREED  = 4;
static const uint8_t NUMBER_OF_STATES = 3; //how many states are we cycling through?
static const uint8_t FLOWSENSORPIN  = 0;
static const uint8_t PITCHER_CONNECTED = LOW;
static const uint8_t PITCHER_AWAY =  HIGH;
static const uint8_t PITCHER_FILLING  = HIGH;
static const uint8_t PITCHER_FULL  = LOW;
static const uint16_t WAIT_TIME_BETWEEEN_FILLS = 5000;

unsigned long start_filling_millis = 0;


// count how many pulses!
volatile uint16_t pulses = 0;
// track the state of the pulse pin
uint16_t lastpulses = 0;

volatile uint8_t lastflowpinstate;
// you can try to keep time of how long it is between pulses
volatile uint32_t lastflowratetimer = 0;
// and use that to calculate a flow rate
volatile float flowrate;
// Interrupt is called once a millisecond, looks for any pulses from the sensor!

void setup() {
  //Serial.begin(9600); 
  pinMode(FILLREED, INPUT_PULLUP);
  pinMode(PROXREED, INPUT_PULLUP);
  pinMode(RELAYOUT, OUTPUT);
  pinMode(FLOWSENSORPIN, INPUT);
  digitalWrite(FLOWSENSORPIN, HIGH);
  lastflowpinstate = digitalRead(FLOWSENSORPIN);
  
  useInterrupt(true);
  
}

void loop(){
  
  bpStateMachine.update();
}
 
//------------------------------------------
// FILLING
//------------------------------------------
void stateFillingEnter()
{ 
  delay(WAIT_TIME_BETWEEEN_FILLS);
  digitalWrite(RELAYOUT, HIGH);
  pulses = 0;
  lastpulses = 0;
  start_filling_millis = millis();
  //timerFillExpire.restartTimer(tickEvent);
  //tickEvent = timerFillExpire.after(MAX_FILL_TIME, fillTimerExpire); 
  //tickEvent = timerFillExpire.setTimeout(90 * 1000, fillTimerExpire);
}

//void fillTimerExpire()
//{
//  bpStateMachine.transitionTo(stateComplete);
//}

void stateFillingUpdate()
{ 
  //timerFillExpire.run();
  
  unsigned long current_millis = millis();
  
  //timerFillExpire.update();
  if (PITCHER_AWAY==digitalRead(PROXREED))
  {
    //if the proximity switch is not set, bail
    bpStateMachine.transitionTo(stateComplete); 
  }
  else if (PITCHER_FULL==digitalRead(FILLREED))
  {
    bpStateMachine.transitionTo(stateComplete);   
  }
  else if (current_millis - start_filling_millis > MAX_FILL_TIME)
  {
    bpStateMachine.transitionTo(stateComplete); 
  }
  else
  {
    if (lastpulses != pulses) 
    {
      // if a plastic sensor use the following calculation
      // Sensor Frequency (Hz) = 7.5 * Q (Liters/min)
      // Liters = Q * time elapsed (seconds) / 60 (seconds/minute)
      // Liters = (Frequency (Pulses/second) / 7.5) * time elapsed (seconds) / 60
      // Liters = Pulses / (7.5 * 60)
      //todo: cleanup
      lastpulses = pulses;
      float ounces = pulses/16;
      //ounces /= 16;
      //liters /= 60.0;
      //Serial.print(ounces); Serial.println(" ounces");
      
      if (ounces >= 100)
      {
        //Serial.println("too many ounces!!");  
        bpStateMachine.transitionTo(stateComplete);
      }
    }
    delay(100);
  }
}

void stateFillingExit()
{ 
  digitalWrite(RELAYOUT, LOW);
  //timerFillExpire.deleteTimer(tickEvent);
}


//------------------------------------------
// COMPLETE
//------------------------------------------
void stateCompleteEnter()
{ 
}

void stateCompleteUpdate()
{ 
  //if pitcher is taken away and the pitcher is full then go to standby 
  //waiting for the pitcher to return
  if (PITCHER_AWAY==digitalRead(PROXREED))// && PITCHER_FULL==digitalRead(FILLREED))
  {
    bpStateMachine.transitionTo(stateStandby);    
  }
}

void stateCompleteExit()
{ 
}


//------------------------------------------
// STANDBY
//------------------------------------------
void stateStandbyEnter()
{ 
}

void stateStandbyUpdate()
{ 
  //when pitcher returns go to the fill state
  if (PITCHER_CONNECTED==digitalRead(PROXREED))
  {
    bpStateMachine.transitionTo(stateFilling);
  }
}  

void stateStandbyExit()
{ 
}

SIGNAL(TIMER0_COMPA_vect) 
{
  uint8_t x = digitalRead(FLOWSENSORPIN);
  
  if (x == lastflowpinstate) 
  {
    lastflowratetimer++;
    return; // nothing changed!
  }
  
  if (x == HIGH) 
  {
    //low to high transition!
    pulses++;
  }
  
  lastflowpinstate = x;
  flowrate = 1000.0;
  flowrate /= lastflowratetimer;  // in hertz
  lastflowratetimer = 0;
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK |= _BV(OCIE0A);
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK &= ~_BV(OCIE0A);
  }
}



