#include <Adafruit_NeoPixel.h>

#define PIN 13
#define LEDCOUNT 299


int LEDindex;

const byte numChars = 33;
char receivedChars[numChars];

bool newCommand = false;

//<1>

struct colorid {
  //char
  int rgb[3];
  int len;
};

typedef struct colorid colorID;

struct item {
  colorID colors[3];
  //num
  int t;
};

typedef struct item Item;

struct ledStrip{

    int active;
    int itemCount;
    Item * items;
};

typedef struct ledStrip LEDStrip;
//Item testItem = {.colors = {c1, c2, c3}, .t = 1000, .active=1};



Adafruit_NeoPixel strip(LEDCOUNT, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  Serial.println("<Ready For Command>");

  strip.begin();
  strip.show();

  LEDStrip testStrip = { .active = 1, .itemCount = 2};

}

void loop() {
//  if(testItem.active == 1){
//    LEDCycle();
//    delay(testItem.t);
//  }
//
//  getCommand();
//  if (newCommand == true) {
//    parseCommand(receivedChars, &testItem);
//  }

}

void Strip_init(LEDStrip * newStrip, int itemCount){

    newStrip->active = 1;

    newStrip->itemCount = itemCount;

    newStrip->items = (Item *)malloc(sizeof(Item) * itemCount);

    for(int i = 0; i < itemCount; i = i + 1){

        Item newItem;
        Item_init(&newItem, 3);
        newStrip->items[i] = newItem;

    }

}

void Item_init(Item * newItem, int colorCount){



}

void Color_init(colorID ** newColor, bool randomColor){

}
//
//void LEDCycle(Item * ColorItem) {
//
//  LEDindex = LEDindex + 1;
//
//  strip.setPixelColor(LEDindex - 1, 0, 0, 0);
//
//  int curIndex = 0;
//
//  for(int j = 0; j < 3; j++){
//    for(int i = 0; i < *testItem.colors[j].len; i++){
//      strip.setPixelColor(LEDindex + curIndex, *testItem.colors[j].r, *testItem.colors[j].g, *testItem.colors[j].b);
//      curIndex++;
//    }
//  }
//  
//  if (LEDindex > LEDCOUNT) {
//    LEDindex = 3;
//  }
//
//  strip.show();
//}
//
//void parseCommand(char commandRaw[numChars], item * changeItem) {
//  Serial.println(commandRaw);
//  Serial.println(receivedChars);
//
//
//  char rawValue[3];
//  char colorIndex[2];
//
//  int value = 0;
//  int colorIndexValue = 0;
//
//  // If the second char is not T that means we are talking about a color
//  if (commandRaw[1] == 't') {
//    Serial.println("it was time");
//    //<1tFFF> <1t000>
//    rawValue[0] = commandRaw[2];
//    rawValue[1] = commandRaw[3];
//    rawValue[2] = commandRaw[4];
//    rawValue[3] = '\0';
//  } else if (commandRaw[1] == 's') {
//    //<1s1> or <1s0>
//    Serial.println("it was start/stop");
//    rawValue[0] = commandRaw[2];
//    rawValue[1] = '\0';
//  } else if(commandRaw[2] == 'l'){
//    colorIndex[0] = commandRaw[1];
//    colorIndex[1] = '\0';
//    rawValue[0] = commandRaw[3];
//    rawValue[1] = commandRaw[4];
//    rawValue[2] = '\0';
//   
//  }else {
//    Serial.println("it was a color");
//    //<11rFF> or <13b0F>
//    colorIndex[0] = commandRaw[1];
//    colorIndex[1] = '\0';
//    rawValue[0] = commandRaw[3];
//    rawValue[1] = commandRaw[4];
//    rawValue[2] = '\0';
//  }
//
//  value = (int)strtol(rawValue, NULL, 16);
//
//  //Switch statements to change color values, easier if they were in array.
//  if (commandRaw[1] == 't') {
//
//    changeItem->t = value;
//
//  } else if (commandRaw[1] == 's') {
//    Serial.println("changing active");
//    
//    changeItem->active = value;
//    Serial.println(commandRaw[2]);
//    Serial.println(testItem.active);
//
//  } else {
//
//    colorIndexValue = (int)strtol(colorIndex, NULL, 16);
//
//    switch (commandRaw[2]) {
//      case 'r':
//        changeItem->colors[colorIndexValue].r = value;
//        break;
//      case 'g':
//        changeItem->colors[colorIndexValue].g = value;
//        break;
//      case 'b':
//        changeItem->colors[colorIndexValue].b = value;
//        break;
//      case 'l':
//        changeItem->colors[colorIndexValue].len = value;
//        break;
//    }
//  }
//
//  //Debug Stuff
////  Serial.println(changeItem->colors[1].r);
////  Serial.println(changeItem->active);
//  Serial.println(testItem.t);
//
//  newCommand = false;
//}
//
//void getCommand() {
//  static boolean recvInProgress = false;
//  static byte recvIndex = 0;
//
//  char startMark = '<';
//  char endMark = '>';
//  char recv;
//
//
//  while (Serial.available() > 0 && newCommand == false) {
//    recv = Serial.read();
//
//    if (recvInProgress == true) {
//      if (recv != endMark) {
//
//        receivedChars[recvIndex] = recv;
//        recvIndex++;
//
//        //May have to bump up numChars
//        if (recvIndex >= numChars) {
//          recvIndex = numChars - 1;
//        }
//
//      }
//      else {
//        receivedChars[recvIndex] = '\0'; // terminate the string
//
//        recvInProgress = false;
//        recvIndex = 0;
//        newCommand = true;
//      }
//    }
//
//    else if (recv == startMark) {
//      recvInProgress = true;
//    }
//  }
//}
