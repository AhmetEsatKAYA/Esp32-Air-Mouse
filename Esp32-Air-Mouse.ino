#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <BleMouse.h>
Adafruit_MPU6050 mpu;
BleMouse bleMouse("ESP BLE Mouse");

float accX, gyrZ;
int moveX, moveY;
bool state = 0, flag = 0, hdurum = 0;

void setup() {
  Serial.begin(115200);
  pinMode(4, INPUT);
  pinMode(15, INPUT);
  while (!Serial)
    delay(10);
  Wire.begin();
  if (!mpu.begin()) {
    Serial.println("No MPU6050!");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 OK.\nBLE Mouse...");
  bleMouse.begin();
  Serial.println("BLE Mouse begin OK!\nWaiting connection");
  while (!bleMouse.isConnected()) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nBLE Mouse connect OK!");
}

void loop() {
  if (bleMouse.isConnected()) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    accX = a.gyro.x;
    gyrZ = g.gyro.z;
    moveX = map(accX, -10, 10, -127, 127) * 1,25;
    moveY = map(gyrZ, -10, 10, -127, 127) * 1,25;
    if(moveX == 0 && moveY == 0) {
      Serial.println("No move");
    }
    else {
      hdurum = digitalRead(15);
      if (hdurum == 1) {
        bleMouse.move(moveY, moveX);
      }
      else {
        Serial.print("Hold the button! ");
      }
      Serial.print(String(moveX) + ", " + String(moveY) + "\n");
    }
    state = digitalRead(4);
    if (state == 1 && flag == 0) {
      bleMouse.click(MOUSE_LEFT);
      flag = 1;
    }
    else if (state == 0 && flag == 1) {
      flag = 0;
    }
    }
    delay(20);
}
