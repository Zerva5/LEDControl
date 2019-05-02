#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <Adafruit_NeoPixel.h>
//#inclide<Adafruit_Neopixel.h>

bool newCommand = false;

const int numChars = 33;
char receivedChars[numChars];


//Contains color data and the specific attributes of that color
struct colorid {
    //char
    int rgb[3];
    int len;
    int test;
};

typedef struct colorid colorID;

struct item {
    colorID * colors;
    //num
    int colorCount;
    int t;
};

typedef struct item Item;

struct ledStrip{

//    enum LEDTypes LEDType;
    int LEDCount;
    int pin;
    Adafruit_NeoPixel NEO_strip;
    int active;
    int itemCount;
    Item * items;
};

typedef struct ledStrip LEDStrip;
//Item testItem = {.colors = {c1, c2, c3}, .t = 1000, .active=1};

//Function declarations
void ChangeItemCount(LEDStrip * strip, int newItemCount);
void ChangeColorCount(Item * item, int newColorCount);
void Strip_close(LEDStrip * oldStrip);
void Strip_init(LEDStrip * newStrip, int itemCount);
void Item_init(Item * newItem, int colorCount);
void Color_init(colorID * newColor, bool randomColor);

//All strips used in the program, need to recompile to add a new strip.

const int StripCount = 1;
LEDStrip Strips[1];

void setup() {
    int itemCount_d = 1;
    //Initialize strips
    for(int s = 0; s < StripCount; s = s + 1){
        Strip_init(&Strips[s], itemCount_d, 13);
        for(int i = 0; i < Strips[s].itemCount; i = i + 1){
            Item_init(&Strips[s].items[i], 1);
            for(int c = 0; c < Strips[s].items[i].colorCount; c = c + 1){
                Color_init(&Strips[s].items[i].colors[c], false);
            }
        }
    }

    Serial.begin(9600);
    Serial.println("<Ready For Command>");

//    Serial.println(Strips[0].items[0].colors[0].rgb[0]);
//    Serial.println(Strips[0].items[0].colors[0].rgb[1]);
//    Serial.println(Strips[0].items[0].colors[0].rgb[2]);


    LED_default(&Strips[0]);


//    for(int i = 0; i < StripCount; i = i + 1){
//        Strip_close(&Strips[i]);
//    }
}

void loop(){
  getCommand();
  if(newCommand == true){
    parseCommand();
    newCommand = false;
    LED_default(&Strips[0]);
  }

//    Serial.println(Strips[0].items[0].colors[0].test);
    delay(1000);
  
    
}

void LED_default(LEDStrip * strip){
    //Need to implement custom delays
    //Make a tick function, every loop it iterates that tick for each strip and each item within the strip
    //if tick >= maxTick then run and set tick back to 0, if not add one to tick and go again
    //Tick would be in item, new value, current time system would work, no need to change commands
    //Have if tick is negative, have it run LED function more than once in an interation?
    //I like comments
//
    int currentIndex = 0;
    uint32_t color = strip->NEO_strip.Color(strip->items[0].colors[0].rgb[0], strip->items[0].colors[0].rgb[1], strip->items[0].colors[0].rgb[2]);

//    strip->items[0].colors[0].len = 10;
//    Serial.println(strip->items[0].colors[0].test);

    strip->NEO_strip.fill(color, 0, 10);

//      for(int c = 0; c < strip->items[0].colorCount; c = c + 1){
//          uint32_t color = strip->NEO_strip.Color(strip->items[0].colors[c].rgb[0], strip->items[0].colors[c].rgb[1], strip->items[0].colors[c].rgb[2]);
////          strip->NEO_strip.fill(color, currentIndex, strip->items[0].colors[c].len);
//          
//          currentIndex = currentIndex + strip->items[0].colors[c].len;
//      }
    
    strip->NEO_strip.show(); 
}

