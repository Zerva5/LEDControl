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
    int maxt;
    int curIndex;
    int direction;
};

typedef struct item Item;

struct ledStrip{

//    enum LEDTypes LEDType;
    int active;
    int itemCount;
    Item * items;
};

typedef struct ledStrip LEDStrip;
//Item testItem = {.colors = {c1, c2, c3}, .t = 1000, .active=1};

//Function declarations
int ChangeItemCount(LEDStrip * strip, int newItemCount);
void ChangeColorCount(Item * item, int newColorCount);
void Strip_close(LEDStrip * oldStrip);
int Item_close(Item *);
void Strip_init(LEDStrip * newStrip, int itemCount);
void Item_init(Item * newItem, int colorCount);
void Color_init(colorID * newColor, bool randomColor);
void LED_default(LEDStrip * strip);

//All strips used in the program, need to recompile to add a new strip.
const int LEDCOUNT = 299;
const int StripCount = 1;
LEDStrip Strips[1];
int startIndex = 0;

int itemCount_d = 1;
int colorCount_d = 1;

Adafruit_NeoPixel STRIP (LEDCOUNT, 13, NEO_GRB + NEO_KHZ800);

void setup() {

    //Initialize strips
    for(int s = 0; s < StripCount; s = s + 1){
        Strip_init(&Strips[s], itemCount_d);
        for(int i = 0; i < Strips[s].itemCount; i = i + 1){
            Item_init(&Strips[s].items[i], colorCount_d);
//            for(int c = 0; c < Strips[s].items[i].colorCount; c = c + 1){
//                Color_init(&Strips[s].items[i].colors[c], false);
//            }
        }
    }

//    Strips[0].items[0].curIndex = 30;
    Strips[0].items[0].colors[0].rgb[1] = 255;

//    ChangeItemCount(&Strips[0], 4);
    Strips[0].items[0].curIndex = 10;
    Strips[0].items[0].colors[0].rgb[2] = 0;
    Strips[0].items[0].colors[0].rgb[1] = 0;
//    Strips[0].items[1].curIndex = 15;
//    Strips[0].items[1].colors[0].rgb[0] = 255;
//    Strips[0].items[2].curIndex = 20;
//    Strips[0].items[2].colors[0].rgb[2] = 255;
    

    Serial.begin(9600);
    Serial.println("<Ready For Command>");

    STRIP.begin();

//    Serial.println(Strips[0].items[0].colors[0].rgb[0]);
//    Serial.println(Strips[0].items[0].colors[0].rgb[1]);
//    Serial.println(Strips[0].items[0].colors[0].rgb[2]);


    LED_default(&STRIP, &Strips[0]);


//    for(int i = 0; i < StripCount; i = i + 1){
//        Strip_close(&Strips[i]);
//    }
}

void loop(){
  getCommand();
  if(newCommand == true){
    parseCommand();
    newCommand = false;
//    LED_default(&STRIP, 5);
  }

//    Serial.println(Strips[0].items[0].colors[0].test);
    LED_default(&STRIP, &Strips[0]);
    delay(50);
//    Serial.println(Strips[0].items[0].t);
  
    
}

