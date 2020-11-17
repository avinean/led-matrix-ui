#ifndef _EFFECTSHELPER_H_
#define _EFFECTSHELPER_H_

/*
         FunkyNoise 1.0
        ----------------              
         A Collection Of 
           Animations 
      And Helper Functions 
 
  for two dimensional led effects
     on the 32x32 SmartMatrix.
 
 Tested on PJRCs Teensy 3.1 @ 96Mhz.
 
 With explicit thanks to Daniel Garcia,
   Mark Kriegsmann and Louis Beaudoin.
 
    Written by Stefan Petrick 2014.
     
     hello(at) stefan-petrick . de
 
                  ...
 
 Download the required software first:
 FastLED 3.0
 SmartMatrix
 Arduino IDE 1.0.6
 Teensyduino 1.2

*/

//#include<SmartMatrix.h>
#include "globals.h"
#include "helpers.h"

// the size of your matrix

// used in FillNoise for central zooming
byte CentreX =  (MX_WIDTH / 2) - 1;
byte CentreY = (MX_HEIGHT / 2) - 1;

// a place to store the color palette
CRGBPalette16 currentPalette;

// can be used for palette rotation
// "colorshift"
byte colorshift;

// The coordinates for 3 16-bit noise spaces.
#define NUM_LAYERS 3

uint32_t x[NUM_LAYERS];
uint32_t y[NUM_LAYERS];
uint32_t z[NUM_LAYERS];
uint32_t scale_x[NUM_LAYERS];
uint32_t scale_y[NUM_LAYERS];

// a 3dimensional array used to store the calculated 
// values of the different noise planes
uint8_t noise[NUM_LAYERS][MX_WIDTH][MX_HEIGHT];

// used for the color histogramm
uint16_t values[256];

uint8_t noisesmoothing;

// everything for the button + menu handling
int button1;
int button2;
int button3;
byte mode;
byte pgm;
byte spd;
byte brightness;
byte red_level;
byte green_level;
byte blue_level;





/*
Helpfull functions to keep the actual animation code short. 
 Contains so far:
 
 XY()
 FillNoise(byte layer)
 BasicVariablesSetup
 ShowFrame
 DimAll(byte value)
 CLS
 MergeMethod1(byte colorrepeat)
 MergeMethod2(byte colorrepeat)
 MergeMethod3(byte colorrepeat)
 MergeMethod4(byte colorrepeat)
 ConstrainedMapping(byte layer, byte lower_limit, byte upper_limit, byte colorrepeat)
 ShowAll(uint16_t frames_per_animation)
 ColorCorrection
 beatsin(accum88 beats_per_minute, uint16_t lowest = 0, uint16_t highest = 65535, byte phase = 0)
 
 -----------------------------------------------------------------
 */





// Fill the x/y array with 16-bit noise values 

void FillNoise(byte layer) {

  for(uint8_t i = 0; i < MX_WIDTH; i++) {

    uint32_t ioffset = scale_x[layer] * (i-CentreX);

    for(uint8_t j = 0; j < MX_HEIGHT; j++) {

      uint32_t joffset = scale_y[layer] * (j-CentreY);

      byte data = inoise16(x[layer] + ioffset, y[layer] + joffset, z[layer]) >> 8;

      // Marks data smoothing
      data = qsub8(data,16);
      data = qadd8(data,scale8(data,39));

      uint8_t olddata = noise[layer][i][j];
      uint8_t newdata = scale8( olddata, noisesmoothing ) + scale8( data, 256 - noisesmoothing );
      data = newdata;

      noise[layer][i][j] = data;
    }
  }
}


// Initialise the coordinates of the noise space with random
// values for an altering starting point.
// Set the zoom factor to a moderate level.

void BasicVariablesSetup() {

  // set to reasonable values to avoid a black out
  colorshift = 0;
  noisesmoothing = 200;

  // just any free input pin
  random16_add_entropy(analogRead(18));

  // fill coordinates with random values
  // set zoom levels
  for(int i = 0; i < NUM_LAYERS; i++) {
    x[i] = random16();
    y[i] = random16();
    z[i] = random16();
    scale_x[i] = 6000;
    scale_y[i] = 6000;
  }
  // everything for the menu
  mode = 0;
  spd = 127;
  brightness = 255;
  red_level = 255;
  green_level = 255;
  blue_level = 255;
}



