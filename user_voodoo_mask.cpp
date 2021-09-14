#if 1 // Change to 0 to disable this code (must enable ONE user*.cpp only!)

#include "globals.h"
#include <Adafruit_NeoPixel.h>

String maskname = "Bilious";  // Update for mask name: Bilious or Cankerous.
static bool animating = false;
static bool speaking = false;
uint32_t animStartTime = 0;
uint32_t animTransitionTime = 0;
uint32_t speakingStartTime = 0;
uint32_t speakingTime = 0;
String stdata = "";

// Define animation mappings for meaningful switch statements
#define LOOP 0
#define LOOK_MME 1
#define LOOK_OTHER 2
#define LOOK_FRONT 3
#define LOOK_LEFT 4
#define LOOK_RIGHT 5
#define LOOK_UP 6
#define LOOK_DOWN 7
#define SPEAKING 8

//Neopixel setup
#define LED_PIN    D2
#define LED_COUNT  10
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


// Positional helpers
float look_mme_x(void) {

  if ( maskname == "Bilious" ) {
    return -0.45;
  }

  if ( maskname == "Cankerous" ) {
    return 0.45;
  }
}


float look_mme_y(void) {
  return -0.6;
}


float look_other_x(void) {

  if ( maskname == "Bilious" ) {
    return -0.85;
  }

  if ( maskname == "Cankerous" ) {
    return 0.85;
  }
}


float look_other_y(void) {
  return 0.0;
}


void maskSpeak(byte red, byte green, byte blue, int SparkleDelay, int SpeedDelay) {
  uint32_t startingTime = millis(); 
  strip.fill(red,green,blue);
  int Pixel = random(LED_COUNT);
  strip.setPixelColor(Pixel,0xff,0xff,0xff);
  strip.show();
  while (millis() > startingTime + (SparkleDelay * 1000)){
    yield();
  }
  strip.setPixelColor(Pixel,red,green,blue);
  strip.show();
  while (millis() > startingTime + ((SparkleDelay + SpeedDelay) * 1000)) {
    yield();  
  }
}


void user_setup(void) {
  showSplashScreen = false;
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  Serial.println("Voodoo mask user setup code initialized: " + maskname);
  
  if (maskname == "Bilious") {
    static byte basecolor[] = {0x86, 0x8F, 0x0A};
  } else {
    static byte basecolor[] = {0xA1, 0x0B, 0x00};
  }
}


void user_loop(void) {
  byte ch;
  
  if (!animating) {
    
    if (Serial.available()) {
      ch = Serial.read();
      stdata += (char)ch;

      if (ch=='\r') {  // Command recevied and ready.
         stdata.trim();
         if (stdata.length() == 1) {

          switch(stdata.toInt()) {
            case LOOP:
              moveEyesRandomly = true;
              animTransitionTime = 2000;
              break;
            case LOOK_MME:
              moveEyesRandomly = false;
              animTransitionTime = 2000;
              eyeTargetX = look_mme_x();
              eyeTargetY = look_mme_y(); 
              break;
            case LOOK_OTHER:
              moveEyesRandomly = false;
              animTransitionTime = 2000;
              eyeTargetX = look_other_x();
              eyeTargetY = look_other_y();
              break;
            case LOOK_FRONT:
              moveEyesRandomly = false;
              animTransitionTime = 2000;
              eyeTargetX = 0.0;
              eyeTargetY = 0.0;
              break;
            case LOOK_LEFT:
              moveEyesRandomly = false;
              animTransitionTime = 2000;
              eyeTargetX = -0.9;
              eyeTargetY = 0.0;
              break;
            case LOOK_RIGHT:
              moveEyesRandomly = false;
              animTransitionTime = 2000;
              eyeTargetX = 0.9;
              eyeTargetY = 0.0;
              break;
            case LOOK_UP:
              moveEyesRandomly = false;
              animTransitionTime = 2000;
              eyeTargetX = 0.0;
              eyeTargetY = 0.9;
              break;
            case LOOK_DOWN:
              moveEyesRandomly = false;
              animTransitionTime = 2000;
              eyeTargetX = 0.0;
              eyeTargetY = -0.9;
              break;
            case SPEAKING:
              speaking = true;
              speakingTime = 2000;
              speakingStartTime = millis();
              maskSpeak(basecolor[0], basecolor[1], basecolor[2], 10, random(10, 100));
          }
          
          animating = true;
          animStartTime = millis();
          
         }
         
         stdata = ""; // Clear the string ready for the next command.
      }
      
    }
  } else {
    
    uint32_t elapsed = millis() - animStartTime;

    if (elapsed <= animTransitionTime) {
      float ratio = (float)elapsed / (float)animTransitionTime;
      Serial.println(maskname +": Animating... " + String(ratio) + "%");
    } else {
      animating = false;
    }  
  }

  if (speaking) {
    uint32_t elapsed = millis() - speakingStartTime;

    if (elapsed <= speakingTransitionTime) {
      float ratio = (float)elapsed / (float)speakingTransitionTime;
      Serial.println(maskname +": Speaking... " + String(ratio) + "%");
    } else {
      speaking = false;
    } 
  }
}

#endif // 1
