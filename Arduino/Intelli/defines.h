#ifndef __DEFINES__
#define __DEFINES__
/*******************
Project wide definitions
********************/
//Timers
#include <arduino.h>
#define TIMER_1 1
#define TIMER_2 2
#define TIMER_3 3

#define DAYMODE_LENGTH 20000UL //1 minute at the moment
#define DAWNMODE_LENGTH 10000UL
#define NIGHTMODE_LENGTH 10000UL
#define STANDBY_LENGTH 10000UL

#define MODE_FULL_DAY 1 //Full lighting usually after a PIR trigger
#define MODE_HALF_DAY 2 //A Slightly less intense day
#define MODE_NIGHT 3  //White LEDs of just to give a nice blue
#define MODE_STANDBY 4 //Both Blue and White are on but in a very dim mode (possibly near off)



#endif