// Dim everything in leds a bit down.

void DimAll(byte value)  
{
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(value);
  }
}


// Delete the leds array.

void CLS()  
{
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = 0;
  }
}


// overlay layers 0&1&2 for color, layer 2 is brightness

void MergeMethod1(byte colorrepeat) { 
  for(uint8_t i = 0; i < MX_WIDTH; i++) {
    for(uint8_t j = 0; j < MX_HEIGHT; j++) {

      // map the noise values down to a byte range
      // layer 0 and 2 interfere for the color
      uint8_t color = ( ( noise[0][i][j] )
        + ( noise[1][i][j] )
        + ( noise[2][i][j] ) )
        / 3; 

      // layer 2 gives the brightness  
      uint8_t   bri = (noise[2][i][j]);

      // assign a color depending on the actual palette
      CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (color + colorshift), bri );

      leds[XY(i,j)] = pixel;
    }
  }
} 


// overlay layers 0&1 for color, layer 2 is brightness

void MergeMethod2(byte colorrepeat) { 
  for(uint8_t i = 0; i < MX_WIDTH; i++) {
    for(uint8_t j = 0; j < MX_HEIGHT; j++) {

      // map the noise values down to a byte range
      // layer 0 and 2 interfere for the color
      uint8_t color = ( ( noise[0][i][j] )
        + ( noise[1][i][j] ) )
        / 2; 

      // layer 2 gives the brightness  
      uint8_t   bri = (noise[2][i][j]);

      // assign a color depending on the actual palette
      CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (color + colorshift), bri );

      leds[XY(i,j)] = pixel;
    }
  }
}


// overlay layers 0&1 for color, brightness is layer1

void MergeMethod3(byte colorrepeat) { 
  for(uint8_t i = 0; i < MX_WIDTH; i++) {
    for(uint8_t j = 0; j < MX_HEIGHT; j++) {

      // map the noise values down to a byte range
      // layer 0 and 2 interfere for the color
      uint8_t color = ( ( noise[0][i][j] )
        + ( noise[1][i][j] ) )
        / 2; 

      // layer 1 gives the brightness  
      uint8_t   bri = noise[1][i][j];

      // assign a color depending on the actual palette
      CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (color + colorshift), bri );

      leds[XY(i,j)] = pixel;
    }
  }
}


// overlay layers 0&1&2 for color, layer 0 is brightness

void MergeMethod4(byte colorrepeat) { 
  for(uint8_t i = 0; i < MX_WIDTH; i++) {
    for(uint8_t j = 0; j < MX_HEIGHT; j++) {

      // map the noise values down to a byte range
      // layer 0 and 2 interfere for the color
      uint8_t color = ( ( noise[0][i][j] )
        + ( noise[1][i][j] )
        + ( noise[2][i][j] ) )
        / 3; 

      // layer 2 gives the brightness  
      uint8_t   bri = (noise[0][i][j]);

      // assign a color depending on the actual palette
      CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (color + colorshift), bri );

      leds[XY(i,j)] = pixel;
    }
  }
} 


// draw the part between lower and upper limit of one layer

void ConstrainedMapping(byte layer, byte lower_limit, byte upper_limit, byte colorrepeat) {

  for(uint8_t i = 0; i < MX_WIDTH; i++) {
    for(uint8_t j = 0; j < MX_HEIGHT; j++) {

      uint8_t data =  noise[layer][i][j] ;

      if ( data >= lower_limit  && data <= upper_limit) {

        CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (data + colorshift), data );

        leds[XY(i,j)] = pixel;
      }
    }
  }
}




void ColorCorrection() {
  for(uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i].r = scale8(leds[i].r, red_level);
    leds[i].g = scale8(leds[i].g, green_level);
    leds[i].b = scale8(leds[i].b, blue_level);
  }
}


// basically beatsin16 with an additional phase

