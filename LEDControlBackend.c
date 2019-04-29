#include<stdlib.h>
#include<stdbool.h>
#include<stdio.h>

struct colorid {
    //char
    int rgb[3];
    int len;
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

    int active;
    int itemCount;
    Item * items;
};

typedef struct ledStrip LEDStrip;
//Item testItem = {.colors = {c1, c2, c3}, .t = 1000, .active=1};

void Color_init(colorID * newColor, bool randomColor);
void Strip_init(LEDStrip * newStrip, int itemCount);
void Item_init(Item * newItem, int colorCount);
void Strip_close(LEDStrip * oldStrip);
void Item_close(Item * oldItem);
void ChangeItemCount(LEDStrip * strip, int newItemCount);
void ChangeColorCount(Item * item, int newColorCount);

void main() {

    LEDStrip testStrip = {};

    Item test

    Strip_init(&testStrip, 5);

    printf("%i\n", testStrip.items[0].colors[0].len);

    ChangeColorCount(testStrip.items, 1);

    printf("%i\n", testStrip.items[0].colors[0].len);

    Strip_close(&testStrip);
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
        for (int i = 0; i < (lengthChange * -1); ++i) {
            Item_close(&strip->items[strip->itemCount - i - 1]);
        }
        strip->items = realloc(strip->items, sizeof(Item) * newItemCount);
    }
    strip->itemCount = newItemCount;
}

void ChangeColorCount(Item * item, int newColorCount){
    int lengthChange = newColorCount - item->colorCount;
    printf("change: %i\n", lengthChange);

    //Had to minus the item index by 1 due to arrays starting at 0 and the itemcount starting at 1
    if(lengthChange > 0){
        item->colors = realloc(item->colors, sizeof(colorID) * newColorCount);
        for (int i = 0; i < lengthChange; ++i) {
            Color_init(&item->colors[i + item->colorCount], false);
        }
    }else if(lengthChange < 0){
        for (int i = 0; i < (lengthChange * -1); ++i) {
            free(&item->colors[item->colorCount - i - 1]);
        }
        item->colors = realloc(item->colors, sizeof(colorID) * newColorCount);

    }
    item->colorCount = newColorCount;
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

void Strip_init(LEDStrip * newStrip, int itemCount){

    newStrip->active = 1;

    newStrip->itemCount = itemCount;

    newStrip->items = (Item *)malloc(sizeof(Item) * itemCount);

    for(int i = 0; i < itemCount; i = i + 1){
        Item_init(&newStrip->items[i], 3);
    }

}

void Item_init(Item * newItem, int colorCount){

    newItem->t = 1000;

    newItem->colorCount = colorCount;

    newItem->colors = (colorID *)malloc(sizeof(colorID) * colorCount);

    for(int i = 0; i < colorCount; i = i + 1){
        Color_init(newItem->colors, true);
    }
}

void Color_init(colorID * newColor, bool randomColor){

    newColor->len = 1;

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
