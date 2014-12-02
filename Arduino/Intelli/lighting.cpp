#include "lighting.h"


int WHITE_LIGHT_PIN = 0;
int BLUE_LIGHT_PIN = 0;
int CURRENT_MODE;

int WHITE_CURRENT_LVL = 0;
int BLUE_CURRENT_LVL = 0;

void Lighting::init(int whtPIN, int bluePIN)
{
  WHITE_LIGHT_PIN = whtPIN;
  BLUE_LIGHT_PIN = bluePIN;
  CURRENT_MODE = MODE_STANDBY;//Set our current
  //Now we need to set the lights to the default on level
}

int Lighting::getCurrentMode()
{
  return CURRENT_MODE;
}

/*This changes the current light level to the new one in a fading manner*/
void Lighting::setSingleLightLevel(int whatLight, int Level)
{
  int currentLvl = 0;
  currentLvl = getLightLevel(whatLight);//First get the current light level

  //Now we need to change the light to the appropriate level;
  if (Level > currentLvl)
  {
    //We are going brighter.
    for (int a = currentLvl; a <= Level; a++)
    {
      setValueToLight(whatLight, a);
      delay(FADE_TIME);
    }
  } else {
    //We are going dimmer.
    for (int a = currentLvl; a >= Level; a--)
    {
      setValueToLight(whatLight, a);
      delay(FADE_TIME);
    }
  }
  //We have changed our light setting
}

/*Changes both lights to the given levels*/
void Lighting::setBothLightLevels(int changeLvl)
{
  //As I dont mind changing the lights one at a time (gives a colour fading action) we can use the above function
  setSingleLightLevel(WHITE_LIGHT_PIN, changeLvl);
  //White goes first as it looks cool when the blue comes into play
  setSingleLightLevel(BLUE_LIGHT_PIN, changeLvl);
}

/*This function will take the lights and put them into a standard mode.*/
void Lighting::applyMode(int mode)
{
  if (CURRENT_MODE != mode)
  {
    CURRENT_MODE = mode;//Set the mode
    //First we need to work out which mode were trying to put ourselves in
    switch (mode)
    {
      case MODE_FULL_DAY:

        //Simple both white and blue lights full on!
        setBothLightLevels(MAX_LIGHT);//Done!
        break;
      case MODE_HALF_DAY:

        setBothLightLevels(HALF_LIGHT);//This should be our dawn mode.
        break;
      case MODE_NIGHT:

        //Here we are into night mode so our blue light should be main
        setSingleLightLevel(BLUE_LIGHT_PIN, MAX_LIGHT);//Set the blue to full
        setSingleLightLevel(WHITE_LIGHT_PIN, MIN_LIGHT);//Set the white to just a very slight tinge.
        break;
      case MODE_STANDBY:

        setBothLightLevels(MIN_LIGHT);//Set both lights to barely on.
        break;
      default:
        //We really dont want to make any changes here or we might end up with some strange effects.
        break;
    }
  }
}

/*Allows us to keep track of the values were setting in our lights*/
inline void Lighting::setValueToLight(int whatLight, int level)
{
  if (whatLight == WHITE_LIGHT_PIN)
  {
    WHITE_CURRENT_LVL = level;
    analogWrite(WHITE_LIGHT_PIN, level);
  } else if (whatLight == BLUE_LIGHT_PIN)
  {
    BLUE_CURRENT_LVL = level;
    analogWrite(BLUE_LIGHT_PIN, level);
  }
}

inline int Lighting::getLightLevel(int light)
{
  if (light == WHITE_LIGHT_PIN)
  {
    return WHITE_CURRENT_LVL;
  } else {
    return BLUE_CURRENT_LVL;
  }

}