uint16_t beatsin(accum88 beats_per_minute, uint16_t lowest = 0, uint16_t highest = 65535, byte phase = 0)
{
  uint16_t beat = beat16( beats_per_minute);
  uint16_t beatsin = (sin16( beat+(phase*256)) + 32768);
  uint16_t rangewidth = highest - lowest;
  uint16_t scaledbeat = scale16( beatsin, rangewidth);
  uint16_t result = lowest + scaledbeat;
  return result;
}


/*
Some color palettes. 
 Includes the predifined FastLED palettes and custom ones.
 
 -----------------------------------------------------------------
 */


// A red-black palette.

void PaletteRed() {
  currentPalette = CRGBPalette16( 
  CHSV( 0, 255, 255 ), 
  CHSV( 0, 255, 0   ), 
  CHSV( 0, 255, 0   ),
  CHSV( 0, 255, 255)); 
}

void PaletteCustom() {
  currentPalette = CRGBPalette16( 
  CHSV( 40, 255, 255), 
  CHSV( 40, 255, 255), 
  CHSV( 0, 255, 0   ),
  CHSV( 0, 255, 255)); 
}

// Set here a global color palette.
// All the the predefined FastLED palettes:

void Pal() {
  //PaletteRed();
  //PaletteCustom();
  //currentPalette = CloudColors_p;
  //currentPalette = LavaColors_p;
  //currentPalette = OceanColors_p;
  currentPalette = ForestColors_p;
  //currentPalette = RainbowColors_p;
  //currentPalette = RainbowStripeColors_p;
  //currentPalette = PartyColors_p;
  //currentPalette = HeatColors_p;
}


/*
Some useful functions for developing and debugging.
 Contains some magic numbers and is written specificly
 for the SmartMatrix.
 
 
 -----------------------------------------------------------------
 */


// Show the current palette.

void ShowPalette() {
  for(int i = 0; i < NUM_LEDS; i++) {
    byte color = i / (NUM_LEDS / 256);
    byte bri = 255; 
    leds[i] = ColorFromPalette( currentPalette, color, bri );
  }
}


// Show 3 small 16x16 versions of the 3 noise planes
// to keep track what is going on where when.
// Useful to check before you start merging layers.
// Expects a 32x32 matrix to be the output device.

void Show3Layers() {
  for(uint8_t i = 0; i < 16; i++) {
    for(uint8_t j = 0; j < 16; j++) {
      leds[XY(i,j)] = ColorFromPalette( currentPalette, noise[0][i*2][j*2]*2 , 255 );
    }
  }
  for(uint8_t i = 16; i < 32; i++) {
    for(uint8_t j = 0; j < 16; j++) {
      leds[XY(i,j)] = ColorFromPalette( currentPalette, noise[1][(i-16)*2][j*2]*2 , 255 );
    }
  }
  for(uint8_t i = 0; i < 16; i++) {
    for(uint8_t j = 16; j < 32; j++) {
      leds[XY(i,j)] = ColorFromPalette( currentPalette, noise[2][i*2][(j-16)*2]*2 , 255 );
    }
  }
}


void ShowParameters(byte layer) {
  Serial.print("L");
  Serial.print(layer);
  Serial.print(" ");
  Serial.print(x[layer]); 
  Serial.print(" "); 
  Serial.print(y[layer]); 
  Serial.print(" "); 
  Serial.print(z[layer]); 
  Serial.print(" ");
  Serial.print(scale_x[layer]); 
  Serial.print(" ");
  Serial.print(scale_y[layer]); 
  Serial.print(" ");   
}

// output the noise value of noise[layer][0][0]
void SerialWriteNoiseValue(byte layer) {
  Serial.print("Layer");
  Serial.print(layer);
  Serial.print(": ");
  Serial.print(noise[layer][0][0]);
  Serial.print("  ");
}


void ShowMenuValues() {
  // serial print all relevant data
  Serial.print("Mode ");
  Serial.print(mode);
  Serial.print(" PGM ");
  Serial.print(pgm);
  Serial.print(" SPD ");
  Serial.print(spd);
  Serial.print(" BRI ");
  Serial.print(brightness);
  Serial.print(" RED ");
  Serial.print(red_level);
  Serial.print(" GRN ");
  Serial.print(green_level);
  Serial.print(" BLU ");
  Serial.print(blue_level);
  Serial.print(" FPS: ");
  Serial.println(LEDS.getFPS());
}

