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

#define DHTLIB_OK		 0
#define DHTLIB_ERROR_CHECKSUM	-1
#define DHTLIB_ERROR_TIMEOUT	-2
#define DHTLIB_INVALID_VALUE	-999

#define DAYMODE_LENGTH (unsigned long) 1800000//10 = minutes
#define DAWNMODE_LENGTH (unsigned long) 300000
#define NIGHTMODE_LENGTH (unsigned long) 1800000

//Display toggling defines
#define DISPLAY_TOGGLE_PERIOD (unsigned long) 60000 //Every 30secs the display changes


#define MODE_FULL_DAY 1 //Full lighting usually after a PIR trigger
#define MODE_HALF_DAY 2 //A Slightly less intense day
#define MODE_NIGHT 3  //White LEDs of just to give a nice blue
#define MODE_STANDBY 4 //Both Blue and White are on but in a very dim mode (possibly near off)




#endif
