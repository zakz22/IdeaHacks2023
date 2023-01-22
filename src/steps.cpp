#include <Arduino.h>
#include <steps.h>
#include "led.h"
//#include <WiFi.h>
//#include <AsyncTCP.h>
//#include <ESPAsyncWebSrv.h>
//#include <Firebase_ESP_Client.h>
//#include "Website.h"
//#include "addons/TokenHelper.h"
//#include "addons/RTDBHelper.h"

//accelerometer stuff
const int xAxis = 32;
const int yAxis = 35;
const int zAxis = 34;

int x_acc;
int y_acc;
int z_acc;
int mag;

//float threshold = 6;
int minimum = 1000000;
int maximum = 0;
int mag_sum = 0;
int sum_prev;
int mag_avg;
int threshold;
int counter = 10;

Led led;

steps::steps() {
  // // setup pins
  //pinMode(xAxis, INPUT);
  //pinMode(yAxis, INPUT);
  //pinMode(zAxis, INPUT);

  for (int i = 0; i < 10; i++) {
    x_acc = analogRead(xAxis);
    y_acc = analogRead(yAxis);
    z_acc = analogRead(zAxis);

    mag = sqrt(x_acc*x_acc + y_acc*y_acc + z_acc*z_acc);
    if (mag < minimum){
      minimum = mag;
    } else if (mag > maximum) {
      maximum = mag;
    }
  }

  threshold = 2*counter*(maximum - minimum);
  m_steps = 0;
  
}

void steps::run() {
  mag_sum = 0;

  for (int i = 0; i < counter; i++) {
    x_acc = analogRead(xAxis);
    y_acc = analogRead(yAxis);
    z_acc = analogRead(zAxis);
    mag = sqrt(x_acc*x_acc + y_acc*y_acc + z_acc*z_acc);
    mag_sum = mag_sum + mag;
    //delay(1);
  }

  if (sum_prev > mag_sum + threshold) {
    m_steps++;
    led.SetLeds();
    //Serial.println("+1");
    sum_prev= 0;
  }

  //Serial.println(steps);
  sum_prev = mag_sum;
}