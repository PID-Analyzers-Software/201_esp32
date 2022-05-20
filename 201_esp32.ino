#include <SimpleKalmanFilter.h>
#include <Wire.h>
#include "Adafruit_ADS1015.h"

Adafruit_ADS1015 ads;     /* Use this for the 12-bit version */
float v_lowLimit = 1070;
int inputPin = 4;
int outputPin = 5;
SimpleKalmanFilter simpleKalmanFilter1(2, 2, 0.01);

// Serial output refresh time
const long SERIAL_REFRESH_TIME = 100;
long refresh_time;
int16_t adc0, adc1, adc2, adc3;
void setup() {
  Serial.begin(115200);
  ads.begin();
  ads.setGain(GAIN_ONE);
  pinMode(outputPin, OUTPUT);
  pinMode(inputPin, INPUT);
}

void loop() {
  // read a reference value from A0 and map it from 0 to 100
  adc0 = ads.readADC_SingleEnded(0);
  float voltage0 = simpleKalmanFilter1.updateEstimate(adc0 * 2);
  Serial.printf("flow=  %.2f mV.\r\n", voltage0);

  //cases:
  if (voltage0 < v_lowLimit) {
    // turn the 201 off
    Serial.println("Flow: LOW. Turning OFF");
    digitalWrite(outputPin, HIGH);
    delay(200);
  } else {
    Serial.println("Flow: Good. 3 minutes delay started");
    delay(3* 60 * 1000);
    Serial.println("Turning ON");
    digitalWrite(outputPin, LOW);
    delay(200);
  }
}
