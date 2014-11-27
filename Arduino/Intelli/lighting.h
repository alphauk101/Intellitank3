#ifndef __LIGHTING__
#define __LIGHTING__

#include <arduino.h>

#define MAX_LIGHT 220 //Maximum light level we shall considered "on"
#define HALF_LIGHT 30 //This level which represents our "dawn" mode.
#define MIN_LIGHT 5 //The level we which shall go down to before shutting of completely
#define FADE_TIME 10 //Ms of the time in steps to fade up or down
//The modes that the lights will operate in
#define MODE_FULL_DAY 1 //Full lighting usually after a PIR trigger
#define MODE_HALF_DAY 2 //A Slightly less intense day
#define MODE_NIGHT 3  //White LEDs of just to give a nice blue
#define MODE_STANDBY 4 //Both Blue and White are on but in a very dim mode (possibly near off)


class Lighting
{
  public:
    void init(int , int );
    void applyMode(int );

  private:
    void setSingleLightLevel(int , int );
    void setBothLightLevels(int );
    void setValueToLight(int , int );
    int getLightLevel(int );
  protected:
};

#endif
