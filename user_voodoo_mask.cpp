#if 1 // Change to 0 to disable this code (must enable ONE user*.cpp only!)

#include "globals.h"
#include <Adafruit_NeoPixel.h>

String maskname = "Bilious";  // Update for mask name: Bilious or Cankerous.
static bool animating = false;
uint32_t animStartTime = 0;
uint32_t animTransitionTime = 0;
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

//Neopixel setup
#define LED_PIN    8
#define LED_COUNT  10
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
if (maskname == "Bilious") {
  uint32_t baseColor = strip.Color(60, 200, 20);
}

if (maskname == "Cankerous") {
  uint32_t baseColor = strip.Color(200, 0, 20);
}



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


void speaking(void) {
  //code goes here
  yield();
}


void user_setup(void) {
  showSplashScreen = false;
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  Serial.println("Voodoo mask user setup code initialized.");
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
              break;
            case LOOK_MME:
              moveEyesRandomly = false;
              animating = true;
              animTransitionTime = 2000;
              eyeTargetX = look_mme_x();
              eyeTargetY = look_mme_y(); 
              break;
            case LOOK_OTHER:
              moveEyesRandomly = false;
              animating = true;
              animTransitionTime = 2000;
              eyeTargetX = look_other_x();
              eyeTargetY = look_other_y();
              break;
            case LOOK_FRONT:
              moveEyesRandomly = false;
              animating = true;
              animTransitionTime = 2000;
              eyeTargetX = 0.0;
              eyeTargetY = 0.0;
              break;
            case LOOK_LEFT:
              moveEyesRandomly = false;
              animating = true;
              animTransitionTime = 2000;
              eyeTargetX = -0.9;
              eyeTargetY = 0.0;
              break;
            case LOOK_RIGHT:
              moveEyesRandomly = false;
              animating = true;
              animTransitionTime = 2000;
              eyeTargetX = 0.9;
              eyeTargetY = 0.0;
              break;
            case LOOK_UP:
              moveEyesRandomly = false;
              animating = true;
              animTransitionTime = 2000;
              eyeTargetX = 0.0;
              eyeTargetY = 0.9;
              break;
            case LOOK_DOWN:
              moveEyesRandomly = false;
              animating = true;
              animTransitionTime = 2000;
              eyeTargetX = 0.0;
              eyeTargetY = -0.9;
              break;
          }
         }
         
         stdata = ""; // Clear the string ready for the next command.
      }
      
    }
  } else {
    
    uint32_t elapsed = millis() - animStartTime;

    if (elapsed <= animTransitionTime) {
      float ratio = (float)elapsed / (float)animTransitionTime;
      Serial.println(maskname +": Animating...");
    } else {
      animating = false;
    }  
  }
}

#endif // 1
