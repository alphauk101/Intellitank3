/***************************
Intelli Tank 3

****************************/
//Important definitions
#include "matrix7219.h"
#include "lighting.h"

#define CSPIN 2
#define DINPIN 7
#define CLKPIN 8
#define WHITELEDPIN 10
#define BLUELEDPIN 11

Matrix matrix;//Display driver
Lighting lighting;//LED lighting driver.


void setup() {
  Serial.begin(9600);
  //Get the screen ready this will block until screen until demo is done.
  matrix.initDisplay(DINPIN,CSPIN,CLKPIN);
  ///Init our led driver and bring lights to a suitable state
  lighting.init(WHITELEDPIN,BLUELEDPIN);
  

}
int a = 0;
void loop() {
  matrix.displayNumber(1);
  lighting.applyMode(MODE_FULL_DAY);
  
  delay(10000);
  matrix.displayNumber(2);
  lighting.applyMode(MODE_HALF_DAY);
  
  delay(10000);
  matrix.displayNumber(3);
  lighting.applyMode(MODE_NIGHT);

  delay(10000);
  matrix.displayNumber(4);
  lighting.applyMode(MODE_STANDBY);
  delay(10000);
}