// under construction!
void ShowNumberDistribution() {
  currentPalette = RainbowColors_p;
  x[0] += 1000;
  y[0] += 1000;
  z[0] += 1000;
  FillNoise(0);
  CLS();
  // clear array
  for(uint16_t i = 0; i < 256; i++) {
    values[i] = 0;
  }
  // count values
  for(uint16_t i = 0; i < 32; i++) {
    for(uint16_t j = 0; j < 32; j++) {
      //if (noise[0][i][j] == 133) values[0]++;
      //if (noise[0][i][j] == 129) values[1]++;
      values[noise[0][i][j]]++;
    }
  }
  // output a part of the result
  for(uint16_t i = 150; i < 170; i++) {
    Serial.print(" ");
    Serial.print(values[i]);
  }
  // draw chart
  for(uint8_t i = 100; i < 132; i++) {
    for(uint8_t j = 0; j < values[i]; j++) {
      leds[XY(i-100, 32-(j/4))] = 0xFF0000;
    }
  }
}


/*
A bunch of animations demonstrating how to creatively combine
 simple functions for nice effects.
 
 MirroredNoise         a symmetric noise pattern
 RedClouds             upstreaming clouds
 Lavalamp1 - Lavlamp 5
 Constrained1
 RelativeMotion1
 Water
 Bubbles1
 ShowPalette
 -----------------------------------------------------------------
 */


// MirroredNoise() draws a symmetric noise pattern
// 108 fps

void MirroredNoise() {
  Serial.println("MirroredNoise");

  // move within the noise space
  x[0] += 100;
  z[0] += 100;
  scale_x[0] = 4000;
  scale_y[0] = 4000;

  // calculate the noise array
  FillNoise(0);

  currentPalette = RainbowStripeColors_p;
  noisesmoothing = 10;

  for(int i = 0; i < MX_WIDTH; i++) {
    for(int j = 0; j < MX_HEIGHT; j++) {
      if ( ! __MODE_STOP__ ) {
        // map the noise values down
        uint16_t index = ( noise[0][i][j] + noise[0][MX_WIDTH - 1 - i][j] ) / 2;
        uint16_t   bri = 255;
        // assign a color from the HSV space
        CRGB color = ColorFromPalette( currentPalette, index, bri);
  
        leds[XY(i,j)] = color;
      } else
        break;
      if ( __MODE_STOP__ )
        break;
    }
  }
}


// RedClouds() draws a constrained noise space with a palette 
// softening the edges.
// Upstreaming red clouds.
// 108 fps

void RedClouds() {
  Serial.println("RedClouds");

  // clear the screenbuffer
  CLS();

  PaletteRed();
  colorshift = 240;

  // move within the noise space
  x[0] = beatsin16(1)*10;
  y[0] += 2000;
  z[0] += 100;
  scale_x[0] = 6000;
  scale_x[0] = 6000;

  // calculate the noise array
  FillNoise(0);

  for(int i = 0; i < MX_WIDTH; i++) {
    if ( ! __MODE_STOP__ ) {
      for(int j = 0; j < MX_HEIGHT; j++) {
        if ( ! __MODE_STOP__ ) {
          // map the noise values down to a byte range
          uint16_t index = noise[0][i][j];
          uint16_t   bri = 255;
          // assign a color depending on the actual palette
          CRGB color = ColorFromPalette( currentPalette, index + colorshift, bri);
    
          // draw only the part lower than the threshold
          if (index < 128) { 
            leds[XY(i,j)] = color;
          }
        }
      }
    }
  }
}


// Lavalamp1
// works good with the RedBlack palette

