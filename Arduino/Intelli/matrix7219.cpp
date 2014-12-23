/********************************************************************
 * 	created:	24:9:2014   8:54
 * 	file base:	matrix7219
 * 	file ext:	cpp
 * 	author:		Lee Williams
 * 	purpose:        To create a simple class to drive the 7219 LED matrix display chip.
 * chip, for arduino.
 *********************************************************************/
#include "matrix7219.h"

//PINS
int DINpin, CSpin, CLKpin;
int intensity, decodeMode;

byte displayBuf[8];//Holds our display characters
//Number definitions
byte zero = 0x7e;
byte one = 0x30;
byte two = 0x6d;
byte three = 0x79;
byte four = 0x33;
byte five = 0x5b;
byte six = 0x5f;
byte seven = 0x70;
byte eight = 0x7f;
byte nine = 0x7b;
byte numberMatrix[10] = {zero, one, two, three, four, five, six, seven, eight, nine};
byte i = 0x06;
byte n = 0x15;
byte t = 0x0f;
byte e = 0x4f;
byte l = 0x0e;
byte c = 0x0D;
byte dash = 0x01;
byte period = 0x80;
byte H = 0x37;


void Matrix::demo()
{
  for (int a = 0; a < 8; a++) {
    shootingStars();
  }
  clearBuffer();
  displayBuf[7] = i;
  displayBuf[6] = n;
  displayBuf[5] = t;
  displayBuf[4] = e;
  displayBuf[3] = l;
  displayBuf[2] = l;
  displayBuf[1] = i;
  displayBuf[0] = numberMatrix[3];
  updateDisplay(displayBuf);
  delay(1000);
}

byte fltTempArray[3];
void Matrix::dispWaterTemp(float temp)
{
  //Reset the array
  fltTempArray[0] = 0x00;
  fltTempArray[1] = 0x00;
  fltTempArray[2] = 0x00;

  //Convert our temperature to a array
  int expo = convertFloatToArray(temp, fltTempArray);

  //We should now have an array to display
  clearBuffer();
  displayBuf[7] = dash;
  displayBuf[6] = dash;
  displayBuf[5] = numberMatrix[fltTempArray[0]];
  //We need add the full stop

  byte dispStopNum = numberMatrix[fltTempArray[1]];
  dispStopNum |= period;

  displayBuf[4] = dispStopNum;
  displayBuf[3] = numberMatrix[fltTempArray[2]];
  displayBuf[2] = c;
  displayBuf[1] = dash;
  displayBuf[0] = dash;
  updateDisplay(displayBuf);
}

int Matrix::convertFloatToArray(float temp, byte* fltArray)
{
  byte a;
  byte b;
  byte c;
  int number = temp * 10; //removes the decimal point
  if (number > 99)
  {
    //we know there are three elements
    a = number / 100;
    b = (number / 10) % 10;
    c = number % 10;

    *fltArray = a;
    *(fltArray + 1) = b;
    *(fltArray + 2) = c;

    return 3;
  } else {
    a = (number / 10) % 10;
    b = number % 10;
    *fltArray = a;
    *(fltArray + 1) = b;
    *(fltArray + 2) = c;
    return 2;
  }
}

void Matrix::showMode()
{
  //I need to build on this but its just demo
  clearBuffer();
  updateDisplay(displayBuf);

  for (int a = 0 ; a < 8; a++)
  {
    clearBuffer();
    displayBuf[a] = dash;
    updateDisplay(displayBuf);
    delay(50);
  }

  clearBuffer();
  updateDisplay(displayBuf);
}

void Matrix::shootingStars()
{
  byte top = 0x40, middle = 0x01, bottom = 0x08; //Dashes on the screen
  for (int a = 0; a < 24; a++)
  {
    clearBuffer();
    if (a < 8)
    {
      displayBuf[a] = top;
    } else if ((a > 7) && (a < 16))
    {
      displayBuf[a - 8] = middle;
    } else if (a > 15) {
      displayBuf[a - 16] = bottom;
    }
    updateDisplay(displayBuf);
    delay(10);
  }
}

void Matrix::displayNumber(int number)
{
  //I need to build on this but its just demo
  clearBuffer();
  displayBuf[0] = numberMatrix[number];
  updateDisplay(displayBuf);
  //demo();
}

void Matrix::showRoomTempAndHum(float temp, float hum)
{
  fltTempArray[0] = 0x00;
  fltTempArray[1] = 0x00;
  fltTempArray[2] = 0x00;
  clearBuffer();//makesure our buffers cleared
  if (convertFloatToArray(temp, fltTempArray) == 3)
  {
    displayBuf[7] = numberMatrix[fltTempArray[0]];
    byte dispStopNum = numberMatrix[fltTempArray[1]];
    dispStopNum |= period;
    displayBuf[6] = dispStopNum;
    displayBuf[5] = numberMatrix[fltTempArray[2]];
  } else {
    displayBuf[7] = 0x00;//Blank
    byte dispStopNum = numberMatrix[fltTempArray[0]];
    dispStopNum |= period;
    displayBuf[6] = dispStopNum;
    displayBuf[5] = numberMatrix[fltTempArray[1]];
  }
  displayBuf[4] = c;
  //Now hum
  fltTempArray[0] = 0x00;
  fltTempArray[1] = 0x00;
  fltTempArray[2] = 0x00;
  convertFloatToArray(hum, fltTempArray);
  displayBuf[2] = numberMatrix[fltTempArray[0]];
  displayBuf[1] = numberMatrix[fltTempArray[1]];
  displayBuf[0] = H;
  updateDisplay(displayBuf);
}

