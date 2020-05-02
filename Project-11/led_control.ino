#include <Adafruit_NeoPixel.h>

// WS2812 configuration
#define neopixel_pin 15
#define neopixel_num 64

// LED matrix global variables (Not recommended)
uint8_t brigth_level = 255;
int brigth_old=-1;
uint8_t rgb[3]={255,255,255};

Adafruit_NeoPixel pixels(neopixel_num, neopixel_pin, NEO_GRB + NEO_KHZ800);

void pixel_setup(){
  pixels.begin();
  pixels.clear();
  pixels_on();
}

void pixels_off(){
  pixels.setBrightness(0);
  pixels.show();
}

void pixels_on(){
  //Full ligth
  Serial.println("luces");
  pixels.setBrightness(brigth_level);
  for(int i=0; i<neopixel_num; i++) {
    // Set the color to each pixel 
    pixels.setPixelColor(i, pixels.Color(rgb[0], rgb[1], rgb[2]));
  }
  pixels.show();
}

void brightness_select(uint8_t brightness){

  if(brightness>15 && brigth_level <255){
    brigth_level = brigth_level + 5;
  }
  else if(brightness<15 && brigth_level >10){
    brigth_level = brigth_level - 10;
  }
  pixels.setBrightness(brigth_level);
  pixels.show();
  Serial.println(brigth_level);
}