void Lavalamp1() {  
  Serial.println("Lavalamp1");

  PaletteRed();
  colorshift = 0;

  x[0] = beatsin16(3, 200, 64000);
  y[0] += 100;
  z[0] = 7000;
  scale_x[0] = 6000;
  scale_y[0] = 8000;
  FillNoise(0); 

  x[1] = beatsin16(2, 200, 64000);
  y[1] += 130;
  z[1] = 7000;
  scale_x[1] = 6000;
  scale_y[1] = 8000;
  FillNoise(1);   

  x[2] = beatsin16(4, 200, 6400);
  y[2] += 1000;
  z[2] = 3000;
  scale_x[2] = 7000;
  scale_y[2] = 8000;
  FillNoise(2);

  noisesmoothing = 200;

  MergeMethod1(2);
  //Show3Layers();
}


// with a scrolling palette

void Lavalamp2() {  
  Serial.println("Lavalamp2");

  currentPalette = PartyColors_p;

  noisesmoothing = 200;

  x[0] = beatsin16(3, 200, 64000);
  y[0] = beatsin16(4, 200, 64000);
  z[0] = 7000;
  scale_x[0] = beatsin16(2, 6000, 8000);
  scale_y[0] = beatsin16(1, 4000, 12000);
  FillNoise(0);  

  x[1] = beatsin16(5, 200, 64000);
  y[1] = beatsin16(6, 200, 64000);
  z[1] = 6000;
  scale_x[1] = 6000;
  scale_y[1] = 8000;
  FillNoise(1);  

  x[2] = beatsin16(4, 200, 6400);
  y[2] += 1000;
  z[2] = 3000;
  scale_x[2] = 7000;
  scale_y[2] = 8000;
  FillNoise(2);

  colorshift++;

  MergeMethod1(2);
  //Show3Layers();
}


// a very slow one 

void Lavalamp3() { 
  Serial.println("Lavalamp3");

  noisesmoothing = 40;
  currentPalette = ForestColors_p;

  y[0] += 100;
  z[0] = 7000;
  scale_x[0] = 6000;
  scale_y[0] = 6000;
  FillNoise(0);  

  y[1] += 200;
  z[1] = 3000;
  scale_x[1] = 7000;
  scale_y[1] = 8000;
  FillNoise(1);

  y[2] += 250;
  z[2] = 6000;
  scale_x[2] = 20000;
  scale_y[2] = 8000;
  FillNoise(2);  

  MergeMethod1(1);
  //Show3Layers();
}


// the palette can also be defined within the animation

void Lavalamp4() { 
  Serial.println("Lavalamp4");

  currentPalette = CRGBPalette16( 
  CHSV(   0, 255, 0    ), 
  CHSV(   0, 255, 255  ), 
  CHSV(   0, 255, 0    ),
  CHSV( 160, 255, 255  )); 

  noisesmoothing = 150;

  y[0] += 100;
  z[0] = 7000;
  scale_x[0] = 6000;
  scale_y[0] = 6000;
  FillNoise(0);  

  y[1] += 200;
  z[1] = 3000;
  scale_x[1] = 7000;
  scale_y[1] = 8000;
  FillNoise(1);

  y[2] += 250;
  z[2] = 6000;
  scale_x[2] = 20000;
  scale_y[2] = 8000;
  FillNoise(2);  

  MergeMethod1(2);
  //Show3Layers();
}


// lets play with the scaling of 2 layers

void Lavalamp5() { 
  Serial.println("Lavalamp5");

  currentPalette = CRGBPalette16( 
  CHSV(   0, 255, 0   ), 
  CHSV(   0, 200, 255 ), 
  CHSV(  63, 150, 255 ),
  CHSV( 160, 255, 0   )); 

  noisesmoothing = 50;

  y[0] += 1000;
  z[0] = 7000;
  scale_x[0] = beatsin16(3, 1000, 20000);
  scale_y[0] = 6000;
  FillNoise(0);  

  y[1] += 2000;
  z[1] = 3000;
  scale_x[1] = beatsin16(4, 1000, 20000);
  scale_y[1] = 8000;
  FillNoise(1);

  y[2] += 3000;
  z[2] = 6000;
  scale_x[2] = beatsin16(5, 1000, 20000);
  scale_y[2] = 8000;
  FillNoise(2);  

  MergeMethod2(3);
  //Show3Layers();
}


// 2 layers of constrained noise using differnt palettes for color mapping