/**********************************************************
 * init(DIN pin, CS pin, CLK pin)
 ***********************************************************/
void Matrix::initDisplay(int DIN, int CS, int CLK) {
  //Lets make sure that our pins are in the correct state
  DINpin = DIN;
  CSpin = CS;
  CLKpin = CLK;
  //Set the direction.
  pinMode(CSpin, OUTPUT);
  pinMode(DINpin, OUTPUT);
  pinMode(CLKpin, OUTPUT);
  setClock(LOW);
  setLatch(HIGH);
  //Set our defaults
  setIntensity(DEFAULT_INTENSITY);
  setDecodeMode(DECODEMODE_NO_DECODE);

  demo();//Show the intro
}

/*********************************************************
 *This is the function to call when we want to change whats on
 * the display all the other parameters (intensity etc) are set
 * aswell.
 * Byte array is passed in col1, col2, col3, etc. fashion.
 **********************************************************/
void Matrix::updateDisplay(byte* columns) {
  //Ok lets start.
  //First latch the no op
  shift(0x00, 0x00); //No op
  //Now shift our columns
  for (int a = 0; a < 8; a++) {
    shift((a + 1), *(columns + a));
  }
  //Now shift decode mode
  shift(0x09, decodeMode);
  //Intensity
  shift(0x0A, intensity);
  //Scan limit
  shift(0x0B, 0x07); //All segs displayed
  //Shut down
  shift(0x0C, 0x01); //Normal operation
  //Test mode off
  shift(0x0F, 0x00);//Display test OFF
  //And were done.
}


//MSB is address and LSB is data
void Matrix::shift(byte MSB, byte LSB)
{
  //We will load MSB first
  setLatch(LOW);
  delayMicroseconds(1);
  for (int a = 7; a >= 0; a--) {
    delayMicroseconds(1);
    if ((MSB & (1 << a)) == (0xFF & (1 << a))) {
      //Its a hi bit
      digitalWrite(DINpin, HIGH);
    } else {
      //Its a low bit
      digitalWrite(DINpin, LOW);
    }
    //First latch our bit
    delayMicroseconds(CLOCK_PULSE_WIDTH * 2);
    setClock(HIGH);
    delayMicroseconds(CLOCK_PULSE_WIDTH);//Clock pulse.
    setClock(LOW); //Put the clk hi
    delayMicroseconds(CLOCK_PULSE_WIDTH);//Clock pulse.
  }

  //delayMicroseconds(CLOCK_PULSE_WIDTH);
  for (int a = 7; a >= 0; a--) {
    delayMicroseconds(1);
    if ((LSB & (1 << a)) == (0xFF & (1 << a))) {
      //Its a hi bit
      digitalWrite(DINpin, HIGH);
    } else {
      //Its a low bit
      digitalWrite(DINpin, LOW);
    }
    delayMicroseconds(CLOCK_PULSE_WIDTH * 2);
    setClock(HIGH);
    delayMicroseconds(CLOCK_PULSE_WIDTH);//Clock pulse.
    setClock(LOW); //Put the clk hi
    delayMicroseconds(CLOCK_PULSE_WIDTH);//Clock pulse.
  }

  setLatch(HIGH);
  delayMicroseconds(5);//This latches our data in.
  setLatch(LOW);
  delayMicroseconds(CLOCK_PULSE_WIDTH);
  setClock(HIGH); //Put the clk hi
  delayMicroseconds(CLOCK_PULSE_WIDTH);
  setClock(LOW); //Put the clk hi
}



/****************************************************
 * Enables the test mode on display all leds.
 ****************************************************/
void Matrix::testDisplay(void) {
  //Pass the relevant bytes to our shiftin
}


/****************************************************
 *Sets the intensity. Is applied the next time the display
 * is updated
 *****************************************************/
void Matrix::setIntensity(int inten) {

  if ((inten > 0) && (inten < 16)) {
    intensity = inten;
  }
  else {
    intensity = DEFAULT_INTENSITY; //Restore dafault because some out of range has been passed.
  }
}

/****************************************************
 *Sets the decode mode. Updated on next screen update.
 *****************************************************/
void Matrix::setDecodeMode(byte mode) {
  decodeMode = mode;
}

/******************************************************
 * Sets our clock line to the appropriate state.
 *******************************************************/
inline void Matrix::setClock(int state) {
  digitalWrite(CLKpin, state);
}

/******************************************************
 * Sets our clock line to the appropriate state.
 *******************************************************/
inline void Matrix::setLatch(int state) {
  digitalWrite(CSpin, state);
}

void Matrix::clearBuffer()
{
  for (int a = 0; a < 8; a++)
  {
    displayBuf[a] = 0x00;
  }
}





