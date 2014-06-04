
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

const uint8_t STANDBY_AY = 10000;

const int FILLREED = 2;
const int RELAYOUT = 3;
const int PROXREED  = 4;
const byte NUMBER_OF_STATES = 3; //how many states are we cycling through?
 
State stateFilling = State(stateFillingEnter, stateFillingUpdate, stateFillingExit);
State stateComplete = State(stateCompleteEnter, stateCompleteUpdate, stateCompleteExit);
State stateStandby = State(stateStandbyEnter, stateStandbyUpdate, stateStandbyExit);
 
FSM bpStateMachine = FSM(stateComplete);     //initialize state machine, start in state: On

#define FLOWSENSORPIN 7

#define PITCHER_CONNECTED LOW
#define PITCHER_AWAY HIGH
#define PITCHER_FILLING HIGH
#define PITCHER_FULL LOW

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
  //Serial.println("state filling enter");
  delay(5000);
  digitalWrite(RELAYOUT, HIGH);
}

void stateFillingUpdate()
{ 
  
  if (PITCHER_AWAY==digitalRead(PROXREED))
  {
    //if the proximity switch is not set, bail
    //Serial.println("proximity switch deactivated");
    bpStateMachine.transitionTo(stateComplete); 
  }
  else if (PITCHER_FULL==digitalRead(FILLREED))
  {
    //Serial.println("fill switch activated");  
    bpStateMachine.transitionTo(stateComplete);   
  }
  //todo: global timeout
  else
  {
    //Serial.println(".");
    if (lastpulses != pulses) 
    {
      //Serial.print("Freq: "); Serial.println(flowrate);
      //Serial.print("Pulses: "); Serial.println(pulses, DEC);
      
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
      //lcd.setCursor(0, 1);
      //lcd.print(liters); lcd.print(" Liters        ");
    }
  
    delay(100);
  }
}

void stateFillingExit()
{ 
  //Serial.println("\nstate filling exit");
  digitalWrite(RELAYOUT, LOW);
}


//------------------------------------------
// COMPLETE
//------------------------------------------
void stateCompleteEnter()
{ 
//  Serial.print("state complete enter\t");
//  Serial.print(digitalRead(PROXREED));
//  Serial.print(digitalRead(FILLREED));
//  Serial.println(LOW);
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
  //Serial.println("state complete exit");
}


//------------------------------------------
// STANDBY
//------------------------------------------
void stateStandbyEnter()
{ 
  //Serial.println("state standby enter"); 
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
  //Serial.println("state standby exit"); 
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
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
  }
}



