#include "SPI.h"
#include "Adafruit_WS2801.h"
#include "strandFunctions.h"

//#include "johnutils.h"

#define RED 0
#define GREEN 1
#define BLUE 2

/*****************************************************************************
Stuff we need to class up. Tests, etc.
*****************************************************************************/
playFlag_t play_flag = 1;
uint32_t test_arr[25] = {4294967040, 4294966272, 4294965504, 4294964736, 
                        4294963968, 4294963200, 4294962432, 4294961664, 
                        4294960896, 4294960128, 4294959360, 4294958592, 
                        4294957824, 4294957056, 4294956288, 4294955520, 
                        4294954752, 4294953984, 4294953216, 4294952448, 
                        4294951680, 4294950912, 4294950144, 4294949376, 
                        4294948608};

int dataPin  = 2;    // Yellow wire on Adafruit Pixels
int clockPin = 3;    // Green wire on Adafruit Pixels
int stripLen = 25;

// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
Adafruit_WS2801 strip = Adafruit_WS2801(stripLen, dataPin, clockPin);



void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show();
}


void loop() {
  // Some example procedures showing how to display to the pixels
 
   
colorWipe(Color(50, 0, 0), 1);
more(BLUE, 30);
delay(1000);
more(GREEN, 200);
delay(1000);
less(GREEN, 100);
delay(1000);
brighter(40);
delay(1000);
brighter(40);
delay(1000);
dimmer(25);
delay(1000);
rainbow(1);

//all of the above works as best I know, uncomment the below to see if this works
//playColors(test_arr, 25);
}

//public stuff
// more COLOR, how much
void more(int color, int amount){
  int i;
  for (i=0;i < stripLen;i++) {
    uint32_t scolor = strip.getPixelColor(i);
    rgbInfo_t rgb_info = unpackColors(scolor);
    
    switch (color) {
      case RED:
        if ((rgb_info.r + amount) < 255) {
          rgb_info.r = rgb_info.r + amount;
        }
        break;
       case GREEN:
        if ((rgb_info.g + amount) < 255) {
          rgb_info.g = rgb_info.g + amount;
        }
        break;
       case BLUE:
        if ((rgb_info.b + amount) < 255) {
          rgb_info.b = rgb_info.b + amount;
        }
        break;
    }
    strip.setPixelColor(i, Color( rgb_info.r,
                                  rgb_info.g,
                                  rgb_info.b));
    strip.show();
  }
}

// less COLOR, how much
void less(int color, int amount){
  int i;
  for (i=0;i < stripLen;i++) {
    uint32_t scolor = strip.getPixelColor(i);
    rgbInfo_t rgb_info = unpackColors(scolor);
    
    switch (color) {
      case RED:
        if ((rgb_info.r - amount) > 0) {
          rgb_info.r = rgb_info.r - amount;
        }
        break;
       case GREEN:
        if ((rgb_info.g - amount) > 0) {
          rgb_info.g = rgb_info.g - amount;
        }
        break;
       case BLUE:
        if ((rgb_info.b - amount) > 0) {
          rgb_info.b = rgb_info.b - amount;
        }
        break;
    }
    strip.setPixelColor(i, Color( rgb_info.r,
                                  rgb_info.g,
                                  rgb_info.b));
    strip.show();
  }
}

