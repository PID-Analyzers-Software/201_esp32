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
  buttonState = digitalRead(inputPin);
  // read a reference value from A0 and map it from 0 to 100
  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  float voltage0 = simpleKalmanFilter1.updateEstimate(adc0 * 2);
  float voltage1 = adc1 * 2;
  Serial.printf("flow=  %.2f mV. adc1 = %.2f mV.\r\n", voltage0, voltage1);
  if (voltage1 > 900) {
    Serial.println("Input High");
  } else {
    Serial.println("Input Low");
  }
  
  //cases:
  if (voltage0 < v_lowLimit) {
    // turn the 201 off
    digitalWrite(outputPin, HIGH);
    Serial.println("Turning OFF");
    delay(200);
  } else if (voltage1 > 900) {
    Serial.println("Turning ON after 10 seconds (test).");
    delay(10 * 1000);
    digitalWrite(outputPin, LOW);
  }
  delay(100);
}
