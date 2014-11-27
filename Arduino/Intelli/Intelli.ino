/***************************
Intelli Tank 3

****************************/
//Important definitions
#include "matrix7219.h"

#define CSPIN 2
#define DINPIN 7
#define CLKPIN 8

Matrix matrix;//Display driver


void setup() {
  
  matrix.initDisplay(DINPIN,CSPIN,CLKPIN);


}
int a = 0;
void loop() {
  // put your main code here, to run repeatedly:
  delay(200);
  
  //matrix.displayNumber(a);
  a++;
  
  if(a > 9) a=0;
}



