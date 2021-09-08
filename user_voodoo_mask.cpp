#if 1 // Change to 0 to disable this code (must enable ONE user*.cpp only!)

#include "globals.h"

static char* maskname = "Bilious";  // Update for mask name. Used as a reference in serial output. Bilious or Cankerous.
static bool animating = false;
static uint32_t animStartTime = 0;
static uint32_t animTransitionTime = 0;
static float lookX = 0.0;
static float lookY = 0.0;


// No-Timer delay
void ntDelay(uint32_t value) {
  uint32_t ntStartTime = millis();

  while (millis() <= ntStartTime + value) {
    yield();
  }
}


// Animations
void look_mme(void) {

  if ( maskname == "Bilious" ) {
    lookX = -0.45;
  }

  if ( maskname == "Cankerous" ) {
    lookX = 0.45;
  }

  lookY = -0.6;
  
}


void look_other(void) {

  if ( maskname == "Bilious" ) {
    lookX = -0.85;
    lookY = 0;
  }

  if ( maskname == "Cankerous" ) {
    lookX = 0.85;
    lookY = 0;
  }
}


void look_kids(void) {
  lookX = 0.25;
  lookY = -0.4;
}


void look_front(void) {
  lookX = 0;
  lookY = 0;
}


void look_loop(void) {
  moveEyesRandomly = true;

}


void cross_eyes(void) {
  // Can't set booped globally. 
}


void user_setup(void) {
  showSplashScreen = false;
}


void user_loop(void) {
  if (!animating) {
    
    if (Serial.available()) {
      
      int command = Serial.parseInt(SKIP_ALL);
      
      switch(command) {
        case 0:
          animating = true;
          animStartTime = millis();
          animTransitionTime = 2500;
          look_loop();
          Serial.write(strcat(maskname, ": look_loop"));
          break;
        case 1:
          animating = true;
          animStartTime = millis();
          animTransitionTime = 3000;
          look_other();
          Serial.write(strcat(maskname, ": look_other"));
          break;
        case 2:
          animating = true;
          animStartTime = millis();
          animTransitionTime = 3000;
          look_mme();
          Serial.write(strcat(maskname, ": look_mme"));
          break;
        case 3:
          animating = true;
          animStartTime = millis();
          animTransitionTime = 2500;
          look_front();
          Serial.write(strcat(maskname, ": look_front"));
          break;
        case 4:
          animating = true;
          look_kids();
          animStartTime = millis();
          animTransitionTime = 3000;
          Serial.write(strcat(maskname, ": look_kids"));
          break;
        case 5:
          animating = true;
          cross_eyes();
          animStartTime = millis();
          animTransitionTime = 2500;
          Serial.write(strcat(maskname, ": cross_eyes"));
          break;
      }
      
      eyeTargetX = lookX;
      eyeTargetY = lookY;
      
    }
  } else {
    
    uint32_t elapsed = millis() - animStartTime;

    if (elapsed < animTransitionTime) {
      float ratio = (float)elapsed / (float)animTransitionTime;
      yield();
    } else {
      animating = false;
    }  
  }
}

#endif // 0
