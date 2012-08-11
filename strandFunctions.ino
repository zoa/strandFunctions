#include "SPI.h"
#include "Zoa_WS2801.h"
#include "strandFunctions.h"
#include "Audio_monitor.h"
#include "MsTimer2.h"

//#include "johnutils.h"

#define RED 0
#define GREEN 1
#define BLUE 2

/*****************************************************************************
Stuff we need to class up. Tests, etc.
*****************************************************************************/
playFlag_t play_flag = 1;
uint8_t sine_arr[101] = { 128, 143, 159, 174, 189, 202, 215, 226, 
                       235, 243, 249, 253, 255, 255, 253, 249, 
                       243, 235, 226, 215, 202, 189, 174, 159, 
                       143, 128, 112, 96, 81, 66, 53, 40, 29, 
                       20, 12, 6, 2, 1, 1, 2, 6, 12, 20, 29, 
                       40, 53, 66, 81, 96, 112, 127, 143, 159, 
                       174, 189, 202, 215, 226, 235, 243, 249, 
                       253, 255, 255, 253, 249, 243, 235, 226, 
                       215, 202, 189, 174, 159, 143, 128, 112, 
                       96, 81, 66, 53, 40, 29, 20, 12, 6, 2, 
                       1, 1, 2, 6, 12, 20, 29, 40, 53, 66, 81, 
                       96, 112, 127};

uint8_t sine_arr2[10] = { 20, 20, 45, 55, 75,
                          105, 175, 105, 75, 55};
                       
uint32_t test_arr[25] = {4294967040, 4294966272, 4294965504, 4294964736, 
                        4294963968, 4294963200, 4294962432, 4294961664, 
                        4294960896, 4294960128, 4294959360, 4294958592, 
                        4294957824, 4294957056, 4294956288, 4294955520, 
                        4294954752, 4294953984, 4294953216, 4294952448, 
                        4294951680, 4294950912, 4294950144, 4294949376, 
                        4294948608};

int dataPin  = 2;    // Yellow wire on Zoa Pixels
int clockPin = 3;    // Green wire on Zoa Pixels
int stripLen = 20;

// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
Zoa_WS2801 strip = Zoa_WS2801(stripLen, dataPin, clockPin);



void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show();
}

uint16_t global_pause = 1;

void loop() {
  // Some example procedures showing how to display to the pixels
colorWipe(Color(25, 50, 25), 1);
Serial.println(rgbColorsString(strip.getPixelColor(10)));
delay(500);
brighter2(2);
Serial.println(rgbColorsString(strip.getPixelColor(10)));
//dimmer(50);
delay(500);
brighter2(1.5);
Serial.println(rgbColorsString(strip.getPixelColor(10)));
//dimmer(50);
delay(500);
//showSine(35);
//global_pause = (1023 - Audio_monitor::instance().get_amplitude())/10;
}



//all of the above works as best I know, uncomment the below to see if this works
//playColors(test_arr, 25);



void testWave() {
  int i;
float amplitude = 255;
for (i = 0; i < 10000; i++) {
  float fubu = sineWave(amplitude, float(i));
  int out = int(fubu + 0.5);
  Serial.println(out);
  delay(5);
  pushOne(Color(out, 0, 0));
}
}
void pushOne(uint32_t c) {
  uint32_t current_arr[stripLen];
  int i;
  for (i=0; i < stripLen; i++) {
    current_arr[i] = strip.getPixelColor(i);
  }

  for (i=1; i < stripLen; i++) {
    strip.setPixelColor(i, current_arr[i-1]);
  }
  strip.setPixelColor(0, c);
  strip.show();
}

float sinewave(float a, int8_t f, float value) {
  float output = a * sin(((3.14159*2)*f)*value/100);
  return output;
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

// UNTESTED, I can't see this work
// SORTA TESTED AND BROKEN NEEDS HELP
// note this is going to make the whole strip the same color. An interpolation that goes
// from where each pixel is now to a new value that is of equal differential to the 
// other pixels would be highly cool as well. That might be easier with Aprils HSV stuff.
// I can do that with this, but it is going to require another function and some time.

// give this rgb as int8's and a number of steps, it goes from where it is now to that.
void interpolateTo(uint8_t red, uint8_t blue, uint8_t green, uint8_t steps) {
  int i, j, rout, gout, bout;
  rgbInfo_t rgb_info[25];

  //first we need all the starting colors of the strip
  for (i=0; i < stripLen; i++) {
    rgb_info[i] = unpackColors(strip.getPixelColor(i));
  }
  //now we do the mathy bits and step through the color change
  for (i=1; i <=steps; i++) {
    for (j=0; j < stripLen; j++) {
      rout = i * (red - rgb_info[j].r)/(steps - 1);
      gout = i * (green - rgb_info[j].g)/(steps - 1);
      bout = i * (blue - rgb_info[j].b)/(steps - 1);
      strip.setPixelColor(i, Color(rout, gout, bout));
    }
    strip.show();
    //DEBUG
    String outstring;
    outstring = rgbColorsString(strip.getPixelColor(10));
    Serial.println("something!  " + outstring);

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

void maxOut() {
  int i;
  for (i=0; i < stripLen; i ++) {
    uint32_t scolor = strip.getPixelColor(i);
  }
}

void brighter2(float amount){
  int i;
  float incr, incg, incb;
  for (i=0;i < stripLen;i++) {  
    uint32_t scolor = strip.getPixelColor(i);
    rgbInfo_t rgb_info = unpackColors(scolor);

    if (rgb_info.r * amount <= 255) { 
      rgb_info.r = int(rgb_info.r * amount);
    }
    else break;
    
    if (rgb_info.g * amount <= 255) { 
      rgb_info.g = int(rgb_info.g * amount);
    }
    else break;
    
    if (rgb_info.b * amount <= 255) { 
      rgb_info.b = int(rgb_info.b * amount);
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

//VERY OLD STUFF BEING KEPT FOR REFERENCE


// real sine function
float sineWave(float amplitude, float value) {
  float frequency = 5;
  value = value;
  float output = (sin(3.14159*frequency*value/amplitude)+1)/2*amplitude;
//  float output = amplitude * sin(((3.14159 *2) * frequency) * value);
  return output;
}
  
//hack using predefined array
void showSine(uint8_t pause) {
  int i;
  for (i = 0; i < 11; i++) {
  pushOne(Color(sine_arr2[i], 0, 0));
  delay(pause);
  }
}

/*    
uint8_t sineTest(){ 
  int i;
  for (i=1;i < 20; i++) {
    float sineout;
    sineout = sinewave(1.0, 2, float(i));
    Serial.println(sineout);
  }
}
  }
*/
