#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#define PIN 13
#define LEDCOUNT 299

int r,g,b, LEDindex;

Adafruit_NeoPixel strip(LEDCOUNT, PIN, NEO_GRB + NEO_KHZ800);

const byte numChars = 33;
char receivedChars[numChars];

bool newCommand = false;

//<1>

struct colorid{
    //char
    int id;

    int r;
    int g;
    int b;
}colorID_d = {0, 0, 0, 0};

typedef struct colorid colorID;

struct item{
    colorID colors[3];
    //num
    int id;

    int t;
    int active;
}item_d = {{colorID_d, colorID_d, colorID_d}, 0, 0, 1};

typedef struct item Item;


Item testItem = item_d;


void setup() {
    Serial.begin(9600);
    Serial.println("<Ready For Command>");

    strip.begin();
    strip.show();

    LEDindex = 5;
}

void loop() {
    getCommand();
    if(newCommand == true){
        parseCommand(receivedChars, &testItem);
    }
    
    if(testItem.active = 1){
        LEDCycle();

        delay(testItem.t);
    }

}

void LEDCycle(){

    LEDindex = LEDindex + 1;

    strip.setPixelColor(LEDindex - 2, 0, 0, 0);
    
    strip.setPixelColor(LEDindex - 1,testItem.colors[1].r, testItem.colors[1].g, testItem.colors[1].b); 
    
    strip.setPixelColor(LEDindex, testItem.colors[2].r, testItem.colors[2].g, testItem.colors[2].b);

    strip.setPixelColor(LEDindex + 1, testItem.colors[3].r, testItem.colors[3].g, testItem.colors[3].b);


    if(LEDindex > LEDCOUNT){
      LEDindex = 3;
    }
    
    strip.show();
}

void parseCommand(char commandRaw[numChars], item * changeItem){

    int id = commandRaw[0] - '0';

    char rawValue[3];
    char colorIndex[2];

    int value = 0;
    int colorIndexValue = 0;

// If the second char is not T that means we are talking about a color
    if(commandRaw[1] == 't'){
        //<1tFFF> <1t000>
        rawValue[0] = commandRaw[2];
        rawValue[1] = commandRaw[3];
        rawValue[2] = commandRaw[4];
        rawValue[3] = '\0';
    }else if(commandRaw[1] == 's'){
      //<1s1> or <1s0>
        rawValue[0] = commandRaw[2];
        rawValue[1] = '\0';
    }else{
        //<11rFF> or <13b0F>
        colorIndex[0] = commandRaw[1];
        colorIndex[1] = '\0';
        rawValue[0] = commandRaw[3];
        rawValue[1] = commandRaw[4];
        rawValue[2] = '\0';
    }

    value = (int)strtol(rawValue, NULL, 16);

//Switch statements to change color values, easier if they were in array.
    if(commandRaw[1] == 't'){
        changeItem->t = value;
    }else if(commandRaw[1] == 's'){
        changeItem->active = value;      
    }else{
        colorIndexValue = (int)strtol(colorIndex, NULL, 16);

        switch(commandRaw[2]){
            case 'r':
                changeItem->colors[colorIndexValue].r = value;
                break;
            case 'g':
                changeItem->colors[colorIndexValue].g = value;
                break;
            case 'b':
                changeItem->colors[colorIndexValue].b = value;
                break;
        }
    }

    Serial.println(changeItem->colors[1].r);
    Serial.println(changeItem->active);
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
