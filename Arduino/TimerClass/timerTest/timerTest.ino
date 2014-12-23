#include "timer.h"


static Timer t1;

boolean a = false;
void setup()
{
  Serial.begin(9600);
  Serial.println("Started");

  t1.init(1,5000);//Init timer 1 with 5 seconds

  t1.setCallBack(timerExpired);
  t1.startTimer(1);//Start Timer one


}

void loop()
{

  t1.nudge();//We have to nudge our timers to check the time

  delay(10);
  
  if(a)
  {
    t1.init(2,10000);//Timer 2 with 10 secs
    t1.startTimer(2);//Start Timer two
    a= false;
  }
}

static void timerExpired(int whichTimer)
{
  Serial.print("TIMER FIRED  ");
  Serial.println(whichTimer);
  a = true;
}


