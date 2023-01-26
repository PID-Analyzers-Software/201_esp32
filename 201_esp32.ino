#include <SimpleKalmanFilter.h>
#include <Wire.h>
#include "Adafruit_ADS1015.h"
#include <movingAvg.h>                  // https://github.com/JChristensen/movingAvg
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

Adafruit_ADS1015 ads;     /* Use this for the 12-bit version */
float v_lowLimit = 1015;
float v_highLimit = 1090;

int inputPin = 15;
int outputPin = 32;
bool state = true;
float voltage0 = 1050;
//SimpleKalmanFilter simpleKalmanFilter1(2, 2, 0.01);
movingAvg avgFlow(20);
// Serial output refresh time
const long SERIAL_REFRESH_TIME = 100;
long refresh_time;
int16_t adc0, adc1, adc2, adc3;
void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  ads.begin();
  ads.setGain(GAIN_ONE);
  pinMode(outputPin, OUTPUT);
  avgFlow.begin();

  digitalWrite(outputPin, HIGH);
  Serial.println("Start up 3 Minutes delay started");
  SerialBT.println("Start up 3 Minutes delay started");
  delay(3 * 60 * 1000);
  Serial.println("Start up 3 Minutes delay finished");
  SerialBT.println("Start up 3 Minutes delay finished");

  for (int i = 0; i <= 60; i++) {
    voltage0 = avgFlow.reading(1050);
    delay(10);
  }

}

void loop() {
  // read a reference value from A0 and map it from 0 to 100

  adc0 = ads.readADC_SingleEnded(0);
  voltage0 = avgFlow.reading(adc0 * 2);
  Serial.printf("flow=  %.2f mV.\r\n", voltage0);
  SerialBT.printf("flow=  %.2f mV.\r\n", voltage0);

  Serial.println(state);
  SerialBT.println(state);

  //cases:
  if (voltage0 < v_lowLimit) {
    Serial.println("Flow: LOW. Turning ON delay ready to start");
    SerialBT.println("Flow: LOW. Turning ON delay ready to start");

    if (state == true) {
      Serial.println("5 Minutes delay started");
      SerialBT.println("5 Minutes delay started");

      delay(5 * 60 * 1000);

      for (int i = 0; i <= 10; i++) {
        voltage0 = avgFlow.reading(1050);
        delay(10);
      }

      Serial.println("5 Minutes delay finished");
      SerialBT.println("5 Minutes delay finished");

      state = false;
    }
    state = false;
    for (int i = 0; i <= 5; i++) {
      adc0 = ads.readADC_SingleEnded(0);
      voltage0 = avgFlow.reading(adc0 * 2);
      delay(10);
    }
    if (voltage0 < v_lowLimit) {
      Serial.println("Flow still low, turning ON");
      SerialBT.println("Flow still low, turning ON");

      digitalWrite(outputPin, LOW);
    }
    delay(100);
  } else if (voltage0 > v_highLimit) {
    Serial.println("Flow: HIGH. Turning ON delay ready to start");
    SerialBT.println("Flow: HIGH. Turning ON delay ready to start");

    if (state == true) {
      Serial.println("5 Minutes delay started");
      SerialBT.println("5 Minutes delay started");

      delay(5 * 60 * 1000);

      for (int i = 0; i <= 10; i++) {
        voltage0 = avgFlow.reading(1050);
        delay(10);
      }

      state = false;
      Serial.println("5 Minutes delay finished");
      SerialBT.println("5 Minutes delay finished");

    }
    state = false;
    for (int i = 0; i <= 5; i++) {
      adc0 = ads.readADC_SingleEnded(0);
      voltage0 = avgFlow.reading(adc0 * 2);
      delay(10);
    }
    if (voltage0 > v_highLimit) {
      Serial.println("Flow still high, turning ON");
      SerialBT.println("Flow still high, turning ON");

      digitalWrite(outputPin, LOW);
    }
    delay(100);
  }
  else {
    if (state == false) {
      digitalWrite(outputPin, HIGH);
    }
    Serial.println("Flow: Good.");
    SerialBT.println("Flow: Good.");

    Serial.println("Turning OFF");
    SerialBT.println("Turning OFF");

    state = true;
    delay(100);
  }
}