// how much brighter
// known issue here: if you have pixels with a color at 255 already,
// your color balance is going to get screwed as the other colors
// go up and that one doesn't. 
// TODO: Add precheck for string that none of this is going to push
// the color balance out at the top end.
void brighter(int amount){
  int i;
  float incr, incg, incb;
  for (i=0;i < stripLen;i++) {  
    uint32_t scolor = strip.getPixelColor(i);
    rgbInfo_t rgb_info = unpackColors(scolor);
    //so we need to make the whole thing brighter, but have
    //to increment each color proportionally or the balance is 
    //screwed.
    incr = (float(rgb_info.r)/255)*amount;
    //Remember to help out the little people
    if (incr < 1) incr = 1;
    if ((int(incr +0.5))+rgb_info.r <= 255) { 
      rgb_info.r = rgb_info.r + (int(incr+0.5));
    }
    else break;
    
    incg = (float(rgb_info.g)/255)*amount;
    if (incg < 1) incg = 1;
    if ((int(incg +0.5))+rgb_info.g <= 255) {
      rgb_info.g = rgb_info.g + (int(incg+0.5));
    }
    else break;
    
    incb = (float(rgb_info.b)/255)*amount;   
    if (incb < 1) incb = 1;
    if ((int(incb +0.5))+rgb_info.b <= 255) {
      rgb_info.b = rgb_info.b + (int(incb+0.5));
    }
    else break;
    strip.setPixelColor(i, Color( rgb_info.r,
                                  rgb_info.g, 
                                  rgb_info.b));
    //this here shows the diff one pixel at a time.
   //to make it just show the whole thing, move past bracket.
  //is only going to matter if a LOT of crap is going on in ard. 
    strip.show();
  }
}

// how much darker
void dimmer(int amount){
  int i;
  float incr, incg, incb;
  for (i=0;i < stripLen;i++) {  
    uint32_t scolor = strip.getPixelColor(i);
    rgbInfo_t rgb_info = unpackColors(scolor);
    //so we need to make the whole thing brighter, but have
    //to increment each color proportionally or the balance is 
    //screwed.
    incr = (float(rgb_info.r)/255)*amount;
    //Remember to help out the little people
    if (incr < 1) incr = 1;
    if (rgb_info.r - (int(incr +0.5)) > 0) { 
      rgb_info.r = rgb_info.r - (int(incr+0.5));
    }
    else break;
    
    incg = (float(rgb_info.g)/255)*amount;
    if (incg < 1) incg = 1;
    if (rgb_info.g - (int(incg +0.5)) > 0) {
      rgb_info.g = rgb_info.g - (int(incg+0.5));
    }
    else break;
    
    incb = (float(rgb_info.b)/255)*amount;   
    if (incb < 1) incb = 1;
    if (rgb_info.b - (int(incb +0.5)) > 0) {
      rgb_info.b = rgb_info.b + (int(incb+0.5));
    }
    else break;
    strip.setPixelColor(i, Color( rgb_info.r,
                                  rgb_info.g, 
                                  rgb_info.b));
    //this here shows the diff one pixel at a time.
   //to make it just show the whole thing, move past bracket.
  //is only going to matter if a LOT of crap is going on in ard. 
    strip.show();
  }
}

// array color seq, delay
//UNTESTED
void playColors(uint32_t colors[], uint8_t arr_len){
  int i;
  int strip_pos = 0;
  
  while (play_flag) {
    strip.setPixelColor(strip_pos, colors[i]);
    if (strip_pos == (stripLen -1)) strip_pos = 0;
    if (i == (arr_len - 1)) i = 0;
    strip.show();
  } 
}

//takes rgb tripes in an array and plays them
void playRGB(){
}


//utility stuff
rgbInfo_t unpackColors(uint32_t c){
  rgbInfo_t rgb_info;
  rgb_info.b = lowByte(c); 
  c >>= 8;
  rgb_info.g = lowByte(c);
  c >>= 8;
  rgb_info.r = lowByte(c);
  return(rgb_info);
}

//untested
//give it a Color value and it gives you a string with RGB. For troubleshooting.
String rgbColorsString (uint32_t c){
  rgbInfo_t rgb_info = unpackColors(c);
  String colors;
  String intro = "RGB: ";
  colors = intro + rgb_info.r + ", " + rgb_info.g + ", " + rgb_info.b;
  return (colors);
}

// original functions in lady ada's stuff

void rainbow(uint8_t wait) {
  int i, j;
   
  for (j=0; j < 256; j++) {     // 3 cycles of all 256 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 255));
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
  int i, j;
  
  for (j=0; j < 256 * 5; j++) {     // 5 cycles of all 25 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

/* Helper functions */

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
