#if 1 // Change to 0 to disable this code (must enable ONE user*.cpp only!)

#include "globals.h"
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>


// Mask configuration
String maskname = "Bilious";  // Update for mask name: Bilious or Cankerous.
bool animating = false;
bool speaking = false;
uint32_t animStartTime = 0;
uint32_t animTransitionTime = 0;
uint32_t speakingStartTime = 0;
uint32_t speakingTime = 0;

//Neopixel setup
#define LED_PIN 2
#define LED_COUNT  10
byte rbyte = 0x00;
byte gbyte = 0x00;
byte bbyte = 0x00;

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


void mask_speak(byte red, byte green, byte blue, int SparkleDelay, int SpeedDelay) {
  uint32_t startingTime = millis(); 
  strip.fill(red,green,blue);
  int Pixel = random(LED_COUNT);
  strip.setPixelColor(Pixel,0xff,0xff,0xff);
  strip.show();
  while (millis() > startingTime + (SparkleDelay * 1000)){}
  strip.setPixelColor(Pixel,red,green,blue);
  strip.show();
  while (millis() > startingTime + ((SparkleDelay + SpeedDelay) * 1000)) {}
}


void go_speak(int speaktime) {
  if (maskname == "Bilious") {
    rbyte = 0x86; 
    gbyte = 0x8F;
    bbyte = 0x0A;
  } else {
    rbyte = 0xA1; 
    gbyte = 0x0B;
    bbyte = 0x00;
  }
  mask_speak(rbyte, gbyte, bbyte, 10, random(10, 100));
}


void user_setup(void) {
  showSplashScreen = false;
  strip.begin();           // Initialize the NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP (intialized to black, above)
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  Serial.println("Voodoo mask user setup code initialized: " + maskname);
}


void user_loop(void) {
  
  if (!animating && (!speaking)) {
    int size_ = 0;
    String payload;

    if (Serial.available()) {
      payload = Serial.readStringUntil('\n');
      StaticJsonDocument<512> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.println(error.c_str());
        return;
      }

      if (doc["command"] == "speak") {
        go_speak(doc["speakTime"]);
      }

      if (doc["command"] == "loop") {
        moveEyesRandomly = true;
        animTransitionTime = doc["transitionTime"];
      }

      if (doc["command"] == "animate") {
        animating = true;
        animTransitionTime = doc["transitionTime"];
        animStartTime = millis();
        moveEyesRandomly = false;

        if (doc["animation"] == "look_mme") {
          eyeTargetX = look_mme_x();
          eyeTargetY = look_mme_y();
        }

        if (doc["animation"] == "look_other") {
          eyeTargetX = look_other_x();
          eyeTargetY = look_other_y();
        }

        if (doc["animation"] == "look_front") {
          eyeTargetX = 0.0;
          eyeTargetY = 0.0;
        }

        if (doc["animation"] == "look_left") {
          eyeTargetX = -0.9;
          eyeTargetY = 0.0;
        }

        if (doc["animation"] == "look_right") {
          eyeTargetX = 0.9;
          eyeTargetY = 0.0;
        }

        if (doc["animation"] == "look_up") {
          eyeTargetX = 0.0;
          eyeTargetY = 0.9;
        }

        if (doc["animation"] == "look_down") {
          eyeTargetX = 0.0;
          eyeTargetY = -0.9;
        }
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

    if (elapsed <= speakingTime) {
      float ratio = (float)elapsed / (float)speakingTime;
      Serial.println(maskname +": Speaking... " + String(ratio) + "%");
    } else {
      speaking = false;
    } 
  }
}

#endif // 1