void Constrained1() { 
  Serial.println("Constrained1");

  noisesmoothing = 100;
  colorshift = 0;

  x[0] += 2000;
  scale_x[0] = 6000;
  scale_y[0] = 6000;
  FillNoise(0);  

  x[1] -= 2000;
  scale_x[1] = 6000;
  scale_y[1] = 6000;
  FillNoise(1);

  CLS();

  // define a palette used for layer 0
  currentPalette = CRGBPalette16( 
  CHSV(   0, 255, 0   ), 
  CHSV(   0, 255, 0   ),
  CHSV(   0, 255, 255 ),
  CHSV( 160, 255, 0   ));

  // map layer 0 (red) for noise values between 100 and 200
  ConstrainedMapping( 0, 100, 200, 1);

  // palette for the second layer  
  currentPalette = CRGBPalette16( 
  CHSV( 0, 255, 0   ), 
  CHSV( 0, 255, 0 ),
  CHSV( 160, 255, 255 ),
  CHSV( 160, 255, 0   ));

  // map layer 1 (blue) for noise values between 100 and 200
  ConstrainedMapping( 1, 100, 200, 1);
}


// move 2 layers relative to each other

void RelativeMotion1() {  
  Serial.println("RelativeMotion1");

  currentPalette = CRGBPalette16( 
  CHSV(  0, 255, 0   ), 
  CHSV( 80, 255, 255 ),
  CHSV( 60, 255, 255 ),
  CHSV(  0, 255, 0   ));

  colorshift = beatsin8(10);
  noisesmoothing = 100;

  x[0] = 5 * beatsin16(2, 15000, 40000);
  y[0] = 5 * beatsin16(3, 15000, 40000);
  z[0] += 100;
  scale_x[0] = 6000 + beatsin16(30, 0, 4000);
  scale_y[0] = 8000 + beatsin16(27, 0, 4000);
  FillNoise(0);  

  x[1] = x[0] + (5 * beatsin16(30, 0, 10000)) - 25000;
  y[1] = y[0] + (5 * beatsin16(40, 0, 10000)) - 25000;
  z[1] += 100;
  scale_x[1] = 6000 + beatsin16(30, 0, 3000);
  scale_y[1] = 8000 + beatsin16(27, 0, 3000);
  FillNoise(1);  

  MergeMethod3(1);
}


// first approach of a water simulation
// uses a beatsin function with phase shift

void Water() {
  Serial.println("Water");
  

  currentPalette = OceanColors_p;
  colorshift++;
  noisesmoothing = 200;

  // 2 sinewaves shiftet by 63 (90 degrees)
  // results in a circular motion
  x[0] = 10 * beatsin(10, 0, 10000, 0);
  y[0] = 10 * beatsin(10, 0, 10000, 63);
  z[0] += 1000;
  scale_x[0] = 6000;
  scale_y[0] = 8000;
  FillNoise(0); 

  x[1] = x[0] + (10 * beatsin(60, 0, 10000, 0)) - 50000;
  y[1] = y[0] + (10 * beatsin(60, 0, 10000, 63)) - 50000;
  z[1] += 1000;
  scale_x[1] = 6000 ;
  scale_y[1] = 8000;
  FillNoise(1);  

  MergeMethod3(3);
}


// outlined bubbles by constrained mapping + palette

void Bubbles1() {
  Serial.println("Bubbles1");

  noisesmoothing = 200;
  PaletteRed();
  colorshift = 0;

  x[0] = beatsin16(7);
  y[0] += 2000;
  z[0] = 7000;
  scale_x[0] = 6000;
  scale_y[0] = 6000;
  FillNoise(0); 

  x[1] = beatsin16(8);
  y[1] += 3000;
  z[1] = 10000;
  scale_x[1] = 6000;
  scale_y[1] = 6000;
  FillNoise(1); 

  CLS();

  ConstrainedMapping(1, 0, 100, 3);
  ConstrainedMapping(0, 0, 100, 3);
}


// just shows the color gradient

void ShowRedPalette() {
  Serial.println("ShowRedPalette");
  PaletteRed();
  ShowPalette();
}


