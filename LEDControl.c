#include <Adafruit_NeoPixel.h>

#define PIN 13
#define LEDCOUNT 299

int r, g, b, LEDindex;

Adafruit_NeoPixel strip(LEDCOUNT, PIN, NEO_GRB + NEO_KHZ800);

const byte numChars = 33;
char receivedChars[numChars];

bool newCommand = false;

//<1>

struct colorid {
  //char
  int r;
  int g;
  int b;
  int len;
};

typedef struct colorid colorID;

struct item {
  colorID colors[3];
  //num
  int t;
  int active;
};

typedef struct item Item;

colorID c1 = {.r=255, .g=255, .b=255, .len=1};
colorID c2 = {.r=255, .g=255, .b=255, .len=1};
colorID c3 = {.r=255, .g=255, .b=255, .len=1};
Item testItem = {.colors = {c1, c2, c3}, .t = 1000, .active=1};

int lightLen = 3;

void setup() {
  Serial.begin(9600);
  Serial.println("<Ready For Command>");

  strip.begin();
  strip.show();

  LEDindex = 5;
}

void loop() {
  if(testItem.active == 1){
    LEDCycle();
    delay(testItem.t);
  }

  getCommand();
  if (newCommand == true) {
    parseCommand(receivedChars, &testItem);
  }

 //  LEDCycle();

}

void LEDCycle() {

  LEDindex = LEDindex + 1;

  strip.setPixelColor(LEDindex - 1, 0, 0, 0);

  int curIndex = 0;

  for(int j = 0; j < 3; j++){
    for(int i = 0; i < testItem.colors[j].len; i++){
      strip.setPixelColor(LEDindex + curIndex, testItem.colors[j].r, testItem.colors[j].g, testItem.colors[j].b);
      curIndex++;
    }
  }
  
  if (LEDindex > LEDCOUNT) {
    LEDindex = 3;
  }

  strip.show();
}

void parseCommand(char commandRaw[numChars], item * changeItem) {
  Serial.println(commandRaw);
  Serial.println(receivedChars);


  char rawValue[3];
  char colorIndex[2];

  int value = 0;
  int colorIndexValue = 0;

  // If the second char is not T that means we are talking about a color
  if (commandRaw[1] == 't') {
    Serial.println("it was time");
    //<1tFFF> <1t000>
    rawValue[0] = commandRaw[2];
    rawValue[1] = commandRaw[3];
    rawValue[2] = commandRaw[4];
    rawValue[3] = '\0';
  } else if (commandRaw[1] == 's') {
    //<1s1> or <1s0>
    Serial.println("it was start/stop");
    rawValue[0] = commandRaw[2];
    rawValue[1] = '\0';
  } else if(commandRaw[2] == 'l'){
    colorIndex[0] = commandRaw[1];
    colorIndex[1] = '\0';
    rawValue[0] = commandRaw[3];
    rawValue[1] = commandRaw[4];
    rawValue[2] = '\0';
   
  }else {
    Serial.println("it was a color");
    //<11rFF> or <13b0F>
    colorIndex[0] = commandRaw[1];
    colorIndex[1] = '\0';
    rawValue[0] = commandRaw[3];
    rawValue[1] = commandRaw[4];
    rawValue[2] = '\0';
  }

  value = (int)strtol(rawValue, NULL, 16);

  //Switch statements to change color values, easier if they were in array.
  if (commandRaw[1] == 't') {

    changeItem->t = value;

  } else if (commandRaw[1] == 's') {
    Serial.println("changing active");
    
    changeItem->active = value;
    Serial.println(commandRaw[2]);
    Serial.println(testItem.active);

  } else {

    colorIndexValue = (int)strtol(colorIndex, NULL, 16);

    switch (commandRaw[2]) {
      case 'r':
        changeItem->colors[colorIndexValue].r = value;
        break;
      case 'g':
        changeItem->colors[colorIndexValue].g = value;
        break;
      case 'b':
        changeItem->colors[colorIndexValue].b = value;
        break;
      case 'l':
        changeItem->colors[colorIndexValue].len = value;
        break;
    }
  }

  //Debug Stuff
//  Serial.println(changeItem->colors[1].r);
//  Serial.println(changeItem->active);
  Serial.println(testItem.t);

  newCommand = false;
}

void getCommand() {
  static boolean recvInProgress = false;
  static byte recvIndex = 0;

  char startMark = '<';
  char endMark = '>';
  char recv;


  while (Serial.available() > 0 && newCommand == false) {
    recv = Serial.read();

    if (recvInProgress == true) {
      if (recv != endMark) {

        receivedChars[recvIndex] = recv;
        recvIndex++;

        //May have to bump up numChars
        if (recvIndex >= numChars) {
          recvIndex = numChars - 1;
        }

      }
      else {
        receivedChars[recvIndex] = '\0'; // terminate the string

        recvInProgress = false;
        recvIndex = 0;
        newCommand = true;
      }
    }

    else if (recv == startMark) {
      recvInProgress = true;
    }
  }
}
