// 27_EdgeImpulse_FOMO.ino
#define MAX_RESOLUTION_VGA 1

/**
 * Run Edge Impulse FOMO model on the Esp32 camera
 */

// replace with the name of your library
#include <esp32-cam-fire-detection_inferencing.h>
#include "esp32cam.h"
#include "esp32cam/tinyml/edgeimpulse/FOMO.h"


using namespace Eloquent::Esp32cam;

Cam cam;
TinyML::EdgeImpulse::FOMO fomo;
const int ledPin = 4;
int i=0;

void setup() {
    pinMode(ledPin, OUTPUT);

    
    Serial.begin(115200);
    delay(3000);
    Serial.println("Init");

    cam.aithinker();
    cam.highQuality();
    cam.highestSaturation();
    cam.vga();

    while (!cam.begin())
        Serial.println(cam.getErrorMessage());
}

void loop() {
    if (!cam.capture()) {
        Serial.println(cam.getErrorMessage());
        delay(1000);
        return;
    }

    // run FOMO model
    if (!fomo.detectObjects(cam)) {
        Serial.println(fomo.getErrorMessage());
        delay(1000);
        return;
    }

    // print found bounding boxes
    if (fomo.hasObjects()) {
        Serial.printf("Found %d objects in %d millis\n", fomo.count(), fomo.getExecutionTimeInMillis());
        i+=1;
        fomo.forEach([](size_t ix, ei_impulse_result_bounding_box_t bbox) {
            Serial.print(" > BBox of label ");
            Serial.print(bbox.label);
            Serial.print("(");
            Serial.print(bbox.value);
            Serial.print(")");
            Serial.print(" at (");
            Serial.print(bbox.x);
            Serial.print(", ");
            Serial.print(bbox.y);
            Serial.print("), size ");
            Serial.print(bbox.width);
            Serial.print(" x ");
            Serial.print(bbox.height);
            Serial.println();
        });
        Serial.println(i);
        if (i>3){
          digitalWrite(ledPin, HIGH);
        }
        
      

    }
    else {
        Serial.println("No objects detected");
        i=0;
        digitalWrite(ledPin, LOW);
  
    }

}