// layer2 movving arround a layer1 moving arround a layer0
void TripleMotion() {
  Serial.println("TripleMotion");

  currentPalette = RainbowColors_p;
  colorshift++;
  noisesmoothing = 200;

  x[0] = 10 * beatsin(10, 0, 10000, 0);
  y[0] = 10 * beatsin(9, 0, 10000, 63);
  z[0] += 1000;
  scale_x[0] = 6000;
  scale_y[0] = 8000;
  FillNoise(0); 

  x[1] = x[0] + (10 * beatsin(13, 0, 10000, 0));
  y[1] = y[0] + (10 * beatsin(12, 0, 10000, 63));
  z[1] += 1000;
  scale_x[1] = 6000;
  scale_y[1] = 8000;
  FillNoise(1);  

  x[2] = x[1] + (10 * beatsin(18, 0, 10000, 0));
  y[2] = y[1] + (10 * beatsin(17, 0, 10000, 63));
  z[2] += 1000;
  scale_x[2] = 6000;
  scale_y[2] = 8000;
  FillNoise(2);  

  MergeMethod4(2);
  //Show3Layers();
}


// Update leds and show fps
// 216 fps when calling nothing else

void ShowFrame() {

  // update leds
  LEDS.show();

  // count and output the fps
//  LEDS.countFPS();

  // output debugging infos
//  ShowMenuValues();
}

void ShowAll(uint16_t count) {
  for(uint16_t i = 0; i < count; i++) {
    MirroredNoise();
    ShowFrame();
  }  

  for(uint16_t i = 0; i < count; i++) {
    RedClouds();
    ShowFrame();
  }

  for(uint16_t i = 0; i < count; i++) {
    Lavalamp1();
    ShowFrame();
  }

  for(uint16_t i = 0; i < count; i++) {
    Lavalamp2();
    ShowFrame();
  }

  for(uint16_t i = 0; i < count; i++) {
    Lavalamp3();
    ShowFrame();
  }


  for(uint16_t i = 0; i < count; i++) {
    Lavalamp4();
    ShowFrame();
  }

  for(uint16_t i = 0; i < count; i++) {
    Lavalamp5();
    ShowFrame();
  }

  for(uint16_t i = 0; i < count; i++) {
    Constrained1();
    ShowFrame();
  }

  for(uint16_t i = 0; i < count; i++) {
    RelativeMotion1();
    ShowFrame();
  }

  for(uint16_t i = 0; i < count; i++) {
    Water();
    ShowFrame();
  }

  for(uint16_t i = 0; i < count; i++) {
    Bubbles1();
    ShowFrame();
  }
  
  for(uint16_t i = 0; i < count; i++) {
    TripleMotion();
    ShowFrame();
  }


}




void setupEffects() {
  // switch dithering off to avoid flicking at low fps
  FastLED.setDither(0);
  
  // adjust the gamma curves to human perception
//  pSmartMatrix->setColorCorrection(cc48);
  
  // fill all animation variables with valid values to
  // allow straight forward animation programming
  BasicVariablesSetup();
}


void effectsLoop() {

  /*
  // Use that when having an input device
   ReadButtons();
   RunAnimationDependingOnPgm();
   ColorCorrection();
   ShowFrame();
   */

  // use that to see all
  // the number names the frames per animation
  ShowAll(500);

  /*
  // use that to run a single animation
   TripleMotion();
   ShowFrame();
   */
} 

void RunAnimationDependingOnPgm() {
  switch (_EFFECT_ID) {              

  case 0:
    MirroredNoise();     
    break;

  case 1:
    RedClouds();     
    break;

  case 2:
    Lavalamp1();     
    break;

  case 3:
    Lavalamp2();     
    break;

  case 4:
    Lavalamp3();     
    break;

  case 5:
    Lavalamp4();     
    break;

  case 6:
    Lavalamp5();     
    break;

  case 7:
    Constrained1();     
    break;

  case 8:
    RelativeMotion1();     
    break;

  case 9:
    Water();     
    break;

  case 10:
    Bubbles1();     
    break;
    
  case 11:
    TripleMotion();     
    break;  
  }
}

#endif
