/***************************
Intelli Tank 3

****************************/
boolean ENABLEDB = false;//Our global for enabling debug out.

//Important definitions
#include "defines.h"
#include "matrix7219.h"
#include "lighting.h"
#include "timer.h"
#include "ds.h"
#include "DHT11.h"

#define CSPIN A2
#define DINPIN 7
#define CLKPIN 8
#define DHT11PIN 12 //This is what our temp sensor 
#define WHITELEDPIN 10
#define BLUELEDPIN 11

typedef int displayMode;
#define externalTemp 0
#define internalTemp 1

Matrix matrix;//Display driver
Lighting lighting;//LED lighting driver.
Timer timer;//Our timer class
ds waterSensor;//Water probe temperature
dht roomSensor;//room sensor.


boolean INT_TRIGGERED = false;//This allows us to disable the interrupt until weve finished the routine
displayMode mDisplayMode = externalTemp;//Set our default mode

void setup() {
  if (ENABLEDB) {
    Serial.begin(9600);
    Serial.println("Starting Intellitank3");
  }
  //Get the screen ready this will block until screen until demo is done.
  matrix.initDisplay(DINPIN, CSPIN, CLKPIN);
  ///Init our led driver and bring lights to a suitable state
  lighting.init(WHITELEDPIN, BLUELEDPIN);

  //We dont init our timers until we need them but we should set our call back
  timer.setCallBack(TimerISR);

  attachInterrupt(0, pirTriggered, RISING);//PIN 2 is this interrupt.

  if (ENABLEDB) Serial.println("Setup Complete");

  //We must put the tank into just triggered mode.
  setmode(MODE_FULL_DAY);
  //and let the looptake care of the rest

  timer.init(TIMER_2, DISPLAY_TOGGLE_PERIOD);//This tells us to chenge what were displaying
  timer.startTimer(TIMER_2);
}
float temp;
void loop() {


  
  //This is where we shall do all our
  //displaying of temp etc.
  switch (mDisplayMode)
  {
    case (externalTemp):
      //display the outdoor temp
      if(roomSensor.read11(DHT11PIN) == DHTLIB_OK)
      {
        //We have a reading.  
        float tmp = (float) roomSensor.temperature;
        float hum = (float) roomSensor.humidity;
        matrix.showRoomTempAndHum(tmp,hum);
      }
           
      break;
    case (internalTemp):
      temp = waterSensor.getTemp();
      matrix.dispWaterTemp(temp);
      //e need to disp this temp.
      
      
      break;
  }

  timer.nudge();//This is important to ensure out timer keeps turning
  delay(10);//Slow the 32Mhz of raging power a tad.
}


//This allows the setting of modes so we can run through the reoutine
void setmode(int mode)
{
  switch (mode)
  {
    case (MODE_FULL_DAY):
      //matrix.showMode();
      if (ENABLEDB) Serial.println("set mode full");
      lighting.applyMode(MODE_FULL_DAY);
      //we need to start the timer
      timer.init(TIMER_1, DAYMODE_LENGTH); //We are setting our timer for day mode.
      timer.startTimer(TIMER_1);
      INT_TRIGGERED = false;//Reset this so that the interrupt is reenabled.

      break;
    case (MODE_HALF_DAY)://Dawn mode
      //matrix.showMode();
      if (ENABLEDB) Serial.println("set mode dawn");
      lighting.applyMode(MODE_HALF_DAY);
      timer.init(1, DAWNMODE_LENGTH); //We are setting our timer for day mode.
      timer.startTimer(TIMER_1);
      break;
    case (MODE_NIGHT):
      //matrix.showMode();
      if (ENABLEDB) Serial.println("set mode night");
      lighting.applyMode(MODE_NIGHT);
      timer.init(1, NIGHTMODE_LENGTH); //We are setting our timer for day mode.
      timer.startTimer(TIMER_1);
      break;
    case (MODE_STANDBY):
      //matrix.showMode();
      if (ENABLEDB) Serial.println("set mode standy by");
      lighting.applyMode(MODE_STANDBY);
      timer.stopTimer(TIMER_1);
      //We dont use a timer now because we stay in this mode until its repeated by pir
      break;
    default:
      //do nothing
      if (ENABLEDB) Serial.println("set mode default case hit!!!!!");
      break;
  }
}


//This allows us to manage the order of the modes
int getNextMode(int current)
{
  switch (current)
  {
    case (MODE_FULL_DAY):
      return MODE_HALF_DAY;
      break;
    case (MODE_HALF_DAY)://Dawn mode
      return MODE_NIGHT;
      break;
    case (MODE_NIGHT):
      return MODE_STANDBY;
      break;
    case (MODE_STANDBY):
      return MODE_STANDBY;//We cant go any lower than this
      break;
    default:
      return MODE_STANDBY;//We cant go any lower than this
      if (ENABLEDB) Serial.println("getNextMode default case hit!!!!!");
      break;
  }
}

//Our PIR has triggered an input so reset our timers back to normal mode.
void  pirTriggered()
{
  if (ENABLEDB) Serial.println("PIR TRIGGERED");
  if (! INT_TRIGGERED) {//Only service this routine if its not currently being service.
    INT_TRIGGERED = true;
    setmode(MODE_FULL_DAY);//We are restaring our routine
  }
}

//Timer callback returns which timer was fired
static void TimerISR(int timer)
{
  int mode;
  switch (timer)
  {
    case (TIMER_1):
      if (ENABLEDB) Serial.println("TIMER 1 TRIGGERED");
      //First get the current lighting mode
      mode = lighting.getCurrentMode();
      mode = getNextMode(mode);
      setmode(mode);
      //Now we can see the next mode.
      break;
    case (TIMER_2):
      if (ENABLEDB) Serial.println("TIMER 2 TRIGGERED");
      changeDisplayMode();//Toggles the display mode
      break;
    default:
      //Do nothing
      break;
  }
}

void changeDisplayMode()
{
  if (mDisplayMode == externalTemp)
  {
    if (ENABLEDB) Serial.println("Display internal temp");
    mDisplayMode = internalTemp;
  } else {
    if (ENABLEDB) Serial.println("Display external temp");
    mDisplayMode = externalTemp;
  }
  timer.startTimer(TIMER_2);//restart the timer
}


