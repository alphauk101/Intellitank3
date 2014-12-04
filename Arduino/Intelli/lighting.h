#ifndef __LIGHTING__
#define __LIGHTING__

#include <arduino.h>
#include "defines.h"

#define MAX_LIGHT 240 //Maximum light level we shall considered "on"
#define HALF_LIGHT 30 //This level which represents our "dawn" mode.
#define MIN_LIGHT 5 //The level we which shall go down to before shutting of completely
#define FADE_TIME 10 //Ms of the time in steps to fade up or down
//The modes that the lights will operate in


class Lighting
{
  public:
    void init(int , int );
    void applyMode(int );
    int getCurrentMode(void);//Gets the current lighting mode.
  private:
    void setSingleLightLevel(int , int );
    void setBothLightLevels(int );
    void setValueToLight(int , int );
    int getLightLevel(int );
  protected:
};

#endif
