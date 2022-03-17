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

SimpleKalmanFilter simpleKalmanFilter1(2, 2, 0.01);
SimpleKalmanFilter simpleKalmanFilter2(2, 2, 0.01);

// Serial output refresh time
const long SERIAL_REFRESH_TIME = 100;
long refresh_time;
int16_t adc0, adc1, adc2, adc3;
float flow_base;
bool 201_state = 1;
void setup() {
  Serial.begin(115200);
  ads.begin();
}

void loop() {

  // read a reference value from A0 and map it from 0 to 100
  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);

  // calculate the estimated value with Kalman Filter
  float flow = simpleKalmanFilter1.updateEstimate(adc0);
  float ppm = simpleKalmanFilter2.updateEstimate(adc1);

if ((flow - flowbase)/flowbase > 0.25){
  // turn the 201 off
}
if (201_state & ppm < 1){
  //wait for 3 mins and turn the 201 on
}

}