void parseCommand(){
    //Init strings for all of the seperate command values
    char stripStr[1];
    char itemStr[1];
    char colorStr[1];
    char colorIndexStr[1];
    char valueStr[4];
    
    //
    stripStr[0] = receivedChars[0];
    itemStr[0] = receivedChars[1];
    
    //Convert char to int
    int strip = (int)strtol(stripStr, NULL, 16);

    //Lots of if statements to do the right things with the cetain commands.
    //Probably amore effecient way to do this, seperate functions?
    if(itemStr[0] == 's'){
        //Start stop strip
        valueStr[0] = receivedChars[2];
        int value = (int)strtol(valueStr, NULL, 16);      
        Strips[strip].active = value;
        Serial.println(Strips[strip].active);
    }else if(itemStr[0] == 'a'){
        // Add or remove items to strip
        valueStr[0] = receivedChars[2];
        int value = (int)strtol(valueStr, NULL, 16);
        ChangeItemCount(&Strips[strip], value);
    }else{
        //Its not changina strip value so its an item change
        int item = (int)strtol(itemStr[0], NULL, 16);
        colorStr[0] = receivedChars[2];

        if(colorStr[0] == 'a'){
            //Add or remove colors from item
            valueStr[0] = receivedChars[3];
            int value = (int)strtol(valueStr, NULL, 16);
            ChangeColorCount(&Strips[strip].items[item], value);
        }else if(colorStr[0] == 't'){
            //Change the time value in the strip
            valueStr[0] = receivedChars[3];
            valueStr[1] = receivedChars[4];

            int value = (int)strtol(valueStr, NULL, 16);

            Strips[strip].items[item].t = value;
        }else{
            //Its not changing any item values so its a color change
            int color = (int)strtol(colorStr, NULL, 16);
            colorIndexStr[0] = receivedChars[3];
            Serial.println("Changed Color");

            if(colorIndexStr[0] == 'l'){
                //Change length of color, this is only useful for some led things, not all
                valueStr[0] = receivedChars[4];
                valueStr[1] = receivedChars[5];

                int value = (int)strtol(valueStr, NULL, 16);

                Strips[strip].items[item].colors[color].len = value;
            }else{
                //Not a length change, its changing the color values
                int colorIndex = (int)strtol(colorIndexStr, NULL, 16);
                valueStr[0] = receivedChars[4];
                valueStr[1] = receivedChars[5];

                int value = (int)strtol(valueStr, NULL, 16);

                Strips[strip].items[item].colors[color].rgb[colorIndex] = value;
            }
        }
    }
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

void ChangeItemCount(LEDStrip * strip, int newItemCount){
    int lengthChange = newItemCount - strip->itemCount;


    //Had to minus the item index by 1 due to arrays starting at 0 and the itemcount starting at 1
    if(lengthChange > 0){
        strip->items = realloc(strip->items, sizeof(Item) * newItemCount);
        for (int i = 0; i < lengthChange; ++i) {
            Item_init(&strip->items[i + strip->itemCount], 3);
        }
    }else if(lengthChange < 0){
        for (int i = 0; i < (lengthChange * -1); i) {
            Item_close(&strip->items[strip->itemCount - i - 1]);
        }
        strip->items = realloc(strip->items, sizeof(Item) * newItemCount);
    }
    strip->itemCount = newItemCount;
}

void ChangeColorCount(Item * item, int newColorCount){
    int lengthChange = newColorCount - item->colorCount;
    printf("change: %i\n", lengthChange);

    if(lengthChange > 0){
        item->colors = realloc(item->colors, sizeof(colorID) * newColorCount);
        for (int i = 0; i < lengthChange; i++) {
            Color_init(&item->colors[i + item->colorCount], false);
        }
    }else if(lengthChange < 0){

        for(int i = 0; i < (lengthChange * -1); i++){
            Color_close(&item->colors[item->colorCount - i - 1]);        
        }

        item->colors = realloc(item->colors, sizeof(colorID) * newColorCount);
    }
    item->colorCount = newColorCount;
}

void Color_close(colorID * oldColor){
    //No need to close colors, no malloc happened
    //Here just in case things change and I can edit this instead of my other functions
}

void Item_close(Item * oldItem){
    free(oldItem->colors);
}

void Strip_close(LEDStrip * oldStrip){
    for (int i = 0; i < oldStrip->itemCount; i = i + 1) {
        Item_close(&oldStrip->items[i]);
    }
    free(oldStrip->items);
}

void Strip_init(LEDStrip * newStrip, int itemCount, int pin){

    newStrip->active = 1;

    newStrip->pin = pin;

    newStrip->LEDCount = 100;

//    newStrip->LEDType = default;

    newStrip->itemCount = itemCount;

    newStrip->items = (Item *)malloc(sizeof(Item) * itemCount);

    newStrip->NEO_strip = Adafruit_NeoPixel(newStrip->LEDCount, newStrip->pin, NEO_GRB + NEO_KHZ800);

    newStrip->NEO_strip.begin();
//    newStrip->NEO_strip.clear();
    for(int i = 0; i < newStrip->LEDCount; i = i + 1){
        newStrip->NEO_strip.setPixelColor(i, 0, 0, 0);
    }
    newStrip->NEO_strip.show();

//    Color_init(&newStrip->items[0].colors[0], 3);
//    newStrip->items[0].colors[0].len = 11;
    //    Item_init(&newStrip->items[0], 3);

//    Serial.println(newStrip->items[0].colors[0].len);
//    Serial.println(Strips[0].items[0].colors[0].len);

//


}

void Item_init(Item * newItem, int colorCount){
//    Serial.println("HELLO");

    newItem->t = 1000;

    newItem->colorCount = colorCount;

    newItem->colors = (colorID *)malloc(sizeof(colorID) * colorCount);

//    for(int i = 0; i < colorCount; i = i + 1){
//        Color_init(&newItem->colors[i], false);
//    }
}

void Color_init(colorID * newColor, bool randomColor){
    int newLen = 1;
    newColor->len = newLen;

    if(randomColor == true){
        newColor->rgb[0] = 0;
        newColor->rgb[1] = 0;
        newColor->rgb[2] = 0;
    }else{
        newColor->rgb[0] = 0;
        newColor->rgb[1] = 0;
        newColor->rgb[2] = 0;
    }

}
