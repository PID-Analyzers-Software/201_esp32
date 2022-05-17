#include <SimpleKalmanFilter.h>
#include <Wire.h>
#include "Adafruit_ADS1015.h"

Adafruit_ADS1015 ads;     /* Use this for the 12-bit version */
int buttonState = 0;
float v_lowLimit = 1;
int inputPin = 2;
int outputPin = 5;
SimpleKalmanFilter simpleKalmanFilter1(2, 2, 0.01);

// Serial output refresh time
const long SERIAL_REFRESH_TIME = 100;
long refresh_time;
int16_t adc0, adc1, adc2, adc3, voltage;
float flow_base;
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
  float flow = simpleKalmanFilter1.updateEstimate(adc0);
  Serial.println(buttonState);
  Serial.println(adc0);
  Serial.printf("flow=  '', v = %.2f v. \r\n", flow);

  //cases:
  if (flow < v_lowLimit) {
    // turn the 201 off
    digitalWrite(outputPin, LOW);
  } else if(buttonState == LOW){
    digitalWrite(outputPin, HIGH);
  }

  if (buttonState == HIGH) {
    Serial.println("Turning ON after 5 seconds. (will change to 5 minutes after test");
    delay(5*1000);
    digitalWrite(outputPin, HIGH);
  }
  delay(500);

}