void LED_default(Adafruit_NeoPixel strip, LEDStrip * stripItem){
    //Need to implement custom delays
    //Make a tick function, every loop it iterates that tick for each strip and each item within the strip
    //if tick >= maxTick then run and set tick back to 0, if not add one to tick and go again
    //Tick would be in item, new value, current time system would work, no need to change commands
    //Have if tick is negative, have it run LED function more than once in an interation?
    //I like comments
//


    STRIP.clear();
//
    for(int i = 0; i < stripItem->itemCount; i++){
//        Serial.println(i);
        int currentIndex = stripItem->items[i].curIndex;
        for(int c = 0; c < stripItem->items[i].colorCount; c = c + 1){
            
            uint32_t color = STRIP.Color(stripItem->items[i].colors[c].rgb[0], stripItem->items[i].colors[c].rgb[1], stripItem->items[i].colors[c].rgb[2]);
            STRIP.fill(color, currentIndex, stripItem->items[i].colors[c].len);
            
            currentIndex = currentIndex + stripItem->items[i].colors[c].len;
    
            if(currentIndex > LEDCOUNT){
                startIndex = 0;
            }
        }

        if(stripItem->active == 1 && stripItem->items[i].t >= stripItem->items[i].maxt){
            if(stripItem->items[i].direction == 1){
                stripItem->items[i].curIndex++;
            }else{
                stripItem->items[i].curIndex--;
            }
            stripItem->items[i].t = 0;
        }else{
            stripItem->items[i].t++;
        }
        if(stripItem->items[i].curIndex > LEDCOUNT){
            stripItem->items[i].curIndex = 0;
        }
        if(stripItem->items[i].curIndex < 0){
            stripItem->items[i].curIndex = LEDCOUNT;
        }
    }

    
    STRIP.show(); 


}

