#include <SimpleKalmanFilter.h>
#include <Wire.h>
#include "Adafruit_ADS1015.h"
/*
  SimpleKalmanFilter(e_mea, e_est, q);
  e_mea: Measurement Uncertainty
  e_est: Estimation Uncertainty
  q: Process Noise
*/
// Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
Adafruit_ADS1015 ads;     /* Use this for the 12-bit version */
int buttonState = 0;

SimpleKalmanFilter simpleKalmanFilter1(2, 2, 0.01);

// Serial output refresh time
const long SERIAL_REFRESH_TIME = 100;
long refresh_time;
int16_t adc0, adc1, adc2, adc3;
float flow_base;
void setup() {
  Serial.begin(115200);
  ads.begin();
  pinMode(5, OUTPUT);
  pinMode(4, INPUT);
}

void loop() {
  buttonState = digitalRead(4);
  // read a reference value from A0 and map it from 0 to 100
  adc0 = ads.readADC_SingleEnded(0);
  // calculate the estimated value with Kalman Filter
  float flow = simpleKalmanFilter1.updateEstimate(adc0);
  Serial.printf("flow v = %.2f v. \r\n", flow);

  //cases:
  if ((flow - flow_base) / flow_base > 0.25) {
    // turn the 201 off
    digitalWrite(5, LOW);
  }

  if(buttonState == HIGH){
    Serial.println("Turning ON after 3 minutes.");
    delay(180000);
    digitalWrite(5,HIGH);
  }
  

}
