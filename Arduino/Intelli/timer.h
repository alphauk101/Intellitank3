/*****************************************
 * Timer class allows use of 1-3 timers can use a 
 * callback but timer must be nudged in order for
 * this is function properly.
 * Author L. Williams 
 *********************************************/
#ifndef __TIMER__
#define __TIMER__
#include <arduino.h>
#include "defines.h"


class Timer
{
public:
  void init(int , unsigned long);//Inits the timer must pass which timer to init
  void startTimer(int);//Starts timer, depending which timer is passed
  void stopTimer(int);
  boolean isTimerRunning(int);
  void setCallBack(void (*cb)(int));
  void nudge(void);
  void resetAllTimers(void);//Allows all timers to be set back to 0 count they are not started
private:


};

#endif