int parseCommand(){
    //Init strings for all of the seperate command values
    char stripStr[2];
    char itemStr[2];
    char colorStr[2];
    char colorIndexStr[2];
    char valueStr[4];
    
    //
    stripStr[0] = receivedChars[0];
    stripStr[1] = '\0';
    itemStr[0] = receivedChars[1];
    itemStr[1] = '\0';
    
    //Convert char to int
    int strip = (int)strtol(stripStr, NULL, 16);

    //Lots of if statements to do the right things with the cetain commands.
    //Probably amore effecient way to do this, seperate functions?
    if(itemStr[0] == 's'){
        //Start stop strip
        valueStr[0] = receivedChars[2];
        valueStr[1] = '\0';
        
        int value = (int)strtol(valueStr, NULL, 16);      
        Strips[strip].active = value;
        Serial.println(Strips[strip].active);
        return 0;
    }else if(itemStr[0] == 'a'){
        // Add or remove items to strip
        valueStr[0] = receivedChars[2];
        valueStr[1] = '\0';

        
        int value = (int)strtol(valueStr, NULL, 16);
        Serial.println(value);

        ChangeItemCount(&Strips[strip], value);
        return 0;
    }else{
        //Its not changina strip value so its an item change
        int item = (int)strtol(itemStr[0], NULL, 16);
        colorStr[0] = receivedChars[2];
        colorStr[1] = '\0';

        if(colorStr[0] == 'a'){
            //Add or remove colors from item
            valueStr[0] = receivedChars[3];
            valueStr[1] = '\0';
            int value = (int)strtol(valueStr, NULL, 16);
            ChangeColorCount(&Strips[strip].items[item], value);
            return 0;
        }else if(colorStr[0] == 't'){
            //Change the time value in the strip
            valueStr[0] = receivedChars[3];
            valueStr[1] = receivedChars[4];
//            valueStr[2] = receivedChars[5];
            valueStr[2] = '\0';

            int value = (int)strtol(valueStr, NULL, 16);

            Strips[strip].items[item].maxt = value;
            Serial.println(value);
            return 0;
        }else if(colorStr[0] == 'd'){
            //CHanging direction of item
            valueStr[0] = receivedChars[3];
            valueStr[1] = '\0';

            int value = (int)strtol(valueStr, NULL, 16);

            Strips[strip].items[item].direction = value;
            return 0;
          
        }else{
            //Its not changing any item values so its a color change
            int color = (int)strtol(colorStr, NULL, 16);
            colorIndexStr[0] = receivedChars[3];
            colorIndexStr[1] = '\0';
            Serial.println("Changed Color");

            if(colorIndexStr[0] == 'l'){
                //Change length of color, this is only useful for some led things, not all
                valueStr[0] = receivedChars[4];
                valueStr[1] = receivedChars[5];
                valueStr[2] = '\0';

                int value = (int)strtol(valueStr, NULL, 16);

                Strips[strip].items[item].colors[color].len = value;
                return 0;
            }else if(colorIndexStr[0] == '0' || colorIndexStr[0] == '1' || colorIndexStr[0] == '2'){
                //Not a length change, its changing the color values
                int colorIndex = (int)strtol(colorIndexStr, NULL, 16);
                valueStr[0] = receivedChars[4];
                valueStr[1] = receivedChars[5];
                valueStr[2] = '\0';


                int value = (int)strtol(valueStr, NULL, 16);
                Serial.println(colorIndex);
                Serial.println(value);

                int rold = Strips[strip].items[item].colors[color].rgb[0];
                int gold = Strips[strip].items[item].colors[color].rgb[1];
                int bold = Strips[strip].items[item].colors[color].rgb[2];


                Strips[strip].items[item].colors[color].rgb[colorIndex] = value;

                Serial.println(rold);
                Serial.println(gold);
                Serial.println(bold);

                Serial.println(Strips[strip].items[item].colors[color].rgb[0]);
                Serial.println(Strips[strip].items[item].colors[color].rgb[1]);
                Serial.println(Strips[strip].items[item].colors[color].rgb[2]);
                return 0;
            }else{
                return 1;
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

int ChangeItemCount(LEDStrip * strip, int newItemCount){
    int lengthChange = newItemCount - strip->itemCount;


    //Had to minus the item index by 1 due to arrays starting at 0 and the itemcount starting at 1
    if(lengthChange > 0){
        strip->items = realloc(strip->items, sizeof(Item) * newItemCount);
        for (int i = 0; i < lengthChange; i++) {
//            int countOld = strip->itemCount;
            Item_init(&strip->items[i + strip->itemCount], colorCount_d);
        }
    }else if(lengthChange < 0){
        Serial.println("Removed Item");
        Serial.println(lengthChange);
        for (int i = 0; i < (lengthChange * -1); i++) {
            Item_close(&strip->items[strip->itemCount - i]);
        }
        strip->items = realloc(strip->items, sizeof(Item) * newItemCount);
//        Serial.println(
    }else{
        return 0;
    }
    Serial.println(strip->itemCount);
    strip->itemCount = newItemCount;
    Serial.println(strip->itemCount);
    return 0;
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
//    Serial.printlm(item->colorCount
}

void Color_close(colorID * oldColor){
    //No need to close colors, no malloc happened
    //Here just in case things change and I can edit this instead of my other functions
}

int Item_close(Item * oldItem){
    free(oldItem->colors);
    Serial.println("closed item");
    return 0;
}

void Strip_close(LEDStrip * oldStrip){
    for (int i = 0; i < oldStrip->itemCount; i = i + 1) {
        Item_close(&oldStrip->items[i]);
    }
    free(oldStrip->items);
}

void Strip_init(LEDStrip * newStrip, int itemCount){

    newStrip->active = 1;

//    newStrip->LEDType = default;

    newStrip->itemCount = itemCount;

    newStrip->items = (Item *)malloc(sizeof(Item) * itemCount);


//    Color_init(&newStrip->items[0].colors[0], 3);
//    newStrip->items[0].colors[0].len = 11;
    //    Item_init(&newStrip->items[0], 3);

//    Serial.println(newStrip->items[0].colors[0].len);
//    Serial.println(Strips[0].items[0].colors[0].len);

//


}

void Item_init(Item * newItem, int colorCount){
//    Serial.println("HELLO");

    newItem->t = 0;
    newItem->maxt = 10;

    newItem->direction = 1;

    newItem->curIndex = 0;

    newItem->colorCount = colorCount;

    newItem->colors = (colorID *)malloc(sizeof(colorID) * colorCount);

    for(int i = 0; i < colorCount; i = i + 1){
        Color_init(&newItem->colors[i], false);
    }
}

void Color_init(colorID * newColor, bool randomColor){
    int newLen = 2;
    newColor->len = newLen;

    if(randomColor == true){
        newColor->rgb[0] = 0;
        newColor->rgb[1] = 0;
        newColor->rgb[2] = 0;
    }else{
        newColor->rgb[0] = 255;
        newColor->rgb[1] = 255;
        newColor->rgb[2] = 255;
    }

}